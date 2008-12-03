#ifndef ACTOR_H
#define ACTOR_H

#include "PropertyBag.h"
#include "ScopedEventHandler.h"
#include "Component.h"
#include "ComponentDataSet.h"

#include "ActionDeleteActor.h"

class ActorSet;
class World;

/**
A basic object in the game world.  An Actor has interfaces to present a
visual representation of itself and to interact on a very basic level with
the other objects in the game world.  For example, the Actor class provide
a very simple collision detection mechanism and an event handling mechanism
that exists for a more advanced system to build upon.
*/
class Actor : public ScopedEventHandler
{
public:
	/**
	Constructor
	@param uid The unique ID of the object
	*/
	Actor(ActorID uid);

	/** Destructor */
	virtual ~Actor();

	/** Set everything to defaults */
	virtual void reset();

	/**
	Loads the object state
	@param componentsData Component name and associated data for each component
	@param initialPosition Initial position of the object
	@param initialVelocity Initial velocity of the object
	@param world World where the object is located
	*/
	virtual void load(const ComponentDataSet &componentsData,
	                  const vec3 &initialPosition,
	                  const vec3 &initialVelocity,
	                  World*const world);

	/**
	Updates the object without displaying it
	@param deltaTime milliseconds since the last tick
	*/
	virtual void update(float deltaTime);
	
	/** Determines whether the actor has a particular component or not */
	bool hasComponent(const string &name) const;

	/** Get actor component by component type */
	shared_ptr<Component> getComponent(const string &comType);

	/** Get actor component by component type */
	shared_ptr<const Component> getComponent(const string &comType) const;

	/** Indicates that the manager may delete us */
	inline bool isZombie() const
	{
		return zombie;
	}

private:
	/** Receive command to delete the actor entirely */
	void handleActionDeleteActor(const ActionDeleteActor *action);

	/** Broadcast the initial position and velocity of the actor */
	void broadcastInitialPosition(const vec3 &initialPosition,
	                              const vec3 &initialVelocity);

private:
	typedef vector<shared_ptr<Component> > ComponentsList;

	/** Game object's components */
	ComponentsList components;

	/** Indicates that the manager may delete us */
	bool zombie;
};

// Garbage Collected pointer to an actor
typedef shared_ptr<Actor> ActorPtr;

#endif
