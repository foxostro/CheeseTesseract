#ifndef COMPONENT_PHYSICS_H
#define COMPONENT_PHYSICS_H

#include "Component.h"

/** Game world position and orientation */
class ComponentPhysics : public Component
{
public:
	virtual string getTypeString() const { return "Physics"; }

	ComponentPhysics(UID _uid, ScopedEventHandler *_blackBoard);

	virtual vec3 getPosition() const = 0;
	virtual mat3 getOrientation() const = 0;

	virtual void setPosition(const vec3 &position) = 0;
	virtual void setOrientation(const mat3 &orientation) = 0;

	virtual dGeomID getGeom() const = 0;
	virtual dBodyID getBody() const = 0;
	
	virtual float getRadius() const = 0;
};

#endif
