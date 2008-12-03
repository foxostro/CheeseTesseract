#ifndef ACTION_PLAY_SOUND_H
#define ACTION_PLAY_SOUND_H

#include "EventHandler.h"

/** Message to request that a sound be played */
class ActionPlaySound : public Action
{
public:
	ActionPlaySound(const FileName &_sound)
	{
		sound = _sound;
	}

public:
	FileName sound;
};

#endif
