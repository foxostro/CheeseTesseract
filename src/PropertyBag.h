/*
Modified by Andrew Fox in 2003-2008
E-Mail: mailto:andrewfox@cmu.edu

Original Source:
McCuskey, Mason. "Game Programming Tricks of the Trade".
	"Trick 15: Serialization Using XML Property Bags".
	Premier Press. 2002.
*/

#ifndef _PROPERTY_BAG_H_
#define _PROPERTY_BAG_H_

#include "ivec2.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "color.h"
#include "ListBag.h"

/** Item stored in a property bag */
class PropertyBagItem {
public:
	/** Destructor */
	virtual ~PropertyBagItem();
	
	/** Constructor */
	PropertyBagItem();
	
	/** Copy Constructor */
	PropertyBagItem(const PropertyBagItem&) {}
	
	/**
	Return a string representation of some XML structure that in turn
	represents this item.
	@param indentlevel Indentation level of the resultant XML code
	*/
	virtual string save(int indentlevel=0) const = 0;
	
	/**
	Equality operator
	@param r Item to test
	@return true when the objects are equal
	*/
	virtual bool operator==(const PropertyBagItem &r) const = 0;
	
	/**
	Inequality operator
	@param r Item to test
	@return true when the objects are not equal
	*/
	bool operator!=(const PropertyBagItem &r);
};

/** String stored in a property bag */
class PropertyBagString : public PropertyBagItem {
public:
	/** Destructor */
	virtual ~PropertyBagString();
	
	/**
	Constructor
	@param data The string data to accept
	@param convert If true, the string should be made safe for an XML file
	*/
	PropertyBagString(const string &data, bool convert = true);
	
	/**
	Constructor
	@param name The name of the item
	@param data The string data to accept
	@param convert If true, the string should be made safe for an XML file
	*/
	PropertyBagString(const string &name, const string &data, bool convert = true);
	
	/**
	Return a string representation of some XML structure that in turn
	represents this item.
	@param indentlevel Indentation level of the resultant XML code
	*/
	virtual string save(int indentlevel=0) const;
	
	/**
	Equality operator
	@param r The item with which to compare
	@return true if the items are equal
	*/
	virtual bool operator==(const PropertyBagItem &r) const;
	
	/**
	Gets the data from the item, converted as appropriate.
	@return data from this item
	*/
	string getData() const;
	
	/**
	Sets data for the item
	@param data The data to accept
	@param convert If true, then it is necessary to convert the string to
	a form safe for XML.
	@return The actual data stored for this item
	*/
	const string& setData(const string &data, bool convert);
	
	/**
	Sets the name of the item
	@param name The new name of the item
	@return The name actually set for the item
	*/
	const string& setName(const string &name);
	
	/** Gets the name of the item */
	const string& getName() const;
	
	/**
	Makes a string safe for insertion into an XML file by replacing special
	characters like '>' and '&' with the appropriate special entity.
	@param str String parameter
	@return Safe string
	*/
	static string makeStringSafe(const string &str);
	
	/**
	Takes a string made safe for an XML file and replaces the entities like
	"&amp;" and "&quot;" with the appropriate characters.
	@param str String parameter
	@return Original string
	*/
	static string restoreFromSafeString(const string &str);
	
private:
	/** Has the item been converted to a safe form? */
	bool itemHasBeenConverted;
	
	/** Name of the item */
	string itemName;
	
	/** Data stored for this item*/
	string itemData;
};

/** Contains property bag items */
class PropertyBag : public PropertyBagItem {
private:
	/** string -> PropertyBagItem */
	typedef multimap<string, PropertyBagItem*> PropertyMap;
	
	/** Property bag content */
	PropertyMap data;
	
public:
	static PropertyBag fromFile(const FileName &fileName);
	
	/** Destructor */
	virtual ~PropertyBag();
	
	/** Constructor */
	PropertyBag();
	
	/** Copy constructor */
	PropertyBag(const PropertyBag &r);
	
	/**
	Construct from a string
	@param s String from which to accept data
	*/
	PropertyBag(const string &s);
	
	/** Clear out the property bag */
	void clear();
	
	/** Assignment operator */
	PropertyBag &operator=(const PropertyBag &r);
	
	/** Equality operator against CPropBag */
	virtual bool operator==(const PropertyBag &r) const;
	
	/**
	Equality operator
	@param r Item to test
	@return true when the objects are equal
	*/
	virtual bool operator==(const PropertyBagItem &r) const;
	
	/**
	Return a string representation of some XML structure that in turn
	represents this item.
	@param indentlevel Indentation level of the resultant XML code
	*/
	virtual string save(int indentlevel=0) const;
	
	/**
	Saves to file some XML structure that in turn represents this item.
	@param fileName Name of the file to write
	@param indentlevel Indentation level of the resultant XML code
	*/
	void saveToFile(const FileName &fileName, int indentLevel=0) const;
	
	/**
	Loads the contents of the property bag from file.  It is possible to
	merge the contents of the file with data that is already loaded.
	Otherwise, the currently loaded data is completely discarded and
	replaced by data loaded from file.
	@param fileName Name of the file from which to load
	@param merge If true, then the contents of the file are merged with any
	existing contents of this bag.
	@return true if success
	*/
	bool loadFromFile(const FileName &fileName, bool merge = false);
	
	/**
	Merges the contents of another PropertyBag with this one
	@param newStuff Contains additional data
	@param overwrite If true, then conflicts are resolved by overwriting
	the existing elements.
	*/
	void merge(const PropertyBag &newStuff, bool overwrite = true);
	
