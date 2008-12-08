#ifndef SCOPED_EVENT_HANDLER_H
#define SCOPED_EVENT_HANDLER_H

#include "UniqueIdFactory.h"
#include "ScopedEventHandlerSubscriber.h"

/**
System to allow anonymous, asynchronous communication between objects using
a message-passing event system
*/
class ScopedEventHandler : public ScopedEventHandlerSubscriber {
public:
	virtual string getTypeString() const {
		return "ScopedEventHandler";
	}
	
	/**
	Default Constructor
	Constructs a blackboard that is not a subscriber to a larger network
	*/
	ScopedEventHandler();
	
	/**
	Constructs a blackboard that is also a subscriber to a larger network
	@param uid UID to identify this blackboard subscriber
	@param parentBlackBoard Black board that will be sending messages to this
	subscriber.  Note that messages may come from
	external sources *anyway*
	*/
	ScopedEventHandler(UID uid, ScopedEventHandler *parentBlackBoard);
	
	/** Clears all subscribers */
	void clear();
	
	/**
	Adds a new BlackBoardSubscriber to the list of those that receive messages.
	@param subscriber New blackboard subscriber
	*/
	UID registerSubscriber(ScopedEventHandlerSubscriber* subscriber);
	
	/**
	Removes a subscriber
	@param uid UID of the subscriber to remove
	@return Pointer to the removed subscriber
	*/
	ScopedEventHandlerSubscriber* removeSubscriber(UID uid);
	
	/** Determines whether the blackboard has a particular subscriber */
	bool isSubscriber(UID uid) const;
	
	/**
	Receives a message of some kind.
	Relays a message to all blackboard subscribers.
	@param message Some message
	*/
	virtual void recvMessage(const Message *message);
	
	/** Generate a UID for some subscriber */
	inline static UID genName() {
		return nameFactory.getUid();
	}
	
private:
	static UniqueIdFactory<UID> nameFactory;
	typedef map<UID, ScopedEventHandlerSubscriber*> MapByUID;
	MapByUID subscribersByUid;
};

#endif
