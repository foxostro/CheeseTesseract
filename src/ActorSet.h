#ifndef _ACTOR_SET_H_
#define _ACTOR_SET_H_

#include "Actor.h"

#include "ScopedEventHandler.h"

#include "ActionDebugEnable.h"
#include "ActionDebugDisable.h"

// class prototypes
class Frustum;
class World;

/**
Collection of actors.

Actors may be referenced in several sets, and will be
deleted automatically when no longer referenced.  Actors within an actor set
are GARBAGE COLLECTED and should not be manually deleted.  To explicitly
request that an actor be destroyed, pass it the message MessageZombifyActor
or manually set the zombie flag within the actor object.  All actor sets take
care to remove references to zombie actors at each tick, and when garbage
collection is explicitly run via the reapZombieActors method.

It should be noted that actors must appear in the actor set of the game world
in order to recognized as being part of the game world.

Any ActorSet set may create actors, but those new actors will belong only to
the set that originally created the actor until explicitly added to another
set.  This is useful for maintaining multiple game worlds at the same time
and for constructs such as actor inventories.
*/
class ActorSet : public ScopedEventHandler
{
private:
	typedef map<ActorID, ActorPtr> MapHandleToObject;

public:
	typedef MapHandleToObject::iterator iterator;
	typedef MapHandleToObject::const_iterator const_iterator;

private:
	/** Tuple containing component data, initial position, and velocity */
	typedef tuple<ComponentDataSet, vec3, vec3> SpawnRequest;

	/** Generates unique names for objects */
	static UniqueIdFactory<ActorID> nameFactory;

private:
	MapHandleToObject actors;
	queue<SpawnRequest> spawnRequests;
	World *world;
	bool displayDebugRendering;

public:
	virtual string getTypeString() const { return "ActorSet"; }

	/** Creates an empty set */
	ActorSet();

	/**
	Creates a set of objects from an XML data source
	@param xml The XML data source
	@param zone The home zone of the object
	*/
	ActorSet(const PropertyBag &data, World *world)
	{
		ASSERT(world!=0, "zone was NULL");
		clear();
		load(data, world);
	}

	inline iterator begin()
	{
		return actors.begin();
	}
	
	inline const_iterator begin() const
	{
		return actors.begin();
	}

	inline iterator end()
	{
		return actors.end();
	}

	inline const_iterator end() const
	{
		return actors.end();
	}

	inline size_t size() const
	{
		return actors.size();
	}

	/**
	Resets the ActorSet to an empty set, eliminating all references to
	contained actors
	*/
	void clear();

	/**
	Resets the ActorSet to an empty set and explicitly requests that each
	object previously within the set be destroyed by ALL other sets that also
	referenced the actors.
	*/
	void destroy();

	/**
	Creates a set of objects from an XML data source
	@param xml The XML data source
	@param world The home world of the object
	*/
	void load(const PropertyBag &xml, World *world);

	/**
	Determines whether an object is a member of the ActorSet
	@param id The GUID of the object
	@return true if the object is contained within the set, false otherwise
	*/
	bool isMember(ActorID id) const;

	/**
	Creates an Actor and adds it to this set.
	@return Tuple containing the unique identifier to the new object and
	a pointer to the object itself
	*/
	tuple<ActorID, ActorPtr> create();

	/** Adds the actor to this set */
	void addReference(ActorPtr actor);

	/**
	Requests that an actor be created at the next tick.
	@param data Component layout and data for the object
	@param position Initial position of the object
	@param position Initial velocity of the object
	@return reference to the object
	*/
	void spawn(const ComponentDataSet &data,
	           const vec3 &position,
			   const vec3 &velocity);

	/**
	Gets an actor from the set
	@param uid The GUID of the actor to retrieve
	@return reference of the actor
	*/
	ActorPtr get(ActorID uid);

	/**
	Gets an actor from the set
	@param uid The GUID of the actor to retrieve
	@return reference of the actor
	*/
	const ActorPtr get(ActorID uid) const;

	/**
	Ticks each actor within the set
	@param deltaTime The milliseconds between now and the last tick
	*/
	void update(float deltaTime);

	/** Deletes zombie actors */
	void reapZombieActors();

private:
	void handleActionDebugEnable(const ActionDebugEnable *)
	{
		displayDebugRendering = true;
	}

	void handleActionDebugDisable(const ActionDebugDisable *)
	{
		displayDebugRendering = false;
	}

	/** Spawns an object right now */
	void _spawn(const SpawnRequest &data);
};

#endif
