#include "Core.h"
#include "sexpr.h"

template<typename T> static 
bool op_eq(T a, T b)
{
	return a==b;
}

template<> static 
bool op_eq<float>(float a, float b)
{
	const float eps = 0.001f;
	return (a-b)<eps;
}

template<typename T> static 
bool op_greater(T a, T b)
{
	return a>b;
}

template<> static 
bool op_greater<bool>(bool, bool)
{
	FAIL("bool > bool is not a valid operation");
	return false;
}

template<typename T> static 
bool op_less(T a, T b)
{
	return a<b;
}

template<> static 
bool op_less<bool>(bool, bool)
{
	FAIL("bool < bool is not a valid operation");
	return false;
}

template<typename T> static 
T op_addition(T a, T b)
{
	return a+b;
}

template<> static 
bool op_addition<bool>(bool, bool)
{
	FAIL("bool + bool is not a valid operation");
	return false;
}

template<typename T> static 
T op_subtraction(T a, T b)
{
	return a-b;
}

template<> static 
string op_subtraction<string>(string, string)
{
	FAIL("string - string is not a valid operation");
	return "nil";
}

template<> static 
bool op_subtraction<bool>(bool, bool)
{
	FAIL("bool - bool is not a valid operation");
	return false;
}

template<typename T> static 
T op_multiplication(T a, T b)
{
	return a*b;
}

template<> static 
string op_multiplication<string>(string a, string b)
{
	FAIL("string * string is not a valid operation");
	return "nil";
}

template<> static 
bool op_multiplication<bool>(bool, bool)
{
	FAIL("bool * bool is not a valid operation");
	return false;
}

template<typename T> static 
T op_division(T a, T b)
{
	return a/b;
}

template<> static 
string op_division<string>(string a, string b)
{
	FAIL("string / string is not a valid operation");
	return "nil";
}

template<> static 
bool op_division<bool>(bool, bool)
{
	FAIL("bool / bool is not a valid operation");
	return false;
}

template<typename T> static 
T op_modulus(T, T)
{
	FAIL("modulus of these types is not a valid operation");
	return 0;
}

template<> static 
int op_modulus<int>(int a, int b)
{
	return a%b;
}

template<> static 
float op_modulus<float>(float a, float b)
{
	return fmodf(a,b);
}

void sexpr_t::setupStandardBindings()
{
	addBinding("not", defun_not);
	addBinding("and", defun_and);
	addBinding("or",  defun_or);

	addRelationBinding((*this), "eq",      op_eq);
	addRelationBinding((*this), "greater", op_greater);
	addRelationBinding((*this), "less",    op_less);

	addOperationBinding((*this), "add", op_addition);
	addOperationBinding((*this), "sub", op_subtraction);
	addOperationBinding((*this), "mul", op_multiplication);
	addOperationBinding((*this), "div", op_division);
	addOperationBinding((*this), "mod", op_modulus);
}

//////////////////////////////////////////////////////////////////////////////

sexpr_t sexpr_t::fromString(const string &s)
{
	return sexpr_t(s);
}

string sexpr_t::toString(const sexpr_t &sexpr)
{
	return sexpr.getRoot()->toString();
}

sexpr_t::tree_node_ptr sexpr_t::eval(const string &name,
									 const cells_t &parameters,
									 const FunctionMapping &functions)
{
	FunctionMapping::const_iterator i = functions.find(name);
	ASSERT(i!=functions.end(), "Function not recognized: " + name);
	return (i->second)(parameters);
}

sexpr_t::tree_node_ptr sexpr_t::eval(tree_branch_ptr node,
									 const FunctionMapping &functions)
{
	ASSERT(node, "Parameter \"node\" was null!");
	return eval(node->begin(), node->end(), functions);
}

sexpr_t::tree_node_ptr sexpr_t::eval(tree_root_ptr _node,
									 const FunctionMapping &functions)
{
	cells_t::const_iterator i = _node->begin();

	tree_node_ptr node = *i;

	if(tree_branch_ptr branch = dynamic_pointer_cast<const tree_branch_t>(node))
	{
		return eval(branch, functions);
	}
	else
	{
		return node;
	}
}

