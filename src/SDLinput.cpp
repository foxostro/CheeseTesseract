#include "stdafx.h"
#include "SDLinput.h"
#include "ActionApplicationQuit.h"

SDLinput::SDLinput(UID uid,
                   ScopedEventHandler *parentScope,
                   vector<Joystick> &_joysticks)
		: ScopedEventHandlerSubscriber(uid, parentScope),
		threshold(10000),
		joysticks(_joysticks) {
	for (size_t s = 0; s < sizeof(keyState); ++s) {
		keyState[s] = false;
	}
	
//	SDL_WM_GrabInput(SDL_GRAB_ON);
}

void SDLinput::broadcastKeyDownEvents(SDLKey key) {
	InputKeyDown event(key);
	sendGlobalEvent(&event);
}

void SDLinput::broadcastKeyUpEvents(SDLKey key) {
	InputKeyUp event(key);
	sendGlobalEvent(&event);
}

void SDLinput::broadcastKeyPressEvents(SDLKey key) {
	InputKeyPress event(key);
	sendGlobalEvent(&event);
}

void SDLinput::broadcastMouseMoveEvents(int x, int y, int xrel, int yrel) {
	InputMouseMove event(ivec2(x, y), ivec2(xrel, yrel));
	sendGlobalEvent(&event);
}

void SDLinput::broadcastMouseDownLeftEvents(int x, int y) {
	InputMouseDownLeft event(ivec2(x, y));
	sendGlobalEvent(&event);
}

void SDLinput::broadcastMouseDownRightEvents(int x, int y) {
	InputMouseDownRight event(ivec2(x, y));
	sendGlobalEvent(&event);
}

void SDLinput::broadcastMouseUpLeftEvents(int x, int y) {
	InputMouseUpLeft event(ivec2(x, y));
	sendGlobalEvent(&event);
}

void SDLinput::broadcastMouseUpRightEvents(int x, int y) {
	InputMouseUpRight event(ivec2(x, y));
	sendGlobalEvent(&event);
}

void SDLinput::broadcastJoyAxisMotionEvents(int joystick, int axis, int value) {
	InputJoyAxisMotion event(joystick, axis, value);
	sendGlobalEvent(&event);
}

void SDLinput::broadcastJoyButtonDownEvents(int joystick, int button) {
	InputJoyButtonDown event(joystick, button);
	sendGlobalEvent(&event);
}

void SDLinput::broadcastJoyButtonUpEvents(int joystick, int button) {
	InputJoyButtonUp event(joystick, button);
	sendGlobalEvent(&event);
}

void SDLinput::broadcastJoyButtonPressEvents(int joystick, int button) {
	InputJoyButtonPress event(joystick, button);
	sendGlobalEvent(&event);
}

void SDLinput::poll() {
	SDL_Event event;
	event.type = SDL_NOEVENT;
	
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			keyState[event.key.keysym.sym] = event.key.state == SDL_PRESSED;
			broadcastKeyPressEvents(event.key.keysym.sym);
			break;
			
		case SDL_KEYUP:
			keyState[event.key.keysym.sym] = event.key.state == SDL_PRESSED;
			broadcastKeyUpEvents(event.key.keysym.sym);
			break;
			
		case SDL_MOUSEMOTION:
			broadcastMouseMoveEvents(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
			break;
			
		case SDL_MOUSEBUTTONDOWN:
			switch (event.button.button) {
			case SDL_BUTTON_LEFT:
				broadcastMouseDownLeftEvents(event.motion.x, event.motion.y);
				break;
				
			case SDL_BUTTON_RIGHT:
				broadcastMouseDownRightEvents(event.motion.x, event.motion.y);
				break;
			}
			break;
			
		case SDL_MOUSEBUTTONUP:
			switch (event.button.button) {
			case SDL_BUTTON_LEFT:
				broadcastMouseUpLeftEvents(event.motion.x, event.motion.y);
				break;
				
			case SDL_BUTTON_RIGHT:
				broadcastMouseUpRightEvents(event.motion.x, event.motion.y);
				break;
			}
			break;
			
		case SDL_JOYBUTTONDOWN:
			joysticks[event.jbutton.which].btnState[event.jbutton.button] = event.jbutton.state==SDL_PRESSED;
			broadcastJoyButtonPressEvents(event.jbutton.which, event.jbutton.button);
			break;
			
		case SDL_JOYBUTTONUP:
			joysticks[event.jbutton.which].btnState[event.jbutton.button] = event.jbutton.state==SDL_PRESSED;
			broadcastJoyButtonUpEvents(event.jbutton.which, event.jbutton.button);
			break;
			
		case SDL_QUIT:
			requestApplicationQuit();
			
			break;
		}
	}
	
	// Poll for joystick axis events
	for (vector<Joystick>::iterator i=joysticks.begin();
	     i!=joysticks.end();
	     ++i) {
		const Joystick &joystick = *i;
		
		if (joystick.handle) {
			const int numAxes = SDL_JoystickNumAxes(joystick.handle);
			for (int axis=0; axis<numAxes; ++axis) {
				Sint16 value = SDL_JoystickGetAxis(joystick.handle, axis);
				
				if (abs(value) > threshold) {
					broadcastJoyAxisMotionEvents(joystick.which, axis, value);
				}
			}
		}
	}
	
	// Fire off events for key holds
	for (size_t key=0; key<sizeof(keyState); ++key) {
		if (keyState[key]) {
			broadcastKeyDownEvents((SDLKey)key);
		}
	}
	
	// Fire off events for joystick button holds
	for (vector<Joystick>::iterator i=joysticks.begin();
	     i!=joysticks.end();
	     ++i) {
		const Joystick &joystick = *i;
		
		for (int i=0; i<MAX_JOYSTICK_BUTTONS; ++i) {
			if (joystick.btnState[i]) {
				broadcastJoyButtonDownEvents(joystick.which, i);
			}
		}
	}
}

void SDLinput::requestApplicationQuit() {
	ActionApplicationQuit action;
	sendGlobalAction(&action);
}
