#ifndef MESSAGE_EXIT_MAP_H
#define MESSAGE_EXIT_MAP_H

#include "EventHandler.h"

/** Message to request that the map be exited and the next map be loaded */
class ActionChangeMap : public Action {
public:
	ActionChangeMap(const FileName &_nextMap) {
		nextMap = _nextMap;
	}
	
public:
	FileName nextMap;
};

#endif
