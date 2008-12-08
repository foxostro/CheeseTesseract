#ifndef ACTION_QUEUE_RENDER_INSTANCE_H
#define ACTION_QUEUE_RENDER_INSTANCE_H

#include "EventHandler.h"
#include "RenderInstance.h"

/**
Queue geometry chunk within the renderer.
The queue is flushed each tick after chunks are rendered.
*/
class ActionQueueRenderInstance : public Action {
public:
	ActionQueueRenderInstance(const RenderInstance &_instance) {
		instance = _instance;
	}
	
public:
	RenderInstance instance;
};

/**
Queue a TreeLib tree for render.
The queue is flushed each tick after trees are rendered.
*/
class ActionQueueTreeForRender : public Action {
public:
	/**
	@param _position Position of the tree in world-space
	@param _seed Random number used to identify a unique tree from the tree
	pool to use for this tree instance. This allows the tree instance
	to remain the same over several frames without having explicit knowledge
	of the exact tree used or the contents of the tree pool.  It is assumed
	that the domain of the random number generator is as wide as the size of
	the tree pool.  It is best to make it as wide as possible, really.
	*/
	ActionQueueTreeForRender(const vec3 &_position, int _seed) {
		position = _position;
		seed = _seed;
	}
	
public:
	vec3 position;
	int seed;
};

#endif