sexpr_t::tree_node_ptr sexpr_t::eval(const sexpr_t &sexpr)
{
	return sexpr_t::eval(sexpr.getRoot(), sexpr.functions);
}

sexpr_t::tree_node_ptr sexpr_t::eval( cells_t::const_iterator beginOfParamsList, cells_t::const_iterator endOfParamsList, const FunctionMapping & functions )
{
	// Call the function and return the result
	string name = getBranchFunctionName(beginOfParamsList);
	cells_t parameters = buildParameters(beginOfParamsList, endOfParamsList, functions);
	return eval(name, parameters, functions);
}

string sexpr_t::getBranchFunctionName(cells_t::const_iterator &i)
{
	tree_node_ptr a = *i++;
	shared_ptr<const tree_leaf_t<string> > b
		= dynamic_pointer_cast<const tree_leaf_t<string> >(a);
	VERIFY(b, "Tried to get function name, but it doesn't look like the first cell of this expression is string");
	return b->getValue();
}

sexpr_t::cells_t sexpr_t::buildParameters(cells_t::const_iterator &i,
								          cells_t::const_iterator &end,
										  const FunctionMapping &functions)
{
	cells_t parameters;
	while(i != end)
	{
		tree_node_ptr node = *i++;

		if(tree_branch_ptr p=dynamic_pointer_cast<const tree_branch_t>(node))
		{
			// Only pass parameter expressions after they have been evaluated
			parameters.push_back(eval(p, functions));
		}
		else
		{
			parameters.push_back(node);
		}
	}
	return parameters;
}

sexpr_t::sexpr_t(const string &s)
{
	setupStandardBindings();
	root = tree_root_ptr(new tree_root_t(s));
}

void sexpr_t::tree_branch_t::fromTokenStream(tokenizer::iterator &i,
											 const tokenizer::iterator &end,
											 int &depth)
{
	VERIFY(depth>=0, "Unbalanced parens in expression!");

	cells.clear();

	while(i!=end)
	{
		string token = *i;

		if(token == "(") // Begin a new, inner sexpr
		{
			++depth;
			++i; // discard the paren
			VERIFY(i!=end, "Unbalanced parens in expression!");
			cells.push_back(tree_node_ptr(new tree_branch_t(i, end, depth)));

			if(i==end)
				return;
		}
		else if(token == ")") // End the current sexpr
		{
			--depth;
			return;
		}
		else
		{
			cells.push_back(interpret_token(token));
		}
		
		++i; // on to the next token, or on to the end
	}
}

sexpr_t::tree_node_ptr sexpr_t::tree_branch_t::interpret_token(const string &s)
{
	ASSERT(!s.empty(), "Cannot interpret empty string");

	char c = s.at(0);

	tree_node_t *node = 0;

	if(!isdigit(c) && c!='-')
	{
		if(s == "true" || s == "false")
		{
			node = new tree_leaf_t<bool>(s);
		}
		else
		{
			node = new tree_leaf_t<string>(s);
		}
	}
	else
	{
		for(string::const_iterator i=s.begin(); i!=s.end(); ++i)
		{
			char c = *i;
			if(c=='.')
			{
				node = new tree_leaf_t<float>(s);
			}
		}
		if(!node)
		{
			node = new tree_leaf_t<int>(s);
		}
	}

	return tree_node_ptr(node);
}

string sexpr_t::tree_branch_t::toString() const
{
	string s = "( ";
	for(cells_t::const_iterator i=cells.begin(); i!=cells.end(); ++i)
	{
		shared_ptr<const tree_node_t> node = *i;
		s += node->toString() + " ";
	}
	s += " )";
	return s;
}

void sexpr_t::tree_branch_t::fromString(const string &s)
{
	sexpr_delimiters delimiters("()");
	tokenizer tokens(s, delimiters);
	fromTokenizer(tokens);
}
