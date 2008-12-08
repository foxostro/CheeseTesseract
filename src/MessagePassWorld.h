#ifndef MESSAGE_PASS_WORLD_H
#define MESSAGE_PASS_WORLD_H

#include "ScopedEventHandler.h"

class World; // forward declaration

/** Passes the game world to actors that require access to it. */
class MessagePassWorld : public Message {
public:
	MessagePassWorld(World *_world) {
		world = _world;
	}
	
public:
	World *world;
};

#endif
