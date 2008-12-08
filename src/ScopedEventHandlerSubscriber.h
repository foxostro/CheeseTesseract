#ifndef SCOPED_EVENT_HANDLER_SUBSCRIBER_H
#define SCOPED_EVENT_HANDLER_SUBSCRIBER_H

#include "EventHandler.h"

class ScopedEventHandler; // forward declaration

#define REGISTER_HANDLER(FUNC) registerHandler(this, &FUNC);

/** Base type for an object that send/receives blackboard messages */
class ScopedEventHandlerSubscriber : public MessageHandler {
public:
	virtual string getTypeString() const {
		return "ScopedEventHandlerSubscriber";
	}
	
	/** Destructor */
	virtual ~ScopedEventHandlerSubscriber();
	
	/**
	@param uid UID to identify this blackboard subscriber
	@param parentScope Scope that will be sending messages to this
	                   subscriber.  Note that messages may come from
	                   external sources *anyway*
	*/
	ScopedEventHandlerSubscriber(UID uid, ScopedEventHandler *parentScope);
	
	/** Gets the BlackBoardUID of the BlackBoardSubscriber */
	inline UID getUID() const {
		return uid;
	}
	
	/** Sets the parent blackboard */
	void setParentScope(ScopedEventHandler *parent);
	
	/** Relays a message to the global (topmost) event scope */
	void sendGlobalEvent(const Event *event);
	
	/**
	Passes a message up to the containing scope
	Relays a message to all blackboard subscribers.
	@param message Some message
	*/
	void sendEvent(const Event *event);
	
	/** Relays a message to the global (topmost) event scope */
	void sendGlobalAction(const Action *action);
	
	/**
	Passes a message up to the containing scope
	Relays a message to all blackboard subscribers.
	@param message Some message
	*/
	void sendAction(const Action *action);
	
protected:
	inline ScopedEventHandler& getParentScope() const {
		ASSERT(parentScope, "parentScope was null");
		return *parentScope;
	}
	
	inline ScopedEventHandler* getParentScopePtr() const {
		return parentScope;
	}
	
private:
	UID uid;
	ScopedEventHandler *parentScope;
};

#endif
