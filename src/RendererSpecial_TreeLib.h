#ifndef _RENDERER_SPECIAL_TREELIB_H_
#define _RENDERER_SPECIAL_TREELIB_H_

#include "RendererSpecial.h"

/**
Interface to the TreeLib library which can draw trees using a neat
and simple interface.  Unfortunately, we do not have direct access
to the tree geometry using this library and so cannot use the normal
render pipeline.
*/
class RendererSpecial_TreeLib : public ScopedEventHandlerSubscriber {
public:
	virtual string getTypeString() const {
		return "RendererSpecial_TreeLib";
	}
	
	/** Destructor */
	virtual ~RendererSpecial_TreeLib() {
		/* Do Nothing */
	}
	
	/** Constructor */
	RendererSpecial_TreeLib(UID uid, ScopedEventHandler *parentScope)
			: ScopedEventHandlerSubscriber(uid, parentScope) {
		/* Do Nothing */
	}
	
	/** Renders objects controlled by the subsystem */
	virtual void render() {
	
	}
};

#endif
