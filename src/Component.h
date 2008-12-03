#ifndef COMPONENT_H
#define COMPONENT_H

#include "PropertyBag.h"
#include "ScopedEventHandler.h"
#include "ActionDebugEnable.h"
#include "ActionDebugDisable.h"

class Actor;

class Component : public ScopedEventHandlerSubscriber
{
public:
	virtual string getTypeString() const { return "Component"; }

	virtual ~Component() { /* Do Nothing */ }

	/**
	Constructor
	@param uid GUID of the component
	@param blackBoard Serves as the component's message relay
	*/
	Component(UID uid, ScopedEventHandler *parentScope);

	/** Resets all object members to defaults */
	virtual void resetMembers();

	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data) = 0;

	/**
	Updates component each tick
	@param milliseconds Time since the last tick
	*/
	virtual void update(float milliseconds) = 0;

	/** Creates a component given the component name */
	static shared_ptr<Component> createComponent(const string &name,
	                                             UID uid,
									             ScopedEventHandler *scope);

protected:
	inline ActorID getActorID() const
	{
		return getParentScope().getUID();
	}

	/** Indicates that any debug data should be displayed by the component */
	bool isDebugDisplayEnabled() const
	{
		return displayDebugData;
	}

private:
	void handleActionDebugDisable(const ActionDebugDisable *message);
	void handleActionDebugEnable(const ActionDebugEnable *message);

	template<typename COMPONENT> static inline
	shared_ptr<Component> newComponent(UID uid, ScopedEventHandler *bb)
	{
		return shared_ptr<Component>(new COMPONENT(uid, bb));
	}

private:
	typedef map<string, function<shared_ptr<Component> (UID, ScopedEventHandler*)> > AllocMap;

	/** Indicates that any debug data should be displayed by the component */
	bool displayDebugData;
};

#endif
