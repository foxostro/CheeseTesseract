#ifndef COMPONENT_PHYSICS_BODY_H
#define COMPONENT_PHYSICS_BODY_H

#include "PropertyBag.h"
#include "AnimationController.h"
#include "ComponentPhysics.h"
#include "ActionDeleteActor.h"
#include "ActionPhysicsDisable.h"
#include "ActionPhysicsEnable.h"
#include "ActionSetPosition.h"
#include "ActionLookAt.h"
#include "ActionSetOrientation.h"
#include "EventDeclareInitialPosition.h"
#include "MessagePassWorld.h"

class PhysicsEngine;

/** Game world position and orientation */
class ComponentPhysicsBody : public ComponentPhysics
{
public:
	ComponentPhysicsBody(UID uid, ScopedEventHandler *parentScope);

	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);

	void loadRigidBodyData( const PropertyBag &data )
	{
		desiredHeight = data.getFloat("height");
		collisionRadius = data.getFloat("radius");
		physicsGeometryType = data.getString("physicsGeometryType");
		kilograms = data.getFloat("kilograms");
		data.get("gravity", influencedByGravity); //optional
	}

	/** Declares the position and orientation for the frame */
	virtual void update(float milliseconds);

	/** Draws the object */
	virtual void draw() const;

	/** Resets all object members to defaults */
	virtual void resetMembers();

	/** Sets rigid body velocity */
	void setVelocity(const vec3 &velocity);

	/** Sets body orientation */
	void setOrientation(const mat3 &orientation);

	/** Sets body position */
	void setPosition(const vec3 &position);

	/** from ComponentPhysics, the super class */
	virtual vec3 getPosition() const
	{
		return lastKnownPosition;
	}

	/** from ComponentPhysics, the super class */
	virtual mat3 getOrientation() const
	{
		return lastKnownOrientation;
	}

	/** from ComponentPhysics, the super class */
	virtual dGeomID getGeom() const
	{
		return geom;
	}

	/** from ComponentPhysics, the super class */
	virtual dBodyID getBody() const
	{
		return body;
	}

	/** from ComponentPhysics, the super class */
	virtual float getRadius() const
	{
		return collisionRadius;
	}

private:
	void handleActionDeleteActor(const ActionDeleteActor *action);
	void handleActionPhysicsDisable(const ActionPhysicsDisable *action);
	void handleActionPhysicsEnable(const ActionPhysicsEnable *action);
	void handleMessagePassWorld(const MessagePassWorld *message);
	void handleActionSetPosition(const ActionSetPosition *action);
	void handleActionLookAt(const ActionLookAt *action);
	void handleActionSetOrientation(const ActionSetOrientation *action);
	void handleEventDeclareInitialPosition(const EventDeclareInitialPosition *event);

	void createGeom(const string &type, float kilograms);
	void createGeomBox(float kilograms);
	void createGeomCylinder(float kilograms);
	void createGeomCapsule(float kilograms);
	void createGeomSphere(float kilograms);

	/**
	Destroy all joints connected to the body
	(including motor joints and contact joints)
	*/
	void destroyAllJoints();

	/** Destroy physics resources */
	void destroyPhysicsResources();

	/**
	Reset body parameters to remove all forces, torques, accelerations,
	velocities, etc.
	@param body Physics body
	*/
	static void resetBodyParameters(dBodyID body);

	/** Draw local axes for this object */
	void drawAxes() const;

	/** Draw the physics engine geometry for this object */
	void drawPhysicsGeom() const;

	/** Polls the physics engine for the object position */
	vec3 pollPosition();

	/** Polls the physics engine for the object orientation */
	mat3 pollOrientation();

	/** Load initial position from data, if available */
	void loadPosition( const PropertyBag &data );

	/** Creates rigid body from previously loaded rigid body parameters */
	void createBodyRigidBodyData();

	void broadcastOrientationUpdate();

	void broadcastPositionUpdate();

	void broadcastHeightUpdate();

private:
	dBodyID body;
	dGeomID geom;
	dTriMeshDataID mesh; // only valid when geom is a trimesh
	shared_ptr<PhysicsEngine> physicsEngine;

	float collisionRadius;
	float desiredHeight;
	float modelScale;
	bool disableCollisions;
	vec3 lastKnownPosition;
	mat3 lastKnownOrientation;

	bool influencedByGravity;
	string physicsGeometryType;
	float kilograms;
};

#endif
