#include "stdafx.h"
#include "GraphicsDevice.h"
#include "Frustum.h"
#include "ComponentPhysics.h"
#include "ComponentPhysicsBody.h"
#include "ActorSet.h"

#include "EventCollisionOccurred.h"

#include "ActionDebugEnable.h"
#include "ActionDebugDisable.h"
#include "ActionDeleteActor.h"

UniqueIdFactory<ActorID> ActorSet::nameFactory(100);

void ActorSet::clear() {
	ScopedEventHandler::clear();
	actors.clear();
	displayDebugRendering = false;
}

void ActorSet::destroy() {
	// delete all actors
	while (!actors.empty()) {
		MapHandleToObject::iterator iter = actors.begin();
		pair<ActorID, ActorPtr> p = *iter;
		ActorID id = p.first;
		ActorPtr actor = p.second;
		
		// Actor should actually be deleted now
		{
			ActionDeleteActor m(id);
			actor->recvAction(&m);
		}
		
		removeSubscriber(id);
		actors.erase(iter);
	}
	
	displayDebugRendering = false;
}

void ActorSet::spawn(const ComponentDataSet &data,
                     const vec3 &position,
                     const vec3 &velocity) {
	const SpawnRequest r = make_tuple(data, position, velocity);
	spawnRequests.push(r);
}

void ActorSet::_spawn(const SpawnRequest &spawnData) {
	const ComponentDataSet &data = spawnData.get<0>();
	const vec3 &position = spawnData.get<1>();
	const vec3 &velocity = spawnData.get<2>();
	
	const tuple<ActorID, ActorPtr> t = create();
	const ActorPtr object = t.get<1>();
	
	object->load(data, position, velocity, world);
	
	// initially set debug display state
	if (displayDebugRendering) {
		ActionDebugEnable m;
		object->recvAction(&m);
	} else {
		ActionDebugDisable m;
		object->recvAction(&m);
	}
}

tuple<ActorID, ActorPtr> ActorSet::create() {
	ActorID uid = nameFactory.getUid();
	
	ActorPtr actor = ActorPtr(new Actor(uid));
	
	actors.insert(make_pair(uid, actor));
	
	// Actor begins receiving messages from this set
	registerSubscriber(actor.get());
	actor->setParentScope(this);
	
	return make_tuple(uid, actor);
}

const ActorPtr ActorSet::get(ActorID id) const {
	ASSERT(isMember(id), "Not a member");
	return actors.find(id)->second;
}

ActorPtr ActorSet::get(ActorID id) {
	ASSERT(isMember(id), "Not a member");
	return actors.find(id)->second;
}

void ActorSet::update(float deltaTime) {
	for (iterator i = begin(); i!=end(); ++i) {
		ActorPtr actor = i->second;
		
		if (actor) {
			actor->update(deltaTime);
		}
	}
	
	while (!spawnRequests.empty()) {
		_spawn(spawnRequests.front());
		spawnRequests.pop();
	}
	
	reapZombieActors();
}

void ActorSet::reapZombieActors() {
	// delete zombie actors
	MapHandleToObject::iterator iter = actors.begin();
	MapHandleToObject::iterator nextIter;
	
	while (iter != actors.end()) {
		// Get an iterator to the next element (or the end of the map)
		nextIter = iter;
		nextIter++;
		
		pair<ActorID, ActorPtr> p = *iter;
		ActorID id = p.first;
		ActorPtr actor = p.second;
		
		if (actor && actor->isZombie()) {
			removeSubscriber(id);
			actors.erase(id);
		}
		
		iter = nextIter;
	}
}

void ActorSet::load(const PropertyBag &objects, World *_world) {
	ASSERT(_world!=0, "world was null");
	world = _world;
	
	for (size_t i=0, n=objects.getNumInstances("object"); i<n; ++i) {
		const tuple<ActorID, ActorPtr> t = create();
		const ActorPtr object = t.get<1>();
		
		const PropertyBag decl = objects.getBag("object", i);
		const FileName templateFile = decl.getFileName("template");
		const vec3 initialPosition = decl.getVec3("position");
		const PropertyBag templateData = PropertyBag::fromFile(templateFile);
		const PropertyBag base = templateData.getBag("components");
		
		ComponentDataSet s = ComponentDataSet::load(base, decl);
		
		object->load(s, initialPosition, vec3(0,0,0), world);
		object->setParentScope(this);
	}
}

bool ActorSet::isMember(ActorID id) const {
	const_iterator i = actors.find(id);
	return (id!=INVALID_ID) && (i != actors.end());
}

ActorSet::ActorSet()
		:world(0) {
	REGISTER_HANDLER(ActorSet::handleActionDebugEnable);
	REGISTER_HANDLER(ActorSet::handleActionDebugDisable);
	clear();
}

void ActorSet::addReference(ActorPtr actor) {
	ASSERT(actor, "Null parameter: actor");
	actors.insert(make_pair(actor->getUID(), actor));
}
