#ifndef EVENT_PLAYER_NUMBER_SET_H
#define EVENT_PLAYER_NUMBER_SET_H

#include "EventHandler.h"

/** Message notifying the player actor that the player number was set */
class EventPlayerNumberSet : public Event
{
public:
	EventPlayerNumberSet(int _playerNumber)
	{
		playerNumber = _playerNumber;
	}

public:
	int playerNumber;
};

#endif
