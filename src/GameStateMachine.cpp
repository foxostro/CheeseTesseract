#include "stdafx.h"
#include "Kernel.h"
#include "LinearInterpolator.h"
#include "GameStateRun.h"
#include "GameStateMachine.h"

GameStateMachine::GameState::GameState(UID uid,
									   ScopedEventHandler *parent,
									   Kernel &_kernel,
		                               float _transitionTime,
		                               float _dimness)
: State(uid, parent),
  kernel(_kernel),
  transitionTime(_transitionTime),
  dimness(_dimness)
{
	/* Do Nothing */
}

void GameStateMachine::GameState::onEnter()
{
	if(transitionTime>0.0f)
	{
		const float start = Dimmer::alphaBlur;
		const float end = dimness;

		kernel.addTask(new LinearInterpolator(&Dimmer::alphaBlur,
			                                  start,
			                                  end,
			                                  transitionTime));
	}
	else
	{
		Dimmer::alphaBlur = dimness;
	}
}

GameStateMachine::GameStateMachine(UID _uid,
								   ScopedEventHandler *parentScope,
								   shared_ptr<Timer> &_frameTimer,
								   Kernel &_kernel,
								   shared_ptr<World> &_world)
: StateMachine(_uid, parentScope),
  RUN(NULL_STATE),
  world(_world)
{
	REGISTER_HANDLER(GameStateMachine::handleEventGameOver);

	// generate state names
	RUN = genNewStateName();
	TRACE("Generated state names (locally)");

	// Create states
	GameStateRun *stateRun = new GameStateRun(genName(),
	                                          this,
											  _frameTimer,
											  _kernel,
											  world);
	registerSubscriber(stateRun);
	TRACE("Created game state objects");

	// bind game state names
	bindState(RUN, stateRun);
	TRACE("Bound game state names");

	// Enter the game world immediately
	world->loadFromFile(FileName("Data/maps/level1.xml"));
	world->playersEnter(1);
	pushState(RUN);
	TRACE("Entered initial state");
}

void GameStateMachine::handleEventGameOver(const EventGameOver *)
{
	while(popState());
	//pushState(MENU);
	//pushState(GAMEOVER);
}
