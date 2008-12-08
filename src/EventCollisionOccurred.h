#ifndef EVENT_COLLISION_OCCURRED_H
#define EVENT_COLLISION_OCCURRED_H

#include "EventHandler.h"

/** Message indicates collision between two physics objects */
class EventCollisionOccurred : public Event {
public:
	EventCollisionOccurred(dGeomID _o1, dGeomID _o2, dContact _contact) {
		o1 = _o1;
		o2 = _o2;
		contact = _contact;
	}
	
public:
	dGeomID o1;
	dGeomID o2;
	dContact contact;
};

#endif
