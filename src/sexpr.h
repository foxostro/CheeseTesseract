#ifndef SEXPR_H
#define SEXPR_H

/** Lisp-like expressions with bindable functions */
class sexpr_t
{
public:
	class tree_node_t
	{
	public:
		virtual string toString() const = 0;
		virtual void fromString(const string &s) = 0;
	};

	template<typename TYPE>
	class tree_leaf_base_t : public tree_node_t
	{
	public:
		tree_leaf_base_t(TYPE _value,
		                 function<string (TYPE)> _fnToString,
		                 function<TYPE (string)> _fnFromString)
		: fnToString(_fnToString),
		  fnFromString(_fnFromString)
		{
			value = _value;
		}

		inline string toString() const
		{
			return fnToString(value);
		}

		inline void fromString(const string &s)
		{
			value = fnFromString(s);
		}

		inline TYPE getValue() const
		{
			return value;
		}

	private:
		TYPE value;
		function<string (TYPE)> fnToString;
		function<TYPE (string)> fnFromString;
	};

	template<typename TYPE>
	class tree_leaf_t;
	
	template<>
	class tree_leaf_t<string> : public tree_leaf_base_t<string>
	{
	public:
		tree_leaf_t(const string &s)
		: tree_leaf_base_t(identity(s), identity, identity)
		{}

		static string identity(string s)
		{
			return s;
		}
	};

	template<>
	class tree_leaf_t<int> : public tree_leaf_base_t<int>
	{
	public:
		tree_leaf_t(const string &s)
		: tree_leaf_base_t(stoi(s), &itos, &stoi)
		{}

		tree_leaf_t(int value)
		: tree_leaf_base_t(value, &itos, &stoi)
		{}
	};

	template<>
	class tree_leaf_t<float> : public tree_leaf_base_t<float>
	{
	public:
		tree_leaf_t(const string &s)
		: tree_leaf_base_t(stof(s), bind(&ftos, _1, 4), &stof)
		{}

		tree_leaf_t(float value)
		: tree_leaf_base_t(value, bind(&ftos, _1, 4), &stof)
		{}
	};

	template<>
	class tree_leaf_t<bool> : public tree_leaf_base_t<bool>
	{
	public:
		tree_leaf_t(const string &s)
		: tree_leaf_base_t(boolFromString(s), &boolToString, &boolFromString)
		{}

		tree_leaf_t(bool value)
		: tree_leaf_base_t(value, &boolToString, &boolFromString)
		{}

		static string boolToString(bool value)
		{
			return string(value ? "true" : "false");
		}
			
		static bool boolFromString(string s)
		{
			return s == "true";
		}
	};

	/**
	Basically, a specially modified version of boost::char_separators that
	allows quoted strings to be interpretted as single tokens
	*/
	class sexpr_delimiters
	{
	public:
		sexpr_delimiters(const string &kept_delims)
		: m_kept_delims(kept_delims)
		{}

		void reset() { }

		template <typename InputIterator, typename Token>
		bool operator()(InputIterator& next, InputIterator end, Token& tok)
		{
			typedef tokenizer_detail::
				assign_or_plus_equal<typename tokenizer_detail
					::get_iterator_category<InputIterator>
						::iterator_category> assigner;

			assigner::clear(tok);

			// skip past all dropped_delims
			for(; next != end  && is_dropped(*next); ++next) {}

			if (next == end)
				return false;

			InputIterator start(next); // save position of token start

			// if we are on a kept_delims move past it and stop
			if (is_kept(*next))
			{
				assigner::plus_equal(tok,*next);
				++next;
			}
			else if(is_double_quote(*next))// if on a quote?
			{
				string test;

				++next; // ignore the open quote
				start=next;
				
				// append all the characters until the close quote
				for (; next != end && !is_double_quote(*next); ++next)
				{
					assigner::plus_equal(tok,*next);
					test+=(*next);
				}
				VERIFY(next != end, "Quoted string not closed properly");
				InputIterator string_end(next);
				++next; // ignore the close quote

				assigner::assign(start,string_end,tok);
				return true;
			}
			else
			{
				// append all the non delim characters
				for (; next != end && !is_dropped(*next) &&
					   !is_kept(*next); ++next)
				{
					assigner::plus_equal(tok,*next);
				}
			}

			assigner::assign(start,next,tok);
			return true;
		}

	private:
		string m_kept_delims;

		bool is_kept(char E) const
		{  
			if(m_kept_delims.length())
			{
				return m_kept_delims.find(E) != string::npos;
			}
			else
			{
				return false;
			}
		}

