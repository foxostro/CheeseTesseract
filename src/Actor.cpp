#include "stdafx.h"
#include "World.h"
#include "Actor.h"
#include "ActorSet.h"
#include "Component.h"

#include "MessagePassWorld.h"

#include "EventDeclareInitialPosition.h"

Actor::Actor(const ActorID uid)
		: ScopedEventHandler(uid, 0) {
	reset();
	REGISTER_HANDLER(Actor::handleActionDeleteActor);
}

Actor::~Actor() {
	reset();
}

void Actor::reset() {
	zombie = false;
	components.clear();
	ScopedEventHandler::clear();
}

void Actor::update(float milliseconds) {
	for (ComponentsList::const_iterator i = components.begin();
	     i != components.end(); ++i) {
		(*i)->update(milliseconds);
	}
}

void Actor::load(const ComponentDataSet &componentsData,
                 const vec3 &initialPosition,
                 const vec3 &initialVelocity,
                 World*const world) {
	reset();
	
	// List of components along-side the data they will be loading
	vector<tuple<shared_ptr<Component>, PropertyBag> > componentsWithData;
	
	// Create all components before loading component data
	for (ComponentDataSet::const_iterator i = componentsData.begin();
	     i != componentsData.end(); ++i) {
		shared_ptr<Component> component
		= Component::createComponent(i->first,
		                             ScopedEventHandler::genName(),
		                             this);
		                             
		if (component) {
			components.push_back(component);
			componentsWithData.push_back(make_tuple(component, i->second));
			ScopedEventHandler::registerSubscriber(component.get());
		}
	}
	
	{
		MessagePassWorld m(world);
		recvMessage(&m);
	}
	
	// Have components load data
	for (vector<tuple<shared_ptr<Component>, PropertyBag> >::iterator
	     i = componentsWithData.begin();
	     i != componentsWithData.end(); ++i) {
		shared_ptr<Component> &component = i->get<0>();
		const PropertyBag &data = i->get<1>();
		component->load(data);
	}
	
	// Declare the object's initial position
	broadcastInitialPosition(initialPosition, initialVelocity);
	
}

shared_ptr<const Component> Actor::getComponent(const string &comType) const {
	for (ComponentsList::const_iterator i = components.begin();
	     i != components.end(); ++i) {
		const shared_ptr<const Component> &component = (*i);
		const string typeString = component->getTypeString();
		
		if (comType == typeString) {
			return component;
		}
	}
	
	FAIL("Could not find component of specified type: " + comType);
	return shared_ptr<const Component>();
}

bool Actor::hasComponent(const string &comType) const {
	for (ComponentsList::const_iterator i = components.begin();
	     i != components.end(); ++i) {
		if (comType == (*i)->getTypeString()) {
			return true;
		}
	}
	
	return false;
}

shared_ptr<Component> Actor::getComponent(const string &comType) {
	for (ComponentsList::iterator i = components.begin();
	     i != components.end(); ++i) {
		shared_ptr<Component> &component = (*i);
		const string typeString = component->getTypeString();
		
		if (comType == typeString) {
			return component;
		}
	}
	
	FAIL("Could not find component of specified type: " + comType);
	return shared_ptr<Component>();
}

void Actor::handleActionDeleteActor(const ActionDeleteActor *message) {
	if (message->id == getUID()) {
		zombie = true;
	}
}

void Actor::broadcastInitialPosition( const vec3 &initialPosition, const vec3 &initialVelocity ) {
	EventDeclareInitialPosition m(initialPosition, initialVelocity);
	recvMessage(&m);
}
