#ifndef COMPONENT_HEALTH_H
#define COMPONENT_HEALTH_H

#include "PropertyBag.h"
#include "Component.h"
#include "DeathBehavior.h"

#include "MessagePassWorld.h"

#include "EventCharacterRevived.h"
#include "EventCharacterHasDied.h"
#include "EventDamageReceived.h"
#include "EventHealingReceived.h"
#include "EventPositionUpdate.h"
#include "EventExplosionOccurred.h"
#include "EventHeightUpdate.h"
#include "EventDeathBehaviorUpdate.h"

/**
The component that holds the value of the character's health.
Tracks messages that announce damage to the character and healing applied to
the character so that it can announce whenever he has died. Very often, other
components will want to poll this one directly to determine the direct health
value and the maximum ealth value.
*/
class ComponentHealth : public Component
{
public:
	virtual string getTypeString() const { return "Health"; }

	ComponentHealth(UID uid, ScopedEventHandler *blackBoard);

	/** Resets all object members to defaults */
	virtual void resetMembers();

	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);

	/** Updates the object */
	virtual void update(float milliseconds);

	/** Gets the health value */
	int getHealth() const
	{
		return health;
	}

	/** Gets the maximum health */
	int getMaxHealth() const
	{
		return maxHealth;
	}
	
	/** Is the character dead? */
	inline bool isDead() const
	{
		return dead;
	}

private:
	void handleEventCharacterRevived(const EventCharacterRevived *event);
	void handleEventCharacterHasDied(const EventCharacterHasDied *event);
	void handleEventDamageReceived(const EventDamageReceived *event);
	void handleEventHealingReceived(const EventHealingReceived *event);
	void handleEventPositionUpdate(const EventPositionUpdate *event);
	void handleEventHeightUpdate(const EventHeightUpdate *event);
	void handleEventExplosionOccurred(const EventExplosionOccurred *event);
	void handleMessagePassWorld(const MessagePassWorld *message);
	void handleEventDeathBehaviorUpdate(const EventDeathBehaviorUpdate *event);

	void loseHealth(int damage);

	/** Gets the death behavior of the character */
	DeathBehavior getDeathBehavior() const;

	void drawHealthBar(const mat3 &cameraOrientation) const;

	void drawResurrectCountDown(const mat3 &cameraOrientation) const;

	void broadcastDamageReceived(int damage);

private:
	World *world;
	int health;
	int maxHealth;
	bool dead;
	vec3 lastReportedPosition;
	float lastReportedHeight;
	bool willResurrectAfterCountDown;
	float timeSpentDead;
	float timeUntilResurrection;
	DeathBehavior lastReportedDeathBehavior;
};

#endif
