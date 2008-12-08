#ifndef COMPONENT_MOVEMENT_H
#define COMPONENT_MOVEMENT_H

#include "PropertyBag.h"
#include "Component.h"
#include "Actions.h"
#include "DeathBehavior.h"

#include "MessagePassWorld.h"

#include "ActionPhysicsDisable.h"
#include "ActionPhysicsEnable.h"
#include "ActionPerformAction.h"
#include "ActionSetCharacterFaceAngle.h"
#include "ActionTurn.h"

#include "EventCharacterRevived.h"
#include "EventCharacterHasDied.h"
#include "EventDeathBehaviorUpdate.h"

// prototypes
class PhysicsEngine;
class World;

/**
Allows movement within the game world.
Requires a ComponentPhysics to be attached to the object
*/
class ComponentMovement : public Component {
public:
	virtual string getTypeString() const {
		return "Movement";
	}
	
	ComponentMovement(UID _uid, ScopedEventHandler *_blackBoard);
	
	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);
	
	/** Declares the position and orientation for the frame */
	virtual void update(float milliseconds);
	
	/** Resets all object members to defaults */
	virtual void resetMembers();
	
	/** Gets the angle (radians) that the character is currently facing */
	float getFacingAngle() const {
		return facingAngle;
	}
	
	/** Gets the direction to attack towards */
	vec3 getFacingVector() const {
		return vec3(getDirectionVector(getFacingAngle()), 0.0f);
	}
	
	/** Sets the top speed for this actor */
	inline void setTopSpeed(float speed) {
		topSpeed = speed;
	}
	
	/** Gets the top speed for this actor */
	inline float getTopSpeed() {
		return topSpeed;
	}
	
private:
	void handleMessagePassWorld(const MessagePassWorld *message);
	
	void handleActionPhysicsDisable(const ActionPhysicsDisable *action);
	void handleActionPhysicsEnable(const ActionPhysicsEnable *action);
	void handleActionPerformAction(const ActionPerformAction *action);
	void handleActionSetCharacterFaceAngle(const ActionSetCharacterFaceAngle *action);
	void handleActionTurn(const ActionTurn *action);
	
	void handleEventCharacterRevived(const EventCharacterRevived *event);
	void handleEventCharacterHasDied(const EventCharacterHasDied *event);
	void handleEventDeathBehaviorUpdate(const EventDeathBehaviorUpdate *event);
	
	/** Create motor joints to facilitate  the character movement now that */
	void createMotorJoints();
	
	/** Attack action (e.g. fire gun) */
	void endChargeUp();
	
	/** Weapon / ability charge action */
	void chargeUp();
	
	/** Fires the weapon without any sort of charging non-sense */
	void attackOnce();
	
	/** Commit suicide (perhaps for testing purposes) */
	void suicide();
	
	/** Applies some action to the object */
	void applyAction(CharacterAction action);
	
	/** Walk a step in some direction */
	void walk(const vec2 &direction, float speed=1.0f);
	
	/** Determines the most appropriate walk animation */
	string determineCurrentAnim() const;
	
	dBodyID getBodyID();
	
	template<typename TYPE> static inline
	TYPE sign(TYPE x) {
		static const TYPE zero(0);
		static const TYPE one(1);
		static const TYPE neg(-1);
		return x==zero ? zero : ( x>zero ? one : neg );
	}
	
	/**
	Gets the unit vector pointing towards a particular angle
	@param angle radians
	*/
	static vec2 getDirectionVector(float angle) {
		return vec2(cosf(angle), sinf(angle));
	}
	
	/**
	Turns the character to face a particular direction
	@param angle radians
	*/
	void turnToFace(float angle) {
		if (dead) return;
		facingAngle = angle;
	}
	
	/**
	Turns the character by some amount
	@param dAngle radians
	*/
	void turn(float dAngle);
	
	/**
	Turns the character to face a particular direction
	@param direction Vector points in the direction to face
	*/
	void turnToFace(const vec2 &direction) {
		turnToFace(getAngle(direction, vec2(1,0)));
	}
	
private:
	/** Gets the death behavior of the character */
	DeathBehavior getDeathBehavior() const;
	
private:
	/** Game world */
	World *world;
	
	/** Connection to the physics engine */
	shared_ptr<PhysicsEngine> physicsEngine;
	
	/** Physics joint used for character locomotion */
	dJointID amotor;
	
	/** Physics joint used for character locomotion */
	dJointID lmotor;
	
	/** Max force that can be exerted by the movement motors */
	dReal maxForce;
	
	/** Top speed of the character (meters/second)*/
	float topSpeed;
	
	/** Last action taken */
	CharacterAction lastAction;
	
	/** Most recent velocity of the character */
	vec3 velocity;
	
	/** Angle that the character is rotated about the Z-axis */
	float facingAngle;
	
	/** indicates that the player is alive (false) or dead (true) */
	bool dead;
	
	/** Indicates that physics have been disabled */
	bool disablePhysics;
	
	DeathBehavior lastReportedDeathBehavior;
};

#endif
