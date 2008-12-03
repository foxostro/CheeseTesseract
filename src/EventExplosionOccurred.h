#ifndef EVENT_EXPLOSION_OCCURRED_H
#define EVENT_EXPLOSION_OCCURRED_H

#include "EventHandler.h"

/** Notify that an explosion occurred somewhere nearby */
class EventExplosionOccurred : public Event
{
public:
	EventExplosionOccurred(const vec3 &_pos, int _baseDamage, ActorID _actor)
	{
		position = _pos;
		baseDamage = _baseDamage;
		actor = _actor;
	}

public:
	vec3 position;
	int baseDamage;
	ActorID actor;
};

#endif
