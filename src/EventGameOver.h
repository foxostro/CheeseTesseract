#ifndef EVENT_GAME_OVER_H
#define EVENT_GAME_OVER_H

#include "EventHandler.h"

/** Message to declare GAME OVER and that the players have lost the game */
class EventGameOver : public Event {
public:
	EventGameOver() { /* Do Nothing */ }
};

#endif
