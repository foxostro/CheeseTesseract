#include "stdafx.h"
#include "ScopedEventHandler.h"

UniqueIdFactory<UID> ScopedEventHandler::nameFactory(1000);

ScopedEventHandler::ScopedEventHandler()
		: ScopedEventHandlerSubscriber(genName(), 0) {
	// Do nothing
}

ScopedEventHandler::ScopedEventHandler(UID uid, ScopedEventHandler *parentBlackBoard)
		: ScopedEventHandlerSubscriber(uid, parentBlackBoard) {
	// Do nothing
}

void ScopedEventHandler::clear() {
	subscribersByUid.clear();
}

UID ScopedEventHandler::registerSubscriber(ScopedEventHandlerSubscriber* subscriber) {
	ASSERT(subscriber, "Subscriber is null");
	
	UID uid = subscriber->getUID();
	
	if (!isSubscriber(uid)) {
		subscribersByUid.insert(make_pair(uid, subscriber));
	}
	
	return uid;
}

ScopedEventHandlerSubscriber* ScopedEventHandler::removeSubscriber(UID uid) {
	ASSERT(isSubscriber(uid), "Can not remove subscriber!");
	MapByUID::iterator i = subscribersByUid.find(uid);
	ScopedEventHandlerSubscriber *r = (i->second);
	subscribersByUid.erase(i);
	return r;
}

void ScopedEventHandler::recvMessage(const Message *message) {
	// pass to any registered handlers at this scope level
	ScopedEventHandlerSubscriber::recvMessage(message);
	
	// propagate to all subscribers
	for (MapByUID::iterator i = subscribersByUid.begin();
	     i != subscribersByUid.end(); ++i) {
		(i->second)->recvMessage(message);
	}
}

bool ScopedEventHandler::isSubscriber(UID uid) const {
	MapByUID::const_iterator i = subscribersByUid.find(uid);
	
	return(i != subscribersByUid.end());
}
