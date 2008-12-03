#include "stdafx.h"
#include "World.h"
#include "SDLinput.h"
#include "ComponentDamageOnCollision.h"

#include "EventDamageReceived.h"
#include "MessagePassWorld.h"

ComponentDamageOnCollision::
ComponentDamageOnCollision(UID uid, ScopedEventHandler *blackBoard)
: Component(uid, blackBoard),
  world(0),
  onlyPlayers(false)
{
	resetMembers();
	REGISTER_HANDLER(ComponentDamageOnCollision::handleEventCollisionOccurred);
	REGISTER_HANDLER(ComponentDamageOnCollision::handleMessagePassWorld);
}

void ComponentDamageOnCollision::load(const PropertyBag &data)
{
	resetMembers();
	onlyPlayers = data.getBool("onlyPlayers");
	damage = data.getInt("damage");
}

void ComponentDamageOnCollision::resetMembers()
{
	onlyPlayers = false;
	damage = 100;
}

void ComponentDamageOnCollision::handleMessagePassWorld( const MessagePassWorld *message )
{
	world = message->world;
}

void ComponentDamageOnCollision::handleEventCollisionOccurred( const EventCollisionOccurred *message )
{
	ASSERT(world, "Null pointer: world");
	ASSERT(message, "Null parameter: message");

	ActorID a2id = PhysicsEngine::getActorFromGeom(message->o2);

	if(!onlyPlayers || (onlyPlayers && world->isAPlayer(a2id)))
	{
		ActorID a1id = PhysicsEngine::getActorFromGeom(message->o1);
		ActorID uid = getActorID();

		if(a1id == uid)
		{
			ActorSet &s = world->getObjects();
			ActorPtr a2 = s.get(a2id);
			{EventDamageReceived m(damage); a2->recvMessage(&m);}
		}
	}
}
