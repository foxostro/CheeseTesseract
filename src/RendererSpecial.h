#ifndef _RENDERER_SPECIAL_H_
#define _RENDERER_SPECIAL_H_

#include "ScopedEventHandlerSubscriber.h"

/**
Interface for a special renderer subsystem.
Reserved for rendering objects that can not be neatly reduced to a
vertex stream and a shader program.
*/
class RendererSpecial : public ScopedEventHandlerSubscriber
{
public:
	virtual string getTypeString() const { return "RendererSpecial"; }

	/** Destructor */
	virtual ~RendererSpecial()
	{
		/* Do Nothing */
	}

	/** Constructor */
	RendererSpecial(UID uid, ScopedEventHandler *parentScope)
	: ScopedEventHandlerSubscriber(uid, parentScope)
	{
		/* Do Nothing */
	}

	/** Renders objects controlled by the subsystem */
	virtual void render() = 0;
};

#endif
