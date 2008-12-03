#ifndef _GAME_STATE_MACHINE_H_
#define _GAME_STATE_MACHINE_H_

#include "StateMachine.h"
#include "Dimmer.h"
#include "ScopedEventHandler.h"
#include "EventGameOver.h"

class Timer;
class GameStateRun;
class Kernel;
class TextureFactory;
class GraphicsDevice;
class World;

/** State machine for game state and transitions */
class GameStateMachine : public StateMachine
{
public:
	/** Game State in a game state machine */
	class GameState : public State
	{
	public:
		/**
		@param kernel Game task kernel
		@param stateRun Access to the game world
		@param transitionTime Time for the dimmer to transition
		@param dimness Intensity of the screen while in this game state
		*/
		GameState(UID uid,
		          ScopedEventHandler *parent,
		          Kernel &kernel,
				  float transitionTime = 666.7f,
				  float dimness = 0.8);

		/**
		Take whatever the screen dimness is and transition from that to
		the value intended for this particular state.
		*/
		virtual void onEnter();

	private:
		/** Do not call the copy constructor */
		GameState(const GameState &o);

#if defined(_MSC_VER)
		/*
		On warning level 4, Microsoft Visual Studio complains that the
		assignment operator for the GameState class cannot be generated.
		However, it should be noted that the assignment operator should NEVER
		be present as the GameState class is an abstract class!	So, let's give
		an inaccessible assignment operator to MSVC to placate the beast.

		Note:
		GameState is abstract because StateMachine::State (the parent class)
		is abstract and GameState does not fully implement the required
		interface.
		*/

		/** Do not call the assignment operator */
		GameState operator=(const GameState &rh);
#endif

	private:
		/** Game task kernel */
		Kernel &kernel;

		/**
		Time for the dimmer to transition when a state transition occurs.
		Specify a zero or negative value to disable the dimmer phase.
		*/
		float transitionTime;

		/** Intensity of the screen while in this game state */
		float dimness;

		/** Screen Dimmer */
		Dimmer dimmer;
	};

	/** Creates game states */
	GameStateMachine(UID uid,
	                 ScopedEventHandler *parentBlackBoard,
	                 shared_ptr<Timer> &frameTimer,
					 Kernel &kernel,
					 shared_ptr<World> &world);

	/** Game state names */
	STATE RUN;

private:
	/** Do not call the assignment operator */
	GameStateMachine operator=(const GameStateMachine &rh);

	/** Do not call the copy constructor */
	GameStateMachine(const GameStateMachine &o);

	void handleEventGameOver(const EventGameOver *event);

private:
	shared_ptr<World> world;
};

#endif
