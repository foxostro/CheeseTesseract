#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <map>
#include "TypeInfo.h"

class Message
{
public:
	virtual ~Message() { /* Do nothing */ }
};

class Event : public Message
{
public:
	virtual ~Event() { /* Do nothing */ }
};

class Action : public Message
{
public:
	virtual ~Action() { /* Do nothing */ }
};

class HandlerFunctionBase
{
public:
	virtual ~HandlerFunctionBase() { /* Do nothing */ }
	void exec(const Message* message) { call(message); }

private:
	virtual void call(const Message *) = 0;
};


template <class T, class MessageT>
class MemberFunctionHandler : public HandlerFunctionBase
{
public:
	typedef void (T::*MemberFunc)(MessageT*);
	MemberFunctionHandler(T* instance, MemberFunc memFn)
	: _instance(instance), _function(memFn)
	{ /* Do Nothing */ }

	void call(const Message* message)
	{
		(_instance->*_function)(static_cast<MessageT*>(message));
	}

private:
	T* _instance;
	MemberFunc _function;
};


class MessageHandler
{
public:
	virtual ~MessageHandler();

	virtual void recvMessage(const Message *message);

	inline void recvEvent(const Event *event)
	{
		recvMessage(event);
	}

	inline void recvAction(const Action *action)
	{
		recvMessage(action);
	}

	template <class T, class MessageT> inline
	void registerHandler(T *obj, void (T::*memFn)(MessageT*))
	{
		handlers[TypeInfo(typeid(MessageT))]= new MemberFunctionHandler<T, MessageT>(obj, memFn);
	}

private:
	typedef std::map<TypeInfo, HandlerFunctionBase*> Handlers;
	Handlers handlers;
};

#endif
