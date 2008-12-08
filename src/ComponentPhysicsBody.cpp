#include "stdafx.h"
#include "World.h"
#include "PhysicsEngine.h"
#include "ComponentRenderAsModel.h"
#include "ComponentPhysicsBody.h"

#include "EventPositionUpdate.h"
#include "EventOrientationUpdate.h"
#include "EventHeightUpdate.h"

ComponentPhysicsBody::ComponentPhysicsBody(UID uid, ScopedEventHandler *parentScope)
		: ComponentPhysics(uid, parentScope) {
	resetMembers();
	
	REGISTER_HANDLER(ComponentPhysicsBody::handleMessagePassWorld);
	REGISTER_HANDLER(ComponentPhysicsBody::handleActionSetPosition);
	REGISTER_HANDLER(ComponentPhysicsBody::handleActionLookAt);
	REGISTER_HANDLER(ComponentPhysicsBody::handleActionSetOrientation);
	REGISTER_HANDLER(ComponentPhysicsBody::handleEventDeclareInitialPosition);
	REGISTER_HANDLER(ComponentPhysicsBody::handleActionDeleteActor);
	REGISTER_HANDLER(ComponentPhysicsBody::handleActionPhysicsDisable);
	REGISTER_HANDLER(ComponentPhysicsBody::handleActionPhysicsEnable);
}

void ComponentPhysicsBody::handleActionDeleteActor( const ActionDeleteActor *action ) {
	const ActorID myID = getParentScopePtr()->getUID();
	
	if (myID == action->id) {
		destroyPhysicsResources();
		resetMembers();
	}
}

void ComponentPhysicsBody::handleMessagePassWorld( const MessagePassWorld *message ) {
	physicsEngine = (message->world)->getPhysicsEngine();
}

void ComponentPhysicsBody::handleActionSetPosition( const ActionSetPosition *action ) {
	setPosition(action->position);
}

void ComponentPhysicsBody::handleActionLookAt( const ActionLookAt *action ) {
	/*
	Why is this code disabled?
	There is a disconnect between the orientation of a model and the
	orientation of the underlying physics geometry
	(See independentModelOrientation).  I have decided to have the lookAt
	action take care of the orientation of the MODEL, specifically, and handle
	physics orientation through regular matrices.
	*/
#if 0
	if (!body) return;
	
	const vec3 &lookDirection = action->lookDirection;
	
	const vec3 eye = getPosition();
	const vec3 center = eye + lookDirection;
	const vec3 up(0,0,1);
	
	mat3 orientation;
	orientation.lookAt(eye, center, up);
	setOrientation(orientation);
#else
	if (body) {
		mat3 orientation;
		orientation.identity();
		setOrientation(orientation);
	}
#endif
}

/* Here if we want to add this capability...
void ComponentPhysicsBody::handleActionApplyForce(const ActionApplyForce *action)
{
	if(!body) return;

	dBodyAddForce(body, action->force.x, action->force.y, action->force.z);
}
*/

void ComponentPhysicsBody::handleEventDeclareInitialPosition( const EventDeclareInitialPosition *event ) {
	setPosition(event->position);
	setVelocity(event->velocity);
}

void ComponentPhysicsBody::resetMembers() {
	modelScale = 1.0f;
	body=0;
	geom=0;
	disableCollisions=false;
	
	collisionRadius = 1.0f;
	desiredHeight = 1.0f;
	influencedByGravity = true;
	physicsGeometryType = "Box";
	kilograms = 1.0f;
}

void ComponentPhysicsBody::update(float) {
	if (disableCollisions) {
		destroyPhysicsResources();
	} else {
		// Poll for last known position and orientation
		pollPosition();
		pollOrientation();
		
		// Declare the final position and orientation for this frame
		broadcastPositionUpdate();
		broadcastOrientationUpdate();
	}
}

void ComponentPhysicsBody::draw() const {
	if (!isDebugDisplayEnabled()) {
		return;
	}
	
	CHECK_GL_ERROR();
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		drawAxes();
		drawPhysicsGeom();
	}
	glPopAttrib();
	
	CHECK_GL_ERROR();
}

