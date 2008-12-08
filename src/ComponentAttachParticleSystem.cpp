#include "stdafx.h"
#include "ParticleSystem.h"
#include "ComponentPhysics.h"
#include "ComponentAttachParticleSystem.h"

ComponentAttachParticleSystem::
ComponentAttachParticleSystem(UID uid, ScopedEventHandler *blackBoard)
		: Component(uid, blackBoard),
		world(0),
		active(true) {
	REGISTER_HANDLER(ComponentAttachParticleSystem::handleEventOrientationUpdate);
	REGISTER_HANDLER(ComponentAttachParticleSystem::handleEventPositionUpdate);
	REGISTER_HANDLER(ComponentAttachParticleSystem::handleMessagePassWorld);
	REGISTER_HANDLER(ComponentAttachParticleSystem::handleActionDeleteActor);
	
	resetMembers();
}

void ComponentAttachParticleSystem::resetMembers() {
	lastReportedPosition.zero();
	lastReportedOrientation.identity();
}

void ComponentAttachParticleSystem::update(float) {
	if (!active) return;
	
	ParticleSystem *system = *handle;
	
	if (system) {
		const vec2 x = lastReportedOrientation.getAxisX().xy();
		const float rotation = getAngle(x, vec2(1,0));
		system->setRotation(rotation);
		system->setPosition(lastReportedPosition);
	}
}

void ComponentAttachParticleSystem::load(const PropertyBag &data) {
	ASSERT(world, "World has not yet been set");
	
	const vec3 position = vec3(0,0,0);
	const float rotation = 0.0f;
	const FileName fileName = data.getFileName("fileName");
	
	handle = world->particleEngine->add(fileName,
	                                    position,
	                                    rotation,
	                                    world->getTextureFactory());
}

void ComponentAttachParticleSystem::handleEventOrientationUpdate( const EventOrientationUpdate *message ) {
	ASSERT(message, "Null parameter: message");
	lastReportedOrientation = message->orientation;
}

void ComponentAttachParticleSystem::handleEventPositionUpdate( const EventPositionUpdate *message ) {
	ASSERT(message, "Null parameter: message");
	lastReportedPosition = message->position;
}

void ComponentAttachParticleSystem::handleMessagePassWorld( const MessagePassWorld *message ) {
	ASSERT(message, "Null parameter: message");
	world = message->world;
}

void ComponentAttachParticleSystem::handleActionDeleteActor( const ActionDeleteActor *message ) {
	ASSERT(message, "Null parameter: message");
	
	if (message->id == getUID()) {
		ASSERT(world, "world has not yet been set");
		world->particleEngine->remove(handle);
		active=false;
	}
}
