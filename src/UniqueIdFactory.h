#ifndef _UNIQUE_ID_FACTORY_H_
#define _UNIQUE_ID_FACTORY_H_

/**
Assigns unique IDs of the parameterized type. IDs are only unique with respect
to other IDs assigned from this factory. The domain of the parameterized type
must also be taken into account: When UID_TYPE=char, only the first 256 IDs
are guaranteed to be unique.

Also, UID_TYPE must have the ++ operator implemented if it is not a
primitive type.
*/
template<typename UID_TYPE> class UniqueIdFactory
{
public:
	/**
	Constructor
	@param initialUid The initial unique ID
	*/
	UniqueIdFactory(UID_TYPE initialUid) : nextUid(initialUid) {}

	/**
	Returns a new UID 
	@return
	*/
	UID_TYPE getUid(void)
	{
		UID_TYPE uid = nextUid;
		nextUid++;
		return uid;
	}

private:
	UID_TYPE nextUid; /* Next UID to be assigned **/
};

#endif
