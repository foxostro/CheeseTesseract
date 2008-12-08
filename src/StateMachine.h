#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

#include "UniqueIdFactory.h"
#include "ScopedEventHandler.h"

/**
State machine for game state and transitions.
StateMachine may only be instantiated through a child class.
*/
class StateMachine : public ScopedEventHandler {
protected:
	/** Default constructor */
	StateMachine(UID uid, ScopedEventHandler *parent);
	
public:
	/** State in a state machine */
	class State : public ScopedEventHandlerSubscriber {
	public:
		virtual ~State() { /* Do Nothing */ }
		
		/**
		@param uid UID to identify this blackboard subscriber
		@param parentScope Scope that will be sending messages to this
		subscriber.  Note that messages may come from
		external sources *anyway*
		*/
		State(UID uid, ScopedEventHandler *parentScope)
				: ScopedEventHandlerSubscriber(uid, parentScope) {
			/* Do Nothing */
		}
		
		/**
		Updates the task every tick as long as the task has not been frozen
		@param deltaTime The seconds since the last tick
		*/
		virtual void update(float deltaTime)=0;
		
		/** Called when the state is entered */
		virtual void onEnter() { /* Do Nothing */ }
		
		/** Called when the state is exited */
		virtual void onExit() { /* Do Nothing */ }
		
	private:
		/** Do not call the copy constructor */
		State(const State &o);
	};
	
	/** UID type for game states */
	typedef int STATE;
	
	/** Identifies the initial "null" state of the state machine */
	STATE NULL_STATE;
	
	/** Destructor */
	virtual ~StateMachine();
	
	/**
	Gets another new state UID (name)
	@return New state UID (name)
	*/
	STATE genNewStateName();
	
	/**
	Binds state behavior to a state name
	@param name Token to identify the state
	@param state Object to encapsulate state behavior
	*/
	void bindState(STATE name, State *state);
	
	/**
	Gets the current state's name
	@return State name
	*/
	STATE getState() const;
	
	/**
	Transitions to a new state
	@param newState The new state's name
	*/
	void pushState(STATE newState);
	
	/**
	Transitions to the previous state.
	If there was no previous state, then no state transition occurs.
	@return true if a state transition did occur
	*/
	bool popState();
	
	/** Transitions to the previous state */
	void popStateOrRevert();
	
	/**
	Updates the current state
	@param deltaTime seconds since the last update
	*/
	void update(float deltaTime);
	
	/** Destroys all state objects */
	virtual void destroy();
	
protected:
	const State* getStatePtr() const;
	
private:
	/** Do not call the assignment operator */
	StateMachine operator=(const StateMachine &rh);
	
	/** Do not call the copy constructor */
	StateMachine(const StateMachine &o);
	
private:
	/** State Name  ->  State Object */
	typedef map<STATE,State*> StateMap;
	
	/** Available states */
	StateMap states;
	
	/**
	Stack containing state transition history as states very often want to
	simply revert the previous state and don't need detailed knowledge of
	that state's operations.
	*/
	stack<STATE> stateStack;
	
	STATE previousState; /** The exact previous state that we were in */
	
	/** Assigns UIDs / names to new states */
	UniqueIdFactory<STATE> stateNameFactory;
};

#endif
