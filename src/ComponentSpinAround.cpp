#include "stdafx.h"
#include "ComponentSpinAround.h"
#include "ActionSetPosition.h"
#include "ActionSetOrientation.h"

ComponentSpinAround::
ComponentSpinAround(UID uid, ScopedEventHandler *parentScope)
: Component(uid, parentScope)
{
	REGISTER_HANDLER(ComponentSpinAround::handleEventDeclareInitialPosition);
	resetMembers();
}

void ComponentSpinAround::resetMembers()
{
	facingAngle = 0.0f;
	rotationSpeed = (float)(2.0 * M_PI) / 1000.0f;
	bounce = 0.0f;
	bounceHeight = 0.5f;
	bounceSpeed = (float)(2.0 * M_PI) / 1000.0f;
}

void ComponentSpinAround::load(const PropertyBag &data)
{
	resetMembers();
	rotationSpeed = data.getFloat("rotationSpeed");
	bounceHeight = data.getFloat("bounceHeight");
	bounceSpeed = data.getFloat("bounceSpeed");
}

void ComponentSpinAround::update(float milliseconds)
{
	facingAngle = angle_clamp(facingAngle += rotationSpeed*milliseconds);
	bounce = angle_clamp(bounce += bounceSpeed*milliseconds);

	broadcastOrientation();
	broadcastPosition();
}

void ComponentSpinAround::handleEventDeclareInitialPosition( const EventDeclareInitialPosition *event )
{
	initialPosition = event->position;
}

void ComponentSpinAround::broadcastPosition()
{
	vec3 offset = vec3(0.0f, 0.0f, cosf(bounce))*bounceHeight;
	vec3 pos = initialPosition + offset;
	ActionSetPosition m(pos);
	sendAction(&m);
}

void ComponentSpinAround::broadcastOrientation()
{
	mat3 orientation = mat3::fromRotateZ(facingAngle);
	orientation = orientation * mat3::fromRotateX((float)(M_PI / 2.0));
	ActionSetOrientation m(orientation);
	sendAction(&m);
}
