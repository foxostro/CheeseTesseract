#include "stdafx.h"
#include "World.h"
#include "ComponentPhysics.h"
#include "ComponentDropsLoot.h"

ComponentDropsLoot::ComponentDropsLoot(UID uid,
                                       ScopedEventHandler *blackBoard)
		: Component(uid, blackBoard) {
	REGISTER_HANDLER(ComponentDropsLoot::handleEventCharacterHasDied);
	REGISTER_HANDLER(ComponentDropsLoot::handleMessagePassWorld);
	REGISTER_HANDLER(ComponentDropsLoot::handleEventPositionUpdate);
	REGISTER_HANDLER(ComponentDropsLoot::handleEventRadiusUpdate);
}

void ComponentDropsLoot::resetMembers() {
	lootFileNames.clear();
	maxLoots = 1;
	lastReportedPosition.zero();
	lastReportedRadius = 1.0f;
}

void ComponentDropsLoot::load(const PropertyBag &data) {
	resetMembers();
	lootFileNames = data.getListBag<FileName>("lootFileNames");
	maxLoots = data.getInt("maxLoots");
}

void ComponentDropsLoot::handleEventCharacterHasDied(const EventCharacterHasDied *) {
	int numOfLoot = IRAND_RANGE(1, maxLoots);
	
	for (int i=0; i<numOfLoot; ++i) {
		spawnLoot(lootFileNames.getRandom());
	}
}

void ComponentDropsLoot::handleMessagePassWorld(const MessagePassWorld *message) {
	world = message->world;
}

void ComponentDropsLoot::spawnLoot(const FileName &lootFilename) {
	float projectileSpeed = FRAND_RANGE(1.0f, 8.0f); // TODO: should not be hard coded
	float radialOffsetRatio = FRAND_RANGE(0.30f, 2.00f); // TODO: should not be hard coded
	
	float radialOffset = lastReportedRadius * radialOffsetRatio;
	vec3 direction = GetRandomVector(1.0f);
	vec3 velocity = direction * projectileSpeed;
	vec3 position = lastReportedPosition + direction*radialOffset;
	
	spawnLoot(lootFilename, position, velocity);
}

void ComponentDropsLoot::spawnLoot(const FileName & lootFilename,
                                   const vec3 &position,
                                   const vec3 &velocity) {
	ASSERT(world, "Null pointer: world");
	const PropertyBag data = PropertyBag::fromFile(lootFilename);
	const PropertyBag base = data.getBag("components");
	ComponentDataSet s = ComponentDataSet::parse(base);
	
#if 1
	// Specifically enable physics for loot
	{
		PropertyBag &physicsBodyData = s.get("PhysicsBody");
		physicsBodyData.remove("initiallyDisablePhysics", 0);
		physicsBodyData.add("initiallyDisablePhysics", false);
	}
#endif
	
	world->getObjects().spawn(s, position, velocity);
}

void ComponentDropsLoot::handleEventPositionUpdate( const EventPositionUpdate *message ) {
	lastReportedPosition = message->position;
}

void ComponentDropsLoot::handleEventRadiusUpdate( const EventRadiusUpdate *message ) {
	lastReportedRadius = message->radius;
}
