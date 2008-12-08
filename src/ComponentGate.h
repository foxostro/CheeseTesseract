#ifndef COMPONENT_GATE_H
#define COMPONENT_GATE_H

#include "PropertyBag.h"

#include "EventUsesObject.h"
#include "EventDeclareInitialPosition.h"

class World;

/** When used, the object will move between two specified positions */
class ComponentGate : public Component {
public:
	virtual string getTypeString() const {
		return "Gate";
	}
	
	ComponentGate(UID uid, ScopedEventHandler *blackBoard);
	
	/** Resets all object members to defaults */
	virtual void resetMembers();
	
	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);
	
	/** Draws the object */
	virtual void draw() const;
	
	/** Updates the object */
	virtual void update(float milliseconds);
	
private:
	enum GateState {
		STATE_A,
		STATE_B,
		STATE_TRANSITION_AB,
		STATE_TRANSITION_BA
	};
	
private:
	void handleEventUsesObject(const EventUsesObject *event);
	void handleEventDeclareInitialPosition(const EventDeclareInitialPosition *event);
	
	void enterState(GateState newState);
	void update_transition(float milliseconds,
	                       float timeInTransition,
	                       float timeToTransition,
	                       GateState endState,
	                       const vec3 &positionStart,
	                       const vec3 &positionEnd);
	                       
	void setPosition(const vec3 &position);
	
private:
	/** Current state of the gate*/
	GateState state;
	
	/** Position for state A, relative to initial position */
	vec3 positionA;
	
	/** Position for state B, relative to initial position */
	vec3 positionB;
	
	/** Initial position, read from ComponentPhysics on init */
	vec3 initialPosition;
	
	/** Milliseconds to transition from state A to state B */
	float timeToTransitionAB;
	
	/** Milliseconds to transition from state B to state A */
	float timeToTransitionBA;
	
	/** Milliseconds into the current transition */
	float timeInState;
};

#endif
