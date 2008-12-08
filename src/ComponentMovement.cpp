#include "stdafx.h"
#include "World.h"
#include "GraphicsDevice.h"
#include "ComponentPhysicsBody.h"
#include "ComponentMovement.h"
#include "ComponentDeathBehavior.h"

#include "EventCharacterHasDied.h"
#include "EventCharacterRevived.h"

#include "ActionChangeAnimation.h"
#include "ActionSetPosition.h"
#include "ActionLookAt.h"

ComponentMovement::ComponentMovement(UID uid,  ScopedEventHandler *s)
		: Component(uid, s),
		amotor(0),
		lmotor(0),
		world(0),
		lastAction(Stand),
		dead(false),
		disablePhysics(false) {
	REGISTER_HANDLER(ComponentMovement::handleActionPerformAction);
	REGISTER_HANDLER(ComponentMovement::handleMessagePassWorld);
	REGISTER_HANDLER(ComponentMovement::handleEventCharacterRevived);
	REGISTER_HANDLER(ComponentMovement::handleEventCharacterHasDied);
	REGISTER_HANDLER(ComponentMovement::handleActionSetCharacterFaceAngle);
	REGISTER_HANDLER(ComponentMovement::handleActionTurn);
	REGISTER_HANDLER(ComponentMovement::handleActionPhysicsDisable);
	REGISTER_HANDLER(ComponentMovement::handleActionPhysicsEnable);
	
	resetMembers();
}

void ComponentMovement::resetMembers() {
	dead = false;
	velocity = vec3(0,0,0);
	facingAngle = 0.0f;
	maxForce = 5000;
	lastAction = Stand;
	lastReportedDeathBehavior = Corpse;
	
	if (amotor) dJointDestroy(amotor);
	if (lmotor) dJointDestroy(lmotor);
}

void ComponentMovement::load(const PropertyBag &data) {
	topSpeed = data.getFloat("topSpeed");
	maxForce = (dReal)data.getFloat("maxForce");
	createMotorJoints();
}

void ComponentMovement::update(float) {
	// Set model orientation properly
	{
		ActionLookAt m(facingAngle);
		sendAction(&m);
	}
	
	// Request animation change based on walk speed
	if (!dead || getDeathBehavior()==Ghost) {
		// Request to change the animation
		const string anim = determineCurrentAnim();
		ActionChangeAnimation m(anim);
		sendAction(&m);
		
		// Move the character
		dJointSetLMotorParam(lmotor, dParamVel,  velocity.x);
		dJointSetLMotorParam(lmotor, dParamVel2, velocity.y);
	}
	
	// house keeping
	velocity.zero();
	lastAction = Stand;
}

string ComponentMovement::determineCurrentAnim() const {
	if (lastAction == Stand) {
		return "idle";
	} else {
		return "run";
	}
}

void ComponentMovement::handleActionPerformAction( const ActionPerformAction *action ) {
	applyAction(action->action);
}

void ComponentMovement::handleActionSetCharacterFaceAngle( const ActionSetCharacterFaceAngle *action ) {
	turnToFace(action->facingAngle);
}

void ComponentMovement::handleActionTurn( const ActionTurn *action ) {
	turn(action->dFacingAngle);
}

void ComponentMovement::handleMessagePassWorld( const MessagePassWorld *message ) {
	physicsEngine = (world = message->world)->getPhysicsEngine();
}

void ComponentMovement::applyAction(CharacterAction action) {
	vec2 x(-1,  1);
	vec2 y(-1, -1);
	
	if (!dead || getDeathBehavior()==Ghost) {
		switch (action) {
			/* Step in an absolute direction */
		case StepNorth:
			walk(y);
			turnToFace(velocity.xy());
			break;
		case StepSouth:
			walk(-y);
			turnToFace(velocity.xy());
			break;
		case StepEast:
			walk(x);
			turnToFace(velocity.xy());
			break;
		case StepWest:
			walk(-x);
			turnToFace(velocity.xy());
			break;
		case StepNorthEast:
			walk(y+x);
			turnToFace(velocity.xy());
			break;
		case StepNorthWest:
			walk(y-x);
			turnToFace(velocity.xy());
			break;
		case StepSouthEast:
			walk(-y+x);
			turnToFace(velocity.xy());
			break;
		case StepSouthWest:
			walk(-y-x);
			turnToFace(velocity.xy());
			break;
			
			/* Step in a relative direction */
		case StepForward:
			walk(getDirectionVector(facingAngle));
			break;
		case StepForwardRight:
			walk(getDirectionVector(facingAngle - (float)(M_PI * 0.25)));
			break;
		case StepRight:
			walk(getDirectionVector(facingAngle - (float)(M_PI * 0.50)));
			break;
		case StepBackWardRight:
			walk(getDirectionVector(facingAngle - (float)(M_PI * 0.75)));
			break;
		case StepBackWard:
			walk(getDirectionVector(facingAngle - (float)M_PI));
			break;
		case StepBackWardLeft:
			walk(getDirectionVector(facingAngle + (float)(M_PI * 0.75)));
			break;
		case StepLeft:
			walk(getDirectionVector(facingAngle + (float)(M_PI * 0.50)));
			break;
		case StepForwardLeft:
			walk(getDirectionVector(facingAngle + (float)(M_PI * 0.25)));
			break;
			
			/* Other */
		case BeginChargeUp:
			chargeUp();
			break;
		case EndChargeUp:
			endChargeUp();
			break;
		case AttackOnce:
			attackOnce();
			break;
		case Suicide:
			suicide();
			break;
			
		default:
			break;
		}
		lastAction = action;
	} else {
		lastAction = InvalidAction;
	}
}

