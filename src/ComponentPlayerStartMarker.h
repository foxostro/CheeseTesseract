#ifndef COMPONENT_PLAYER_START_MARKER_H
#define COMPONENT_PLAYER_START_MARKER_H

#include "PropertyBag.h"
#include "Component.h"

/**
Indicates that the actor marks the position where a party of players may
be spawned into the level.
*/
class ComponentPlayerStartMarker : public Component {
public:
	/** Constructor */
	ComponentPlayerStartMarker(UID uid, ScopedEventHandler *blackBoard);
	
	/** Returns type of component in string */
	virtual string getTypeString() const {
		return "PlayerStartMarker";
	}
	
public:
	virtual void resetMembers()            { /* Do Nothing */ }
	virtual void load(const PropertyBag &) { /* Do Nothing */ }
	virtual void draw() const              { /* Do Nothing */ }
	virtual void update(float)             { /* Do Nothing */ }
};

#endif