		bool is_dropped(char E) const
		{
			return std::isspace(E) != 0;
		}

		bool is_double_quote(char E) const
		{
			return E == '\"';
		}
	};

	typedef tokenizer<sexpr_delimiters> tokenizer;
	typedef shared_ptr<const tree_node_t> tree_node_ptr;
	typedef vector<tree_node_ptr> cells_t;
	
	class tree_branch_t : public tree_node_t
	{
	public:
		typedef cells_t::const_iterator const_iterator;

		tree_branch_t(const string &s)
		{
			fromString(s);
		}

		tree_branch_t(tokenizer::iterator &i,
		              const tokenizer::iterator &end,
					  int &paren_depth)
		{
			fromTokenStream(i, end, paren_depth);
		}

		const_iterator begin() const
		{
			return cells.begin();
		}

		const_iterator end() const
		{
			return cells.end();
		}

		virtual string toString() const;

		virtual void fromString(const string &s);

	private:
		void fromTokenizer(const tokenizer &tokens)
		{
			tokenizer::iterator i = tokens.begin();
			int paren_depth = 0;
			fromTokenStream(i, tokens.end(), paren_depth);
			VERIFY(paren_depth==0, "Unbalanced parens in expression!");
		}

		void fromTokenStream(tokenizer::iterator &i,
		                     const tokenizer::iterator &end,
							 int &paren_depth);

		static shared_ptr<const tree_node_t> interpret_token(const string &s);

	protected:
		cells_t cells;
	};

	class tree_root_t : public tree_branch_t
	{
	public:
		tree_root_t(const string &s)
		: tree_branch_t(s)
		{}

		virtual string toString() const
		{
			string s;
			for(cells_t::const_iterator i=cells.begin(); i!=cells.end(); ++i)
			{
				shared_ptr<const tree_node_t> node = *i;
				s += node->toString() + " ";
			}
			return s;
		}
	};

	typedef shared_ptr<const tree_branch_t> tree_branch_ptr;
	typedef shared_ptr<const tree_root_t> tree_root_ptr;

	typedef function<tree_node_ptr (cells_t)> FunctionBinding;
	typedef map<string, FunctionBinding> FunctionMapping;

public:
	/** Default costructor */
	sexpr_t()
	{
		setupStandardBindings();
	}

	/** Build from a string containing the expression code */
	sexpr_t(const string &s);

	/**
	Build from a string containing the expression code.
	The inverse of toString
	*/
	static sexpr_t fromString(const string &s);
	
	/** Build a string from the expression. The inverse of fromString */
	static string toString(const sexpr_t &sexpr);

	/** Gets the value of a leaf node. The parameter MUST be a leaf node! */
	template<typename TYPE> static inline
	TYPE eval_leaf(tree_node_ptr leaf)
	{
		return eval_leaf(dynamic_pointer_cast<const tree_leaf_base_t<TYPE> >(leaf));
	}
	
	/** Gets the value of a leaf node */
	template<typename TYPE> static inline
	TYPE eval_leaf(shared_ptr<const tree_leaf_base_t<TYPE> > leaf)
	{
		return leaf->getValue();
	}

	/**
	Evaluates a sexpr and tries to cast the resultant value to something
	more useable by the client
	@param sexpr sexpr to evaluate
	@return value of the expression
	*/
	template<typename TYPE> static inline
	TYPE eval(const sexpr_t &sexpr)
	{
		tree_node_ptr result_node = sexpr_t::eval(sexpr);
		
		ASSERT(result_node, "Failed to evaluate expression: result is nil");

		shared_ptr<const tree_leaf_t<TYPE> > result_node_value
			= dynamic_pointer_cast<const tree_leaf_t<TYPE> >(result_node);
		
		ASSERT(result_node_value, string("Failed to evaluate expression: ") +
		"could not convert result to a value of type " + typeid(TYPE).name());
		
		TYPE v = result_node_value->getValue();
		
		return v;
	}

	/**
	Evaluates this sexpr and tries to cast the resultant value to something
	more useable by the client
	@return value of the expression
	*/
	template<typename TYPE> inline
	TYPE eval() const
	{
		return sexpr_t::eval<TYPE>(*this);
	}

	/**
	Binds a function for use within the sexpr
	@param name Name of the function
	@param fn Function to execute
	*/
	inline void addBinding(const string &name, FunctionBinding fn)
	{
		functions[name] = fn;
	}

