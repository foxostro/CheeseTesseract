#include "stdafx.h"
#include "World.h"
#include "PhysicsEngine.h"
#include "ComponentPhysicsGeom.h"

#include "EventPositionUpdate.h"
#include "EventOrientationUpdate.h"
#include "EventHeightUpdate.h"

ComponentPhysicsGeom::
ComponentPhysicsGeom(UID uid, ScopedEventHandler *parentScope)
: ComponentPhysics(uid, parentScope)
{
	resetMembers();

	REGISTER_HANDLER(ComponentPhysicsGeom::handleMessagePassWorld);
	REGISTER_HANDLER(ComponentPhysicsGeom::handleActionSetPosition);
	REGISTER_HANDLER(ComponentPhysicsGeom::handleActionLookAt);
	REGISTER_HANDLER(ComponentPhysicsGeom::handleActionSetOrientation);
	REGISTER_HANDLER(ComponentPhysicsGeom::handleEventDeclareInitialPosition);
	REGISTER_HANDLER(ComponentPhysicsGeom::handleActionDeleteActor);
}

void ComponentPhysicsGeom::handleActionDeleteActor( const ActionDeleteActor *action )
{
	if(action->id == getUID())
	{
		if(geom){dGeomDestroy(geom);} geom=0;
		resetMembers();
	}
}

void ComponentPhysicsGeom::handleMessagePassWorld( const MessagePassWorld *message )
{
	physicsEngine = (message->world)->getPhysicsEngine();
}

void ComponentPhysicsGeom::handleActionSetPosition( const ActionSetPosition *action )
{
	setPosition(action->position);
}

void ComponentPhysicsGeom::handleEventDeclareInitialPosition( const EventDeclareInitialPosition *event )
{
	setPosition(event->position);
}

void ComponentPhysicsGeom::handleActionLookAt( const ActionLookAt *action )
{
	/*
	Why is this code disabled?
	There is a disconnect between the orientation of a model and the
	orientation of the underlying physics geometry
	(See independentModelOrientation).  I have decided to have the lookAt
	action take care of the orientation of the MODEL, specifically, and handle
	physics orientation through regular matrices.
	*/
#if 0
	const vec3 lookDirection = action->lookDirection;

	const vec3 eye = getPosition();
	const vec3 center = eye + lookDirection;
	const vec3 up(0,0,1);

	mat3 orientation;
	orientation.lookAt(eye, center, up);
	setOrientation(orientation);
#else
	mat3 orientation;
	orientation.identity();
	setOrientation(orientation);
#endif
}

void ComponentPhysicsGeom::resetMembers()
{
	collisionRadius = 1.0f;
	desiredHeight = 1.0f;
	geom=0;
}

void ComponentPhysicsGeom::update(float)
{
	// Declare the final position and orientation for this frame

	EventPositionUpdate m1(getPosition());
	sendEvent(&m1);

	EventOrientationUpdate m2(getOrientation());
	sendEvent(&m2);
}

void ComponentPhysicsGeom::draw() const
{
	if(!isDebugDisplayEnabled())
	{
		return;
	}

	CHECK_GL_ERROR();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	drawAxes();
	drawPhysicsGeom();
	glPopAttrib();

	CHECK_GL_ERROR();
}

void ComponentPhysicsGeom::load(const PropertyBag &data)
{
	resetMembers();

	desiredHeight = data.getFloat("height");
	collisionRadius = data.getFloat("radius");

	// Create as physics geometry
	if(geom){dGeomDestroy(geom);} geom=0;
	createGeom(data.getString("physicsGeometryType"));

	// Set initial position
	{
		vec3 position;
		if(data.get("position", position)) // optional tag
		{
			setPosition(position);
		}
	}

	// Declare the initial state
	{
		EventPositionUpdate m1(getPosition());
		sendEvent(&m1);

		EventOrientationUpdate m2(getOrientation());
		sendEvent(&m2);

		EventHeightUpdate m3(desiredHeight);
		sendEvent(&m3);
	}
}

vec3 ComponentPhysicsGeom::getPosition() const
{
	// Get our position back from the physics engine
	const dReal *p = dGeomGetPosition(geom);
	return vec3((float)p[0], (float)p[1], (float)p[2]);
}

