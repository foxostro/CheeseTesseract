#include "stdafx.h"
#include "StateMachine.h"

StateMachine::StateMachine(UID uid, ScopedEventHandler *parent)
: ScopedEventHandler(uid, parent),
  NULL_STATE(0),
  previousState(NULL_STATE),
  stateNameFactory(1000)
{}

StateMachine::~StateMachine(void)
{
	destroy();
}

StateMachine::STATE StateMachine::genNewStateName(void)
{
	return stateNameFactory.getUid();
}

void StateMachine::bindState(STATE name, State *state)
{
	states[name] = state;
}

StateMachine::STATE StateMachine::getState(void) const
{
	if(stateStack.empty())
	{
		return NULL_STATE;
	}
	else
	{
		return stateStack.top();
	}
}

void StateMachine::pushState(STATE newState)
{
	ASSERT(newState != NULL_STATE, "Cannot transition to null state");

	if(!stateStack.empty())
	{
		states[stateStack.top()]->onExit();
		previousState = stateStack.top(); // save the previous state
	}
	else
	{
		previousState = newState;
	}

	stateStack.push(newState);

	states[newState]->onEnter();
}

bool StateMachine::popState()
{
	// cannot pop the stack if that would empty it
	if(stateStack.size() > 1)
	{
		states[stateStack.top()]->onExit();

		previousState = stateStack.top(); // save the previous state

		stateStack.pop();

		states[stateStack.top()]->onEnter();

		return true;
	}

	return false;
}

void StateMachine::popStateOrRevert()
{
	if(!popState())
	{
		pushState(previousState);
	}
}

void StateMachine::update(float deltaTime)
{
	ASSERT(!stateStack.empty(), "cannot update state when the current state is NULL");

	State *state = states[getState()];

	ASSERT(0 != state, "state object was null");

	state->update(deltaTime);
}

void StateMachine::destroy()
{
	for(StateMap::iterator iter = states.begin(); iter!=states.end(); ++iter)
	{
		delete(iter->second);
	}
	states.clear();

	while(stateStack.empty())
		stateStack.pop();

	TRACE("Deleted game states");
}

const StateMachine::State* StateMachine::getStatePtr() const
{
	return states.find(getState())->second;
}