void ComponentPhysicsBody::load(const PropertyBag &data) {
	ASSERT(physicsEngine, "physics engine is null");
	
	resetMembers();
	
	// Destroy any previous rigid body
	destroyAllJoints();
	destroyPhysicsResources();
	
	// Create the rigid body
	loadRigidBodyData(data);
	createBodyRigidBodyData();
	
	// Load initial position from data, if available
	loadPosition(data);
	
	// Poll for last known position and orientation
	pollPosition();
	pollOrientation();
	
	// Declare the initial state
	broadcastPositionUpdate();
	broadcastOrientationUpdate();
	broadcastHeightUpdate();
	
	// Disable physics initially?
	bool initiallyDisablePhysics = false;
	if (data.get("initiallyDisablePhysics", initiallyDisablePhysics)) {
		if (initiallyDisablePhysics) {
			ActionPhysicsDisable m;
			sendAction(&m);
		}
	}
}

void ComponentPhysicsBody::setPosition(const vec3 &position) {
	lastKnownPosition = position;
	
	if (!disableCollisions) {
		ASSERT(body, "Physics body is nil");
		dBodySetPosition(body, position.x, position.y, position.z);
	}
	
	broadcastPositionUpdate();
}

void ComponentPhysicsBody::setVelocity(const vec3 &velocity) {
	if (!disableCollisions) {
		ASSERT(body, "Physics body is nil");
		dBodySetLinearVel(body, velocity.x, velocity.y, velocity.z);
	}
}

void ComponentPhysicsBody::setOrientation(const mat3 &m) {
	lastKnownOrientation = m;
	
	if (!disableCollisions) {
		ASSERT(body, "Physics body is nil");
		
		dMatrix3 r;
		
		const vec3 x = m.getAxisX().getNormal();
		const vec3 y = m.getAxisY().getNormal();
		const vec3 z = m.getAxisZ().getNormal();
		
		r[0] = x.x;
		r[1] = x.y;
		r[2] = x.z;
		r[3] = 0.0f;
		r[4] = y.x;
		r[5] = y.y;
		r[6] = y.z;
		r[7] = 0.0f;
		r[8] = z.x;
		r[9] = z.y;
		r[10]= z.z;
		r[11]= 0.0f;
		
		dBodySetRotation(body, r);
	}
	
	broadcastOrientationUpdate();
}

void ComponentPhysicsBody::createGeom(const string &type, float kilograms) {
	if (type == "Box")      createGeomBox(kilograms);
	else if (type == "Cylinder") createGeomCylinder(kilograms);
	else if (type == "Capsule")  createGeomCapsule(kilograms);
	else if (type == "Sphere")   createGeomSphere(kilograms);
	else FAIL("Unknown geom type: " + type);
	
	ActorID uid = getActorID();
	ActorID *data = new ActorID;
	*data = uid;
	dGeomSetData(geom, data);
}

void ComponentPhysicsBody::createGeomSphere(float kilograms) {
	ASSERT(physicsEngine, "\"physicsEngine\" was null");
	dMass mass;
	geom = dCreateSphere(physicsEngine->getSpace(), collisionRadius);
	dGeomSetBody(geom, body);
	dMassSetSphereTotal(&mass, kilograms, collisionRadius);
	dBodySetMass(body, &mass);
}

void ComponentPhysicsBody::createGeomBox(float total_mass) {
	ASSERT(physicsEngine, "\"physicsEngine\" was null");
	
	dMass mass;
	dMassSetBoxTotal(&mass,
	                 total_mass,
	                 collisionRadius,
	                 collisionRadius,
	                 desiredHeight);
	                 
	geom = dCreateBox(physicsEngine->getSpace(),
	                  collisionRadius,
	                  collisionRadius,
	                  desiredHeight);
	                  
	dGeomSetBody(geom, body);
	
	dBodySetMass(body, &mass);
}

void ComponentPhysicsBody::createGeomCylinder(float kilograms) {
	ASSERT(physicsEngine, "\"physicsEngine\" was null");
	
	dMass mass;
	
	geom = dCreateCylinder(physicsEngine->getSpace(),
	                       collisionRadius,
	                       desiredHeight);
	                       
	dGeomSetBody(geom, body);
	
	dMassSetCylinderTotal(&mass,
	                      kilograms,
	                      3, // aligned with Z-axis
	                      collisionRadius,
	                      desiredHeight);
	                      
	dBodySetMass(body, &mass);
}

void ComponentPhysicsBody::createGeomCapsule(float kilograms) {
	ASSERT(physicsEngine, "\"physicsEngine\" was null");
	
	dMass mass;
	
	const float capsuleHeight = desiredHeight - collisionRadius*2.0f;
	
	geom = dCreateCCylinder(physicsEngine->getSpace(),
	                        collisionRadius,
	                        capsuleHeight);
	                        
	dGeomSetBody(geom, body);
	
	dMassSetCappedCylinderTotal(&mass,
	                            kilograms,
	                            3, // aligned with Z-axis
	                            collisionRadius,
	                            capsuleHeight);
	                            
	dBodySetMass(body, &mass);
}

