#ifndef TYPEINFO_H
#define TYPEINFO_H

#include <typeinfo>

class TypeInfo {
public:
	explicit TypeInfo(const type_info& info) : _typeInfo(info) { /* empty */ }
	
	bool operator<(const TypeInfo& rhs) const {
		return _typeInfo.before(rhs._typeInfo) != 0;
	}
	
private:
	/** @brief Do not call assignment operator. */
	TypeInfo operator=(const TypeInfo &rh);
	
	const type_info& _typeInfo;
};

#endif