void ComponentPhysicsGeom::setPosition(const vec3 &position)
{
	dGeomSetPosition(geom, position.x, position.y, position.z);

	EventPositionUpdate m(getPosition());
	sendEvent(&m);
}

mat3 ComponentPhysicsGeom::getOrientation() const
{
	const dReal *r = dGeomGetRotation(geom);

	return mat3(vec3((float)r[0], (float)r[1], (float)r[2]),
	            vec3((float)r[4], (float)r[5], (float)r[6]),
	            vec3((float)r[8], (float)r[9], (float)r[10]));
}

void ComponentPhysicsGeom::setOrientation(const mat3 &orientation)
{
	dMatrix3 r;

	const vec3 x = orientation.getAxisX().getNormal();
	const vec3 y = orientation.getAxisY().getNormal();
	const vec3 z = orientation.getAxisZ().getNormal();

	r[0] = x.x; r[1] = x.y; r[2] = x.z; r[3] = 0.0f;
	r[4] = y.x; r[5] = y.y; r[6] = y.z; r[7] = 0.0f;
	r[8] = z.x; r[9] = z.y; r[10]= z.z; r[11]= 0.0f;

	dGeomSetRotation(geom, r);

	{broadcastOrientationUpdate();
	}
}

void ComponentPhysicsGeom::createGeom( const string &type )
{
	     if(type == "Box")      createGeomBox();
	else if(type == "Cylinder") createGeomCylinder();
	else if(type == "Capsule")  createGeomCapsule();
	else if(type == "Sphere")   createGeomSphere();
	else FAIL("Unknown geom type: " + type);

	ActorID uid = getActorID();
	ActorID *data = new ActorID;
	*data = uid;
	dGeomSetData(geom, data);
}

void ComponentPhysicsGeom::createGeomSphere()
{
	ASSERT(physicsEngine, "\"physicsEngine\" was null");
	geom = dCreateSphere(physicsEngine->getSpace(), collisionRadius);
}

void ComponentPhysicsGeom::createGeomBox()
{
	ASSERT(physicsEngine, "\"physicsEngine\" was null");
	geom = dCreateBox(physicsEngine->getSpace(),
	                  collisionRadius,
					  collisionRadius,
					  desiredHeight);
}

void ComponentPhysicsGeom::createGeomCylinder()
{
	ASSERT(physicsEngine, "\"physicsEngine\" was null");
	geom = dCreateCylinder(physicsEngine->getSpace(),
	                       collisionRadius,
	                       desiredHeight);
}

void ComponentPhysicsGeom::createGeomCapsule()
{
	ASSERT(physicsEngine, "\"physicsEngine\" was null");

	const float capsuleHeight = desiredHeight - collisionRadius*2.0f;

	geom = dCreateCCylinder(physicsEngine->getSpace(),
	                        collisionRadius,
	                        capsuleHeight);
}

void ComponentPhysicsGeom::drawPhysicsGeom() const
{
	CHECK_GL_ERROR();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	PhysicsEngine::drawGeom(geom);
	glPopAttrib();
	CHECK_GL_ERROR();
}

void ComponentPhysicsGeom::drawAxes() const
{
	CHECK_GL_ERROR();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
		glLineWidth(2.0f);

		const mat3 orientation = getOrientation();
		mat4 transformation(getPosition(),
			                orientation.getAxisX(),
							orientation.getAxisY(),
							orientation.getAxisZ());

		glPushMatrix();
		glMultMatrixf(transformation);

		glBegin(GL_LINES);
		glColor4fv(red);
		glVertex3fv(vec3(0,0,0));
		glVertex3fv(orientation.getAxisX());

		glColor4fv(green);
		glVertex3fv(vec3(0,0,0));
		glVertex3fv(orientation.getAxisY());

		glColor4fv(blue);
		glVertex3fv(vec3(0,0,0));
		glVertex3fv(orientation.getAxisZ());
		glEnd();

		glPopMatrix();
	}
	glPopAttrib();
	CHECK_GL_ERROR();
}

void ComponentPhysicsGeom::handleActionSetOrientation( const ActionSetOrientation *action )
{
	setOrientation(action->orientation);
}

void ComponentPhysicsGeom::broadcastOrientationUpdate()
{
	EventOrientationUpdate m(getOrientation());
	sendEvent(&m);
}
