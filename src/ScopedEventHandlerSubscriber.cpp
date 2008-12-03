#include "stdafx.h"
#include "ScopedEventHandlerSubscriber.h"
#include "ScopedEventHandler.h"

ScopedEventHandlerSubscriber::~ScopedEventHandlerSubscriber()
{
	/* Do nothing */
}

ScopedEventHandlerSubscriber::
ScopedEventHandlerSubscriber(UID _uid, ScopedEventHandler *_scope)
: uid(_uid),
  parentScope(_scope)
{
	/* Do nothing */
}

void ScopedEventHandlerSubscriber::setParentScope(ScopedEventHandler *p)
{
	ASSERT(p, "Parameter \"p\" was null");

	if(parentScope)
	{
		parentScope->removeSubscriber(getUID());
	}

	parentScope = p;

	parentScope->registerSubscriber(this);
}

void ScopedEventHandlerSubscriber::sendGlobalEvent(const Event *event)
{
	ASSERT(event, "Null param");

	if(parentScope)
	{
		parentScope->sendGlobalEvent(event);
	}
	else
	{
		recvEvent(event);
	}
}

void ScopedEventHandlerSubscriber::sendEvent(const Event *event)
{
	getParentScope().recvEvent(event);
}

void ScopedEventHandlerSubscriber::sendGlobalAction(const Action *action)
{
	ASSERT(action, "Null param");

	if(parentScope)
	{
		parentScope->sendGlobalAction(action);
	}
	else
	{
		recvAction(action);
	}
}

void ScopedEventHandlerSubscriber::sendAction(const Action *action)
{
	getParentScope().recvAction(action);
}
