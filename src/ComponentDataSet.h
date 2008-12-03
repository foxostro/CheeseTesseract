#ifndef COMPONENT_DATA_SET_H
#define COMPONENT_DATA_SET_H

#include "PropertyBag.h"

/**
Structure used to create a particular actor instance.
Lists the components used by the actor and the data that is to be parsed
and loaded by each component.
*/
class ComponentDataSet
{
public:
	typedef vector<pair<string, PropertyBag> >::const_iterator const_iterator;
	typedef vector<pair<string, PropertyBag> >::iterator iterator;

private:
	vector<pair<string, PropertyBag> > data;

public:
	/** Default Constructor */
	ComponentDataSet() {}

	/** Copy Constructor */
	ComponentDataSet(const ComponentDataSet &s) : data(s.data) {}

	const_iterator begin() const
	{
		return data.begin();
	}

	const_iterator end() const
	{
		return data.end();
	}

	iterator begin()
	{
		return data.begin();
	}

	iterator end()
	{
		return data.end();
	}

	/**
	Finds data for the component, given a particular component name
	@param name Component name
	@return access to the specified component data
	*/
	PropertyBag& get(const string &name)
	{
		for(ComponentDataSet::iterator i=begin(); i!=end(); ++i)
		{
			if(i->first == name)
			{
				return i->second;
			}
		}

		FAIL("Failed to find component: " + name);
		return end()->second;
	}

	/** Parses out component data from an object template */
	static ComponentDataSet load(const PropertyBag &base,
		const PropertyBag &specialization)
	{
		return merge(parse(base), parse(specialization));
	}

	/** Parses out component data from an object template */
	static ComponentDataSet parse(const PropertyBag &data)
	{
		ComponentDataSet set;

		for(size_t i=0, n=data.getNumInstances("component"); i<n; ++i)
		{
			const PropertyBag component = data.getBag("component", i);
			const string name = component.getString("name");
			set.add(name, component);
		}

		return set;
	}

	/** Parses out component data from an object template */
	static PropertyBag merge(const PropertyBag &baseData,
		const ComponentDataSet &specializaions,
		const string &name)
	{
		for(ComponentDataSet::const_iterator i=specializaions.begin();
			i!=specializaions.end(); ++i)
		{
			if(i->first == name)
			{
				PropertyBag mergedData = baseData;
				mergedData.merge(i->second, true);
				return mergedData;
			}
		}

		return baseData;
	}

	/**
	Merges two component data sets into a single set
	@param base Base data set
	@param sp Specializations are extensions and modifications to
	the base data set.  Components that are specified
	here, and are not also specified in the base, are
	ignored.
	@return Merged data set
	*/
	static ComponentDataSet merge(const ComponentDataSet &base,
		const ComponentDataSet &sp)
	{
		ComponentDataSet mergedSet;

		for(ComponentDataSet::const_iterator i=base.begin();
			i!=base.end();++i)
		{
			const string &name = i->first;
			const PropertyBag &baseData = i->second;
			mergedSet.add(name, merge(baseData, sp, name));
		}

		return mergedSet;
	}

private:
	void add(const string &name, const PropertyBag &componentData)
	{
		data.push_back(make_pair(name, componentData));
	}
};

#endif
