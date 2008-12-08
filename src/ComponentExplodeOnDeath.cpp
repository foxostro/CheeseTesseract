#include "stdafx.h"
#include "World.h"
#include "ComponentExplodeOnDeath.h"

ComponentExplodeOnDeath::
ComponentExplodeOnDeath(UID uid, ScopedEventHandler *blackBoard)
		: Component(uid, blackBoard) {
	REGISTER_HANDLER(ComponentExplodeOnDeath::handleEventCharacterHasDied);
	REGISTER_HANDLER(ComponentExplodeOnDeath::handleEventOrientationUpdate);
	REGISTER_HANDLER(ComponentExplodeOnDeath::handleEventPositionUpdate);
	REGISTER_HANDLER(ComponentExplodeOnDeath::handleMessagePassWorld);
}

void ComponentExplodeOnDeath::resetMembers() {
	baseDamage = 0;
	soundFileName = FileName("(nil)");
	particlesFileName = FileName("(nil)");
}

void ComponentExplodeOnDeath::load(const PropertyBag &data) {
	resetMembers();
	
	baseDamage = data.getInt("baseDamage");
	soundFileName = data.getFileName("soundFileName");
	particlesFileName = data.getFileName("particlesFileName");
}

void ComponentExplodeOnDeath::handleMessagePassWorld( const MessagePassWorld *event ) {
	world = event->world;
}

void ComponentExplodeOnDeath::handleEventCharacterHasDied(const EventCharacterHasDied *) {
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
}

void ComponentExplodeOnDeath::handleEventOrientationUpdate(const EventOrientationUpdate *event) {
	lastReportedOrientation = event->orientation;
}

void ComponentExplodeOnDeath::handleEventPositionUpdate(const EventPositionUpdate *event) {
	lastReportedPosition = event->position;
}
