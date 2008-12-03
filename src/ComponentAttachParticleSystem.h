#ifndef COMPONENT_ATTACH_PARRTICLE_SYSTEM_H
#define COMPONENT_ATTACH_PARRTICLE_SYSTEM_H

#include "PropertyBag.h"
#include "World.h"
#include "Component.h"

#include "EventOrientationUpdate.h"
#include "EventPositionUpdate.h"
#include "MessagePassWorld.h"

#include "ActionDeleteActor.h"

/** Attaches a particle system to the object's position */
class ComponentAttachParticleSystem : public Component
{
public:
	virtual string getTypeString() const { return "AttachParticleSystem"; }

	ComponentAttachParticleSystem(UID uid, ScopedEventHandler *blackBoard);

	/** Resets all object members to defaults */
	virtual void resetMembers();

	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);

	/** Updates the object */
	virtual void update(float milliseconds);

private:
	void handleEventOrientationUpdate(const EventOrientationUpdate *message);
	void handleEventPositionUpdate(const EventPositionUpdate *message);
	void handleMessagePassWorld(const MessagePassWorld *message);
	void handleActionDeleteActor(const ActionDeleteActor *message);

private:
	vec3 lastReportedPosition;
	mat3 lastReportedOrientation;
	World *world;
	ParticleEngine::handle handle;
	bool active;
};

#endif
