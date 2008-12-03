#include "stdafx.h"
#include "World.h"
#include "ActionDeleteActor.h"
#include "ComponentExplodeAfterTimeout.h"

ComponentExplodeAfterTimeout::
ComponentExplodeAfterTimeout(UID uid, ScopedEventHandler *blackBoard)
: Component(uid, blackBoard)
{
	REGISTER_HANDLER(ComponentExplodeAfterTimeout::handleEventOrientationUpdate);
	REGISTER_HANDLER(ComponentExplodeAfterTimeout::handleEventPositionUpdate);
	REGISTER_HANDLER(ComponentExplodeAfterTimeout::handleMessagePassWorld);
}

void ComponentExplodeAfterTimeout::resetMembers()
{
	timeleft = 0.0f;
	baseDamage = 0;
	soundFileName = FileName("(nil)");
	particlesFileName = FileName("(nil)");
}

void ComponentExplodeAfterTimeout::load(const PropertyBag &data)
{
	resetMembers();

	timeleft = data.getFloat("timeout");
	baseDamage = data.getInt("baseDamage");
	soundFileName = data.getFileName("soundFileName");
	particlesFileName = data.getFileName("particlesFileName");
}

void ComponentExplodeAfterTimeout::handleMessagePassWorld( const MessagePassWorld *message )
{
	world = message->world;
}

void ComponentExplodeAfterTimeout::handleEventOrientationUpdate( const EventOrientationUpdate *event )
{
	lastReportedOrientation = event->orientation;
}

void ComponentExplodeAfterTimeout::handleEventPositionUpdate( const EventPositionUpdate *event )
{
	lastReportedPosition = event->position;
}

void ComponentExplodeAfterTimeout::explode()
{
	ASSERT(world, "world has not yet been set");

	const vec2 x = lastReportedOrientation.getAxisX().xy();
	const float rotation = getAngle(x, vec2(1,0));
	ActorID actor = getActorID();

	world->generateExplosion(actor,
	                         lastReportedPosition,
	                         rotation,
	                         baseDamage,
	                         soundFileName,
	                         particlesFileName);

	broadcastDeleteActor();

}

void ComponentExplodeAfterTimeout::update( float milliseconds )
{
	if((timeleft -= milliseconds) <= 0.0f)
	{
		explode();
	}
}

void ComponentExplodeAfterTimeout::broadcastDeleteActor()
{
	ActionDeleteActor m(getActorID());
	sendGlobalAction(&m);
}
