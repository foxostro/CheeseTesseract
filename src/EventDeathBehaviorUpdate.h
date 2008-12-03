#ifndef EVENT_DEATH_BEHAVIOR_UPDATE_H
#define EVENT_DEATH_BEHAVIOR_UPDATE_H

#include "EventHandler.h"
#include "DeathBehavior.h"

/** Message to notify that the actor's death behavior has changed */
class EventDeathBehaviorUpdate : public Event
{
public:
	EventDeathBehaviorUpdate(DeathBehavior _deathBehavior)
	{
		deathBehavior = _deathBehavior;
	}

public:
	DeathBehavior deathBehavior;
};

#endif
