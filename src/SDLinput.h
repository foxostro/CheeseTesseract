#ifndef _SDL_INPUT_H_
#define _SDL_INPUT_H_

#include "UniqueIdFactory.h"
#include "myassert.h"
#include "xpad.h"
#include "ScopedEventHandlerSubscriber.h"

class Application;

#define MAX_JOYSTICK_BUTTONS (64)
#define MAX_JOYSTICK_AXII (64)
#define MAX_JOYSTICKS (64)

/** State of a single joystick */
class Joystick {
public:
	SDL_Joystick *handle;
	int which; /** Joystick index */
	bool btnState[MAX_JOYSTICK_BUTTONS];
	
	Joystick(SDL_Joystick *_handle, int _which)
			: handle(_handle),
			which(_which) {
		for (size_t i=0; i<sizeof(btnState); ++i) {
			btnState[i] = false;
		}
	}
};

class InputKeyDown : public Event {
public:
	InputKeyDown(SDLKey _key) {
		key = _key;
	}
	
	SDLKey key;
};

class InputKeyUp : public Event {
public:
	InputKeyUp(SDLKey _key) {
		key = _key;
	}
	
	SDLKey key;
};

class InputKeyPress : public Event {
public:
	InputKeyPress(SDLKey _key) {
		key = _key;
	}
	
	SDLKey key;
};

class InputMouseMove : public Event {
public:
	InputMouseMove(const ivec2 &_pos, const ivec2 &_delta) {
		pos = _pos;
		delta = _delta;
	}
	
	ivec2 pos;
	ivec2 delta;
};

class InputMouseDownLeft : public Event {
public:
	InputMouseDownLeft(const ivec2 &_pos) {
		pos = _pos;
	}
	
	ivec2 pos;
};

class InputMouseDownRight : public Event {
public:
	InputMouseDownRight(const ivec2 &_pos) {
		pos = _pos;
	}
	
	ivec2 pos;
};

class InputMouseUpLeft : public Event {
public:
	InputMouseUpLeft(const ivec2 &_pos) {
		pos = _pos;
	}
	
	ivec2 pos;
};

class InputMouseUpRight : public Event {
public:
	InputMouseUpRight(const ivec2 &_pos) {
		pos = _pos;
	}
	
	ivec2 pos;
};

class InputJoyAxisMotion : public Event {
public:
	InputJoyAxisMotion(int _joystick, int _axis, int _value) {
		joystick = _joystick;
		axis = _axis;
		value = _value;
	}
	
	int joystick;
	int axis;
	int value;
};

class InputJoyButtonDown : public Event {
public:
	InputJoyButtonDown(int _joystick, int _button) {
		joystick = _joystick;
		button = _button;
	}
	
	int joystick;
	int button;
};

class InputJoyButtonUp : public Event {
public:
	InputJoyButtonUp(int _joystick, int _button) {
		joystick = _joystick;
		button = _button;
	}
	
	int joystick;
	int button;
};

class InputJoyButtonPress : public Event {
public:
	InputJoyButtonPress(int _joystick, int _button) {
		joystick = _joystick;
		button = _button;
	}
	
	int joystick;
	int button;
};

/**
Handles SDL input events and passes them onto registered input handlers.

Only one SDLinput object should be in action at a time, as it empties out the
input event queue. However, a separate SDLinput object should be used for each
separate game state as event handlers are usually only applicable to some
specific context.

For example, the player movement callbacks should not be handled while in the
game menu, and the menu navigation callbacks should not be called when outside
the game menu.
*/
class SDLinput : public ScopedEventHandlerSubscriber {
public:
	/**
	Constructor
	@param uid UID to identify this blackboard subscriber
	@param parentScope Scope that will be sending messages to this
	subscriber.  Note that messages may come from
	external sources *anyway*
	*/
	SDLinput(UID uid,
	         ScopedEventHandler *parentScope,
	         vector<Joystick> &joysticks);
	         
	/** Polls for new events in input */
	void poll();
	
private:
	/** Do not call the assignment operator */
	SDLinput operator=(const SDLinput &rh);
	
	/** Do not call the copy constructor */
	SDLinput(const SDLinput &world);
	
	void requestApplicationQuit();
	
	void broadcastKeyDownEvents(SDLKey key);
	void broadcastKeyUpEvents(SDLKey key);
	void broadcastKeyPressEvents(SDLKey key);
	void broadcastMouseMoveEvents(int x, int y, int xrel, int yrel);
	void broadcastMouseDownLeftEvents(int x, int y);
	void broadcastMouseDownRightEvents(int x, int y);
	void broadcastMouseUpLeftEvents(int x, int y);
	void broadcastMouseUpRightEvents(int x, int y);
	void broadcastJoyButtonDownEvents(int joystick, int button);
	void broadcastJoyButtonUpEvents(int joystick, int button);
	void broadcastJoyButtonPressEvents(int joystick, int button);
	void broadcastJoyAxisMotionEvents(int joystick, int axis, int value);
	
private:
	/** Array of all keys where true indicates that the key is down */
	bool keyState[SDLK_LAST];
	
	/** Joystick threshold / dead-zone */
	int threshold;
	
	/** Open joysticks */
	vector<Joystick> &joysticks;
};

#endif