void ComponentMovement::endChargeUp() { /* Stub */ }

void ComponentMovement::chargeUp() { /* Stub */ }

void ComponentMovement::attackOnce() { /* Stub */ }

void ComponentMovement::walk(const vec2 &direction, float speed) {
	ASSERT(speed >= -1.0f, "Speed is too negative: " + ftos(speed));
	ASSERT(speed <= +1.0f, "Speed is too positive: " + ftos(speed));
	
	const float modifiedSpeed = speed * topSpeed;
	vec3 dv = vec3(-direction, 0.0f).getNormal() * modifiedSpeed;
	
	velocity = velocity + dv;
}

void ComponentMovement::handleEventCharacterRevived(const EventCharacterRevived *) {
	dead = false;
}

void ComponentMovement::handleEventCharacterHasDied(const EventCharacterHasDied *) {
	dead = true;
	velocity.zero();
	
	dJointSetLMotorParam(lmotor, dParamVel,  velocity.x);
	dJointSetLMotorParam(lmotor, dParamVel2, velocity.y);
	
	ActionChangeAnimation m("dying");
	sendAction(&m);
}

void ComponentMovement::suicide() {
	EventCharacterHasDied m;
	sendEvent(&m);
}

DeathBehavior ComponentMovement::getDeathBehavior() const {
	return lastReportedDeathBehavior;
}

void ComponentMovement::turn( float dAngle ) {
	if (!dead || getDeathBehavior()==Ghost) {
		facingAngle = angle_clamp(facingAngle + dAngle);
	}
}

void ComponentMovement::handleActionPhysicsDisable(const ActionPhysicsDisable*) {
	disablePhysics = true;
}

void ComponentMovement::handleActionPhysicsEnable(const ActionPhysicsEnable*) {
	if (disablePhysics) {
		disablePhysics = false;
		
		// recreated motor joints
		createMotorJoints();
	}
}

void ComponentMovement::createMotorJoints() {
	ASSERT(physicsEngine, "Null pointer: physicsEngine");
	
	dBodyID body = getBodyID();
	ASSERT(body, "Cannot create motor joints: No physics body available");
	
	amotor = dJointCreateAMotor(physicsEngine->getWorld(), 0);
	dJointAttach(amotor, body, 0);
	dJointSetAMotorNumAxes(amotor, 3);
	dJointSetAMotorAxis (amotor, 0, 1, 1, 0, 0);
	dJointSetAMotorAxis (amotor, 1, 1, 0, 1, 0);
	dJointSetAMotorAxis (amotor, 2, 1, 0, 0, 1);
	dJointSetAMotorParam(amotor, dParamFMax,  maxForce);
	dJointSetAMotorParam(amotor, dParamFMax2, maxForce);
	dJointSetAMotorParam(amotor, dParamFMax3, maxForce);
	dJointSetAMotorParam(amotor, dParamVel,  0);
	dJointSetAMotorParam(amotor, dParamVel2, 0);
	dJointSetAMotorParam(amotor, dParamVel3, 0);
	
	lmotor = dJointCreateLMotor(physicsEngine->getWorld(), 0);
	dJointAttach(lmotor, body, 0);
	dJointSetLMotorNumAxes(lmotor, 2);
	dJointSetLMotorAxis (lmotor, 0, 1, 1, 0, 0);
	dJointSetLMotorAxis (lmotor, 1, 1, 0, 1, 0);
	dJointSetLMotorParam(lmotor, dParamFMax,  maxForce);
	dJointSetLMotorParam(lmotor, dParamFMax2, maxForce);
	dJointSetLMotorParam(lmotor, dParamVel,  0);
	dJointSetLMotorParam(lmotor, dParamVel2, 0);
}

void ComponentMovement::handleEventDeathBehaviorUpdate(const EventDeathBehaviorUpdate *event) {
	ASSERT(event, "event is null");
	lastReportedDeathBehavior = event->deathBehavior;
}

dBodyID ComponentMovement::getBodyID() {
	ScopedEventHandler *parentScope = getParentScopePtr();
	ASSERT(parentScope, "parentScope is null");
	
	Actor *parentActor = dynamic_cast<Actor*>(parentScope);
	ASSERT(parentActor, "parentActor is null");
	
	shared_ptr<Component> component = parentActor->getComponent("Physics");
	ASSERT(component, "component is null");
	
	shared_ptr<ComponentPhysics> rigidBody = dynamic_pointer_cast<ComponentPhysics>(component);
	ASSERT(rigidBody, "rigidBody is null");
	
	dBodyID body = rigidBody->getBody();
	ASSERT(body, "No physics body is available?");
	
	return body;
}
