#include "stdafx.h"
#include "World.h"
#include "SDLinput.h"
#include "ComponentUseOnCollision.h"
#include "EventUsesObject.h"

ComponentUseOnCollision::
ComponentUseOnCollision(UID uid, ScopedEventHandler *parentScope)
: world(0),
  onlyPlayers(false),
  Component(uid, parentScope)
{
	REGISTER_HANDLER(ComponentUseOnCollision::handleEventCollisionOccurred);
	REGISTER_HANDLER(ComponentUseOnCollision::handleMessagePassWorld);
}

void ComponentUseOnCollision::load(const PropertyBag &data)
{
	resetMembers();
	onlyPlayers = data.getBool("onlyPlayers");
}

void ComponentUseOnCollision::resetMembers()
{
	onlyPlayers = false;
}

void ComponentUseOnCollision::handleMessagePassWorld( const MessagePassWorld *message )
{
	world = message->world;
}

void ComponentUseOnCollision::handleEventCollisionOccurred( const EventCollisionOccurred *event )
{
	ActorID a2id = PhysicsEngine::getActorFromGeom(event->o2);

	if(!onlyPlayers || (onlyPlayers && world->isAPlayer(a2id)))
	{
		ActorID a1id = PhysicsEngine::getActorFromGeom(event->o1);
		ActorID uid = getActorID();

		if(a1id == uid)
		{
			EventUsesObject m(uid);
			sendEvent(&m);
		}
	}
}
