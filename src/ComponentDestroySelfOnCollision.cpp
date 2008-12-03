#include "stdafx.h"
#include "World.h"
#include "SDLinput.h"
#include "ComponentDestroySelfOnCollision.h"

#include "EventCharacterHasDied.h"
#include "EventUsesObject.h"
#include "ActionDeleteActor.h"

ComponentDestroySelfOnCollision::
ComponentDestroySelfOnCollision(UID uid, ScopedEventHandler *blackBoard)
: Component(uid, blackBoard),
  world(0),
  shouldActivate(false),
  onlyPlayers(false)
{
	REGISTER_HANDLER(ComponentDestroySelfOnCollision::handleEventCollisionOccurred);
	REGISTER_HANDLER(ComponentDestroySelfOnCollision::handleMessagePassWorld);
}

void ComponentDestroySelfOnCollision::load(const PropertyBag &data)
{
	resetMembers();
	data.get("onlyPlayers", onlyPlayers); // optional tag
}

void ComponentDestroySelfOnCollision::resetMembers()
{
	shouldActivate = false;
	onlyPlayers = false;
}

void ComponentDestroySelfOnCollision::handleMessagePassWorld(const MessagePassWorld *message )
{
	world = message->world;
}

void ComponentDestroySelfOnCollision::handleEventCollisionOccurred( const EventCollisionOccurred *message )
{
	if(!onlyPlayers)
	{
		shouldActivate=true;
		return;
	}

	ActorID a2id = PhysicsEngine::getActorFromGeom(message->o2);

	if(world->isAPlayer(a2id))
	{
		ActorID a1id = PhysicsEngine::getActorFromGeom(message->o1);
		ActorID uid = getActorID();

		if(a1id == uid)
		{
			shouldActivate=true;
		}
	}
}

void ComponentDestroySelfOnCollision::update(float)
{
	if(shouldActivate)
	{
		{broadcastEventCharacterHasDied();
		}

		broadcastDeleteSelfActor();
	}
}

void ComponentDestroySelfOnCollision::broadcastDeleteSelfActor()
{
	ActionDeleteActor m(getActorID());
	sendGlobalAction(&m);
}

void ComponentDestroySelfOnCollision::broadcastEventCharacterHasDied()
{
	EventCharacterHasDied m;
	sendEvent(&m);
}
