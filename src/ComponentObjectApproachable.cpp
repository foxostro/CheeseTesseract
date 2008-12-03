#include "stdafx.h"
#include "World.h"
#include "ComponentPhysics.h"
#include "ComponentObjectApproachable.h"

#include "EventApproachActor.h"
#include "EventRecedesFromActor.h"

ComponentObjectApproachable::ComponentObjectApproachable( UID uid, ScopedEventHandler *parentScope )
: Component(uid, parentScope),
  world(0),
  thresholdTrigger(0.0f),
  thresholdRelease(0.0f)
{
	REGISTER_HANDLER(ComponentObjectApproachable::handleEventPositionUpdate);
	REGISTER_HANDLER(ComponentObjectApproachable::handleMessagePassWorld);

	resetMembers();
}

void ComponentObjectApproachable::resetMembers()
{
	thresholdTrigger = 1.0f;
	thresholdRelease = 2.0f;

	playerState.clear();
}

void ComponentObjectApproachable::load( const PropertyBag &data )
{
	resetMembers();
	thresholdTrigger = data.getFloat("thresholdTrigger");
	thresholdRelease = data.getFloat("thresholdRelease");
}

void ComponentObjectApproachable::update(float)
{
	ASSERT(world, "World has not been set yet");

	const ActorSet &players = world->players;

	for(ActorSet::const_iterator i=players.begin(); i!=players.end(); ++i)
	{
		ActorPtr player = i->second;

		if(player->hasComponent("Physics"))
		{
			shared_ptr<Component> component;
			shared_ptr<ComponentPhysics> physics;
			
			component = player->getComponent("Physics");
			physics = dynamic_pointer_cast<ComponentPhysics>(component);

			if(physics)
			{
				const float distance = vec3((physics->getPosition())-lastReportedPosition).getMagnitude();
				const ActorID id = player->getUID();

				if(distance <= thresholdTrigger)
				{
					playerApproaches(id);
				}
				else if(distance > thresholdRelease)
				{
					playerRecedes(id);
				}
			}
		}
	}
}

void ComponentObjectApproachable::handleEventPositionUpdate( const EventPositionUpdate *event )
{
	lastReportedPosition = event->position;
}

void ComponentObjectApproachable::handleMessagePassWorld( const MessagePassWorld *message )
{
	world = message->world;
}

void ComponentObjectApproachable::playerRecedes(ActorID id)
{
	if(playerState[id] == OBJECT_NEAR)
	{
		playerState[id] = OBJECT_FAR;
		EventRecedesFromActor m(id);
		sendEvent(&m);
	}
}

void ComponentObjectApproachable::playerApproaches(ActorID id)
{
	if(playerState[id] == OBJECT_FAR)
	{
		playerState[id] = OBJECT_NEAR;
		EventApproachActor m(id);
		sendEvent(&m);
	}
}
