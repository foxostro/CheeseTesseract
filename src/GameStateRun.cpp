#include "stdafx.h"
#include "Application.h"
#include "Camera.h"
#include "ScreenShot.h"
#include "GameStateRun.h"
#include "ActionApplicationQuit.h"

GameStateRun::GameStateRun(UID uid,
                           ScopedEventHandler *parent,
                           shared_ptr<Timer> &frameTimer,
                           Kernel &_kernel,
                           shared_ptr<World> &_world)
		: GameState(uid, parent, _kernel, 666.7f, 0.0f),
		world(_world) {
	//REGISTER_HANDLER(GameStateRun::handleInputKeyPress);
}

void GameStateRun::handleInputKeyPress(const InputKeyPress *) {
	/*
	TODO: [ESC] key goes back to the menu
	TODO: [P] key pauses the game
	*/
}

void GameStateRun::update(float deltaTime) {
	world->update(deltaTime);
}

void GameStateRun::onEnter() {
	GameStateMachine::GameState::onEnter();
}
