#ifndef EVENT_CHARACTER_HAS_BEEN_RESURRECTED_H
#define EVENT_CHARACTER_HAS_BEEN_RESURRECTED_H

#include "EventHandler.h"

/**
Message to notify that the specified character was dead, but now is in an
alive state again.
*/
class EventCharacterRevived : public Event {
public:
	EventCharacterRevived() { /* Do Nothing */ }
};

#endif
