#include "stdafx.h"
#include "color.h"
#include "World.h"
#include "Actor.h"
#include "ComponentPhysics.h"
#include "ComponentBigSwitchDevice.h"
#include "ActionPlaySound.h"
#include "EventSwitchToggled.h"

ComponentBigSwitchDevice::
ComponentBigSwitchDevice(UID uid, ScopedEventHandler *parentScope)
		: Component(uid, parentScope) {
	resetMembers();
	
	REGISTER_HANDLER(ComponentBigSwitchDevice::handleEventHeightUpdate);
	REGISTER_HANDLER(ComponentBigSwitchDevice::handleEventOrientationUpdate);
	REGISTER_HANDLER(ComponentBigSwitchDevice::handleEventPositionUpdate);
	REGISTER_HANDLER(ComponentBigSwitchDevice::handleEventUsesObject);
	REGISTER_HANDLER(ComponentBigSwitchDevice::handleActionEnableModelHighlight);
	REGISTER_HANDLER(ComponentBigSwitchDevice::handleActionDisableModelHighlight);
	
	stateHandler[STATE_A]     = bind(&ComponentBigSwitchDevice::update_PositionA, this);
	stateHandler[STATE_B]     = bind(&ComponentBigSwitchDevice::update_PositionA, this);
	stateHandler[TRANSITION_BA] = bind(&ComponentBigSwitchDevice::update_TransitionToA, this);
	stateHandler[TRANSITION_AB] = bind(&ComponentBigSwitchDevice::update_TransitionToB, this);
}

void ComponentBigSwitchDevice::handleEventOrientationUpdate(const EventOrientationUpdate *message) {
	lastReportedOrientation = message->orientation;
}

void ComponentBigSwitchDevice::handleEventPositionUpdate(const EventPositionUpdate *message) {
	lastReportedPosition = message->position;
}

void ComponentBigSwitchDevice::handleEventHeightUpdate( const EventHeightUpdate *event ) {
	lastReportedHeight = event->height;
}

void ComponentBigSwitchDevice::handleEventUsesObject(const EventUsesObject *) {
	switch (state) {
	case STATE_A:
		enterState(TRANSITION_AB);
		break;
	case STATE_B:
		enterState(TRANSITION_BA);
		break;
	default:
		break; // Do Nothing
	};
}

void ComponentBigSwitchDevice::resetMembers() {
	lastReportedPosition.zero();
	lastReportedOrientation.identity();
	lastReportedHeight = 1.0f;
	positionA = (float)(M_PI / 4.0);
	positionB = (float)(M_PI / -4.0);
	timeToTransition = 1500.0f;
	angle = positionA;
	categoryID = 0;
	highlight = false;
	highlightIntensity = 1.0f;
	hightlightIntensityRate = 400.0f;
	colora = yellow;
	colorb = white;
	
	enterState(STATE_A);
}

void ComponentBigSwitchDevice::draw() const {
	CHECK_GL_ERROR();
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLineWidth(2.0f);
	
	glPushMatrix();
	
	mat4 transformation(lastReportedPosition,
	                    lastReportedOrientation.getAxisX(),
	                    lastReportedOrientation.getAxisY(),
	                    lastReportedOrientation.getAxisZ());
	                    
	                    
	glMultMatrixf(transformation);
	
	drawLever();
	drawBase(5, 0.5f, 0.001f);
	
	glPopMatrix();
	glPopAttrib();
	
	CHECK_GL_ERROR();
}

void ComponentBigSwitchDevice::drawBase(const int numIter,
                                        const float radius,
                                        const float eps) const {
	for (int i=0; i<numIter; ++i) {
		drawCircle(radius * (1.0f - i/(numIter-1.0f)), eps + (0.05f * i));
	}
}

void ComponentBigSwitchDevice::drawLever() const {
	CHECK_GL_ERROR();
	glBegin(GL_LINES);
	glColor4fv(getLeverColor());
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(lastReportedHeight*sinf(angle), 0.0f,
	           lastReportedHeight*cosf(angle));
	glEnd();
	CHECK_GL_ERROR();
}

color ComponentBigSwitchDevice::getLeverColor() const {
	switch (state) {
	case STATE_A:
		return(red);
		break;
		
	case STATE_B:
		return(green);
		break;
		
	case TRANSITION_BA:
	case TRANSITION_AB:
		return(cyan);
		break;
		
	default:
		return(yellow);
		break;
	};
}

void ComponentBigSwitchDevice::drawCircle(float radius, float z) const {
	CHECK_GL_ERROR();
	glPushMatrix();
	glScalef(radius, radius, 1.0f);
	glBegin(GL_LINE_LOOP);
	glColor4fv(highlight
	           ? color::interpolate(colora, colorb, highlightIntensity)
	           : cornflower);
	for (int i=0; i<16; ++i) {
		float angle = (float)(i * M_PI / 8.0);
		glVertex3f(sinf(angle), cosf(angle), z);
	}
	glEnd();
	glPopMatrix();
	CHECK_GL_ERROR();
}

void ComponentBigSwitchDevice::load(const PropertyBag &data) {
	resetMembers();
	
	categoryID = data.getInt("categoryID");
	timeToTransition = data.getFloat("timeToTransition");
	
	enterState(STATE_A);
}

void ComponentBigSwitchDevice::update(float milliseconds) {
	timeInTransition += milliseconds;
	stateHandler[state]();
	
	// update highlighting intensity
	highlightIntensity += milliseconds/hightlightIntensityRate;
	highlightIntensity = (highlightIntensity>1.0f) ? 0.0f
	                     : highlightIntensity;
}

void ComponentBigSwitchDevice::handleActionEnableModelHighlight(const ActionEnableModelHighlight *message) {
	highlight = true;
	colora = message->colora;
	colorb = message->colorb;
}

void ComponentBigSwitchDevice::handleActionDisableModelHighlight(const ActionDisableModelHighlight *) {
	highlight = false;
}

void ComponentBigSwitchDevice::update_PositionA() {
	/* Do Nothing */
}

void ComponentBigSwitchDevice::update_PositionB() {
	/* Do Nothing */
}

void ComponentBigSwitchDevice::update_TransitionToA() {
	const float bias = timeInTransition / timeToTransition;
	
	angle = positionB + bias*(positionA - positionB);
	
	if (timeInTransition > timeToTransition) {
		enterState(STATE_A);
	}
}

void ComponentBigSwitchDevice::update_TransitionToB() {
	const float bias = timeInTransition / timeToTransition;
	
	angle = positionA + bias*(positionB - positionA);
	
	if (timeInTransition > timeToTransition) {
		enterState(STATE_B);
	}
}

void ComponentBigSwitchDevice::enterState(SwitchState newState) {
	timeInTransition = 0.0f;
	state = newState;
	
	if (state == STATE_A || state == STATE_B) {
		EventSwitchToggled m(categoryID, INVALID_ID);
		sendGlobalEvent(&m);
	}
}
