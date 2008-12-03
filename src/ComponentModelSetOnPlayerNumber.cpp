#include "stdafx.h"
#include "World.h"
#include "SDLinput.h"
#include "ComponentModelSetOnPlayerNumber.h"

#include "ActionSetModel.h"

ComponentModelSetOnPlayerNumber::
ComponentModelSetOnPlayerNumber(UID uid, ScopedEventHandler *parentScope)
: Component(uid, parentScope)
{
	REGISTER_HANDLER(ComponentModelSetOnPlayerNumber::handleEventPlayerNumberSet);
}

void ComponentModelSetOnPlayerNumber::handleEventPlayerNumberSet( const EventPlayerNumberSet *event )
{
	const int NUM_MODELS = 5;

	FileName playerModels[NUM_MODELS] = // todo: load this list from data?
	{
		FileName("data/models/hero-green/hero-green.md3xml"),
		FileName("data/models/hero-red/hero-red.md3xml"),
		FileName("data/models/hero-blue/hero-blue.md3xml"),
		FileName("data/models/hero-yellow/hero-yellow.md3xml"),
		FileName("data/models/theryn/theryn.md3xml")
	};

	ActionSetModel m(playerModels[event->playerNumber % NUM_MODELS]);

	sendAction(&m);
}
