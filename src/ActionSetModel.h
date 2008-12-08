#ifndef ACTION_SET_MODEL_H
#define ACTION_SET_MODEL_H

#include "EventHandler.h"

/** Message to request that the object's model be changed */
class ActionSetModel : public Action {
public:
	ActionSetModel(const FileName& _fileName) {
		fileName = _fileName;
	}
	
public:
	FileName fileName;
};

#endif