	/**
	Remove all instances of the item
	@param key Name of the key to remove
	*/
	void remove(const string &key);
	
	/**
	Remove all instances of the item
	@param key Name of the key to remove
	@param instance Index of the instance to remove
	*/
	void remove(const string &key, int instance);
	
	/**
	Gets the number of instances of data elements paired with the given key
	@param key Key value
	@return The number of times the key is present in the data
	*/
	size_t getNumInstances(const string &key) const;
	
	/** Adds a C string */
	void add(const string& key, const char* data, bool convert = true);
	
	/** Adds a string */
	void add(const string &key, const string &data, bool convert = true);
	
	/** Adds a FileName */
	void add(const string &key, const FileName &data);
	
	/** Adds an int */
	void add(const string &key, int data);
	
	/** Adds a size_t */
	void add(const string &key, size_t data);
	
	/** Adds a double */
	void add(const string &key, double data);
	
	/** Adds a float */
	void add(const string &key, float data);
	
	/** Adds a bool */
	void add(const string &key, bool data);
	
	/** Adds a PropertyBag */
	void add(const string &key, const PropertyBag &contents);
	
	/** Adds an ivec2 */
	void add(const string &key, const ivec2 &vec);
	
	/** Adds a vec2 */
	void add(const string &key, const vec2 &vec);
	
	/** Adds a vec3 */
	void add(const string &key, const vec3 &vec);
	
	/** Adds a vec4 */
	void add(const string &key, const vec4 &vec);
	
	/** Gets a string */
	bool get(const string &key, string &dest, size_t instance = 0) const;
	
	/** Gets a FileName */
	bool get(const string &key, FileName &dest, size_t instance = 0) const;
	
	/** Gets an int */
	bool get(const string &key, int &dest, size_t instance = 0) const;
	
	/** Gets a size_t */
	bool get(const string &key, size_t &dest, size_t instance = 0) const;
	
	/** Gets a double */
	bool get(const string &key, double &dest, size_t instance = 0) const;
	
	/** Gets a float */
	bool get(const string &key, float &dest, size_t instance = 0) const;
	
	/** Gets a bool */
	bool get(const string &key, bool &dest, size_t instance = 0) const;
	
	/** Gets a PropertyBag */
	bool get(const string &key, PropertyBag &dest, size_t instance = 0) const;
	
	/** Gets an ivec2 */
	bool get(const string &key, ivec2 &vec, size_t instance = 0) const;
	
	/** Gets a vec2 */
	bool get(const string &key, vec2 &vec, size_t instance = 0) const;
	
	/** Gets a vec3 */
	bool get(const string &key, vec3 &vec, size_t instance = 0) const;
	
	/** Gets a vec4 */
	bool get(const string &key, vec4 &vec, size_t instance = 0) const;
	
	/** Gets a color */
	bool get(const string &key, color &c, size_t instance = 0) const;
	
	/** Gets a list bag */
	template<typename ElementType>
	bool get(const string &key,
	         ListBag<ElementType> &l,
	         size_t instance = 0) const {
		PropertyBag data;
		
		if (!get(key, data, instance)) {
			return false;
		}
		
		const string elementName = "element";
		size_t numOfElements = data.getNumInstances(elementName);
		
		l.clear();
		l.resize(numOfElements);
		
		for (size_t i=0; i<numOfElements; ++i) {
			ElementType element;
			if (!data.get(elementName, element, i)) {
				ERR("Failed to get element #" + sizet_to_string(i));
			} else {
				l[i] = element;
			}
		}
		
		return true;
	}
	
	/** Gets a string */
	string getString(const string &key, size_t instance = 0) const;
	
	/** Gets a file name */
	FileName getFileName(const string &key, size_t instance = 0) const;
	
	/** Gets an int */
	int getInt(const string &key, size_t instance = 0) const;
	
	/** Gets a size_t */
	size_t getSizeT(const string &key, size_t instance = 0) const;
	
	/** Gets a double */
	double getDouble(const string &key, size_t instance = 0) const;
	
	/** Gets a float */
	float getFloat(const string &key, size_t instance = 0) const;
	
	/** Gets a bool */
	bool getBool(const string &key, size_t instance = 0) const;
	
	/** Gets a PropertyBag */
	PropertyBag getBag(const string &key, size_t instance = 0) const;
	
	/** Gets a vec2 */
	vec2 getVec2(const string &key, size_t instance = 0) const;
	
	/** Gets a vec3 */
	vec3 getVec3(const string &key, size_t instance = 0) const;
	
	/** Gets a vec4 */
	vec4 getVec4(const string &key, size_t instance = 0) const;
	
	/** Gets a color */
	color getColor(const string &key, size_t instance = 0) const;
	
	/** Gets a list bag */
	template<typename ElementType>
	ListBag<ElementType> getListBag(const string &key,
	                                size_t instance = 0) const {
		ListBag<ElementType> x;
		VERIFY(get(key, x, instance),
		       "Failed to get list from PropertyBag: " + key +
		       " (instance #" + sizet_to_string(instance) + ")");
		return x;
	}
	
private:
	/**
	Inserts a tag into the property bag
	@param tagName Name of the tag
	@param tagContent Content if the tag
	*/
	void insertTag(const string &tagName, const string &tagContent);
	
	/**
	Interprets the contents of the string as property bag contents and
	merges with the existing contents.
	@param newStuff String containing additional data
	@return true if successful, false otherwise
	*/
	bool loadMergeFromString(const string &newStuff);
	
	/**
	Copies the contents of this bag from another
	@param r Bag to copy
	*/
	void copy(const PropertyBag &r);
};

#endif