	/**
	Tries to cast the leaf node to a value
	@param leaf Must be a leaf node
	@param result returns the value of the leaf when the cast suceeds
	@return true if the cast succeeded, false otherwise
	*/
	template<typename TYPE> static
	bool tryToCastLeaf(tree_node_ptr leaf, TYPE &result)
	{
		shared_ptr<tree_leaf_base_t<TYPE> const> p =
			dynamic_pointer_cast<tree_leaf_base_t<TYPE> const>(leaf);

		if(p)
		{
			result = p->getValue();
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	Attempts to perform an operation on two leaf nodes.  If the leaf types are
	not of the type TYPE then return false without completing the operation.
	@param op Operation to perform
	@param leaf1 Leaf node containing the first value
	@param leaf2 Leaf node containing the second value
	@param out returns the result of the operation when the cast succeeds
	*/
	template<typename A, typename B, typename C> static
	bool tryOperation(function<C (A, B)> op,
	                  tree_node_ptr leaf1,
	                  tree_node_ptr leaf2,
				      C &out)
	{
		A a;
		B b;

		if(tryToCastLeaf<A>(leaf1, a) && tryToCastLeaf<B>(leaf2, b))
		{
			out = op(a, b);
			return true;
		}
		else
		{
			return false;
		}
	}

private:
	/** Create collection of standard bindings for a sexpr */
	void setupStandardBindings();

	/** Gets the root node of the expression tree */
	inline tree_root_ptr getRoot() const
	{
		return root;
	}

	/**
	Evaluates a sexpr directly
	@return node represents the final value of the expression
	*/
	static tree_node_ptr eval(const sexpr_t &sexpr);
	
	/**
	Evaluates the root node of a sexpr
	@param node Branch to evaluate
	@param functions Maps between function names and bound functions
	@return node represents the final value of the expression
	*/
	static tree_node_ptr eval(tree_root_ptr node,
	                          const FunctionMapping &functions);
	
	/**
	Evaluates some branch expression within a sexpr
	@param node Branch to evaluate
	@param functions Maps between function names and bound functions
	@return node represents the final value of the expression
	*/
	static tree_node_ptr eval(tree_branch_ptr node,
	                          const FunctionMapping &functions);
	
	/**
	Evaluates a function called by the sexpr
	@param name Name of the function
	@param parameters List of parameters provided to the sexpr
	@param functions Maps between function names and bound functions
	@return node represents the final value of the expression
	*/
	static tree_node_ptr eval(const string &name,
	                          const cells_t &parameters,
							  const FunctionMapping &functions);

	/**
	Evaluates a function called by the sexpr
	@param beginOfParamsList Beginning of the parameters list
	@param endOfParamsList End of the parameters list
	@param functions Maps between function names and bound functions
	@return node represents the final value of the expression
	*/
	static tree_node_ptr eval(cells_t::const_iterator beginOfParamsList,
	                          cells_t::const_iterator endOfParamsList,
					          const FunctionMapping &functions);

	/** Gets the name of the function called by the branch */
	static string getBranchFunctionName(cells_t::const_iterator &i);

	/**
	Builds the parameter list, evaluating each parameter left to right
	@param i Iterator points to the current cell
	@param end Iterator indicates the end of the stream of cells
	@param functions Maps between function names and bound functions
	@return List of evaluated cells to be passed as parameters
	*/
	static cells_t buildParameters(cells_t::const_iterator &i,
	                               cells_t::const_iterator &end,
								   const FunctionMapping &functions);

	static tree_node_ptr defun_and(cells_t parameters)
	{
		for(cells_t::const_iterator i = parameters.begin();
			i != parameters.end(); ++i)
		{
			bool x = eval_leaf<bool>(*i);
			if(!x)
			{
				return tree_node_ptr(new tree_leaf_t<bool>(false));
			}
		}
		return tree_node_ptr(new tree_leaf_t<bool>(true));
	}

	static tree_node_ptr defun_or(cells_t parameters)
	{
		bool result=false;
		for(cells_t::const_iterator i = parameters.begin();
			i != parameters.end(); ++i)
		{
			bool x = eval_leaf<bool>(*i);
			result = result || x;
		}
		return tree_node_ptr(new tree_leaf_t<bool>(result));
	}

	static tree_node_ptr defun_not(cells_t parameters)
	{
		bool value = eval_leaf<bool>(*parameters.begin());
		return tree_node_ptr(new tree_leaf_t<bool>(!value));
	}

	tree_node_ptr defun_relation(cells_t parameters,
						         function<bool(int,int)> op_int,
						         function<bool(bool,bool)> op_bool,
						         function<bool(float,float)> op_float,
						         function<bool(string,string)> op_str)
	{
		bool r=false;

		cells_t::const_iterator i = parameters.begin();
		tree_node_ptr leaf1 = *i++;
		tree_node_ptr leaf2 = *i++;

		if(!tryOperation<int, int, bool>(op_int,leaf1,leaf2,r))
		{
			if(!tryOperation<bool, bool, bool>(op_bool,leaf1,leaf2,r))
			{
				if(!tryOperation<float, float, bool>(op_float,leaf1,leaf2,r))
				{
					if(!tryOperation<string, string, bool>(op_str,leaf1,leaf2,r))
					{
						FAIL("Types are not compatible!");
					}
				}
			}
		}

		return tree_node_ptr(new tree_leaf_t<bool>(r));
	}

	tree_node_ptr defun_op(cells_t parameters,
						    function<int(int,int)> op_int,
						    function<bool(bool,bool)> op_bool,
						    function<float(float,float)> op_float,
						    function<string(string,string)> op_string)
	{
		int r_int=0;
		float r_float=0.0f;
		string r_str="nil";
		bool r_bool=false;

		cells_t::const_iterator i = parameters.begin();
		tree_node_ptr leaf1 = *i++;
		tree_node_ptr leaf2 = *i++;

		if(tryOperation<int, int, int>(op_int,leaf1,leaf2,r_int))
		{
			return tree_node_ptr(new tree_leaf_t<int>(r_int));
		}

		if(tryOperation<float, float, float>(op_float,leaf1,leaf2,r_float))
		{
			return tree_node_ptr(new tree_leaf_t<float>(r_float));
		}

		if(tryOperation<string, string, string>(op_string,leaf1,leaf2,r_str))
		{
			return tree_node_ptr(new tree_leaf_t<string>(r_str));
		}

		if(tryOperation<bool, bool, bool>(op_bool,leaf1,leaf2,r_bool))
		{
			return tree_node_ptr(new tree_leaf_t<bool>(r_bool));
		}

		FAIL("Types are not compatible!");
		return tree_node_ptr();
	}

private:	
	/** Root of the expression tree */
	tree_root_ptr root;

	/** Binds function names to actual functions */
	FunctionMapping functions;
};

/** 
Allows us to bind operator type functions with a measure of
syntactic niceness. Operation result is generated as a generic tree node with
a boolean type (regardless of the parameter types)
@param sexpr sexpr to add the operator binding to
@param name String name of the operator
@param op Function to handle the operation.  Must accept a single template
          parameter which is the type of both parameters to the operation.
*/
#define addRelationBinding(sexpr, name, op) \
{                                                                           \
	function<bool(int,int)> op_int = bind(& op <int>, _1, _2);              \
	function<bool(bool,bool)> op_bool = bind(& op <bool>, _1, _2);          \
	function<bool(float,float)> op_float = bind(& op <float>, _1, _2);      \
	function<bool(string,string)> op_string = bind(& op <string>, _1, _2);  \
	sexpr.addBinding(name,                                                  \
	                 bind(&sexpr_t::defun_relation,                         \
				          &sexpr,                                           \
	                      _1,                                               \
	                      op_int,                                           \
	                      op_bool,                                          \
	                      op_float,                                         \
	                      op_string));                                      \
}

/** 
Allows us to bind operator type functions with a measure of
syntactic niceness. Operation result is generated as a generic tree node
@param sexpr sexpr to add the operator binding to
@param name String name of the operator
@param op Function to handle the operation.  Must accept a single template
          parameter which is the type of both parameters to the operation.
*/
#define addOperationBinding(sexpr, name, op) \
{                                                                           \
	function<int(int,int)> op_int = bind(& op <int>, _1, _2);               \
	function<bool(bool,bool)> op_bool = bind(& op <bool>, _1, _2);          \
	function<float(float,float)> op_float = bind(& op <float>, _1, _2);     \
	function<string(string,string)> op_string = bind(& op <string>, _1, _2);\
	sexpr.addBinding(name,                                                  \
	                 bind(&sexpr_t::defun_op,                               \
				          &sexpr,                                           \
	                      _1,                                               \
	                      op_int,                                           \
	                      op_bool,                                          \
	                      op_float,                                         \
	                      op_string));                                      \
}

#endif