void ComponentPhysicsBody::drawPhysicsGeom() const {
	if (!geom) return;
	
	CHECK_GL_ERROR();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//PhysicsEngine::drawGeomAABB(geom);
	PhysicsEngine::drawGeom(geom);
	glPopAttrib();
	CHECK_GL_ERROR();
}

void ComponentPhysicsBody::drawAxes() const {
	if (!body) return;
	
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

void ComponentPhysicsBody::handleActionPhysicsDisable(const ActionPhysicsDisable *) {
	disableCollisions = true;
}

void ComponentPhysicsBody::handleActionPhysicsEnable(const ActionPhysicsEnable *) {
	if (disableCollisions) {
		disableCollisions = false;
		
		// Recreate rigid body data
		createBodyRigidBodyData();
		
		/*
		Reaffirm last known transformation.
		NOTE: Raise just above the ground to allow the object to
		make good contact with the floor beneath it.
		*/
		setPosition(lastKnownPosition + vec3(0.0f, 0.0f, 0.2f));
		setOrientation(lastKnownOrientation);
		
		// Declare the initial state
		broadcastPositionUpdate();
		broadcastOrientationUpdate();
		broadcastHeightUpdate();
	}
}

void ComponentPhysicsBody::destroyAllJoints() {
	if (!body) {
		return;
	}
	
	int numOfJoints = dBodyGetNumJoints(body);
	vector<dJointID> joints;
	
	for (int i=0; i<numOfJoints; ++i) {
		dJointID joint = dBodyGetJoint(body, i);
		joints.push_back(joint);
	}
	
	for (size_t i=0; i<joints.size(); ++i) {
		dJointDestroy(joints[i]);
	}
}

void ComponentPhysicsBody::resetBodyParameters( dBodyID body ) {
	ASSERT(body, "Physics body is nil");
	dBodySetForce(body, 0, 0, 0);
	dBodySetTorque(body, 0, 0, 0);
	dBodySetLinearVel(body, 0, 0, 0);
	dBodySetAngularVel(body, 0, 0, 0);
}

void ComponentPhysicsBody::destroyPhysicsResources() {
	if (body) {
		dBodyDestroy(body);
	}
	
	if (geom) {
		dGeomDestroy(geom);
	}
	
	body=0;
	geom=0;
	mesh=0;
}

void ComponentPhysicsBody::handleActionSetOrientation( const ActionSetOrientation *action ) {
	setOrientation(action->orientation);
}

vec3 ComponentPhysicsBody::pollPosition() {
	ASSERT(body, "Physics body is nil");
	const dReal *p = dBodyGetPosition(body);
	lastKnownPosition = vec3((float)p[0], (float)p[1], (float)p[2]);
	return lastKnownPosition;
}

mat3 ComponentPhysicsBody::pollOrientation() {
	ASSERT(body, "Physics body is not nil");
	const dReal *r = dBodyGetRotation(body);
	lastKnownOrientation = mat3(vec3((float)r[0], (float)r[1], (float)r[2]),
	                            vec3((float)r[4], (float)r[5], (float)r[6]),
	                            vec3((float)r[8], (float)r[9], (float)r[10]));
	return lastKnownOrientation;
}

void ComponentPhysicsBody::createBodyRigidBodyData() {
	body = dBodyCreate(physicsEngine->getWorld());
	createGeom(physicsGeometryType, kilograms);
	dBodySetGravityMode(body, influencedByGravity ? 1 : 0);
	resetBodyParameters(body);
}

void ComponentPhysicsBody::loadPosition( const PropertyBag &data ) {
	vec3 position;
	if (data.get("position", position)) { // optional tag
		setPosition(position);
	}
}

void ComponentPhysicsBody::broadcastOrientationUpdate() {
	EventOrientationUpdate m(getOrientation());
	sendEvent(&m);
}

void ComponentPhysicsBody::broadcastPositionUpdate() {
	EventPositionUpdate m(getPosition());
	sendEvent(&m);
}

void ComponentPhysicsBody::broadcastHeightUpdate() {
	EventHeightUpdate m(desiredHeight);
	sendEvent(&m);
}
