#ifndef _GAME_STATE_RUN_H_
#define _GAME_STATE_RUN_H_

#include "SDLinput.h"
#include "Camera.h"
#include "Dimmer.h"
#include "GameStateMachine.h"
#include "SDLinput.h"
#include "World.h"

/** Normal running state of the game */
class GameStateRun : public GameStateMachine::GameState {
public:
	/**
	Constructor
	@param frameTimer Calculates FPS and frame-rate
	@param kernel Game task kernel
	@param world Game world
	@param gui Widget layer
	*/
	GameStateRun(UID uid,
	             ScopedEventHandler *parent,
	             shared_ptr<Timer> &frameTimer,
	             Kernel &kernel,
	             shared_ptr<World> &world);
	             
	/**
	Updates the task every tick as long as the task has not been frozen
	@param deltaTime The seconds since the last tick
	*/
	virtual void update(float deltaTime);
	
	/**
	Take whatever the screen dimness is and transition from that to
	the value intended for this particular state.
	*/
	virtual void onEnter();
	
private:
	void handleInputKeyPress(const InputKeyPress *input);
	
private:
	shared_ptr<World> world;
};

#endif
