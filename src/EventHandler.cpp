#include "stdafx.h"
#include "EventHandler.h"

MessageHandler::~MessageHandler()
{
	Handlers::iterator it = handlers.begin();
	while(it != handlers.end())
	{
		delete it->second;
		++it;
	}
	handlers.clear();
}

void MessageHandler::recvMessage(const Message *message)
{
	ASSERT(message, "Null parameter: message");

	Handlers::iterator it = handlers.find(TypeInfo(typeid(*message)));
	if(it != handlers.end())
	{
		(it->second)->exec(message);
	}
}
