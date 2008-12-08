#include "stdafx.h"
#include "ComponentUserControllable.h"

#include "ActionSetCharacterFaceAngle.h"
#include "ActionUseObject.h"
#include "ActionPerformAction.h"
#include "ActionTurn.h"

#include "SDLinput.h"
#include "EventPlayerNumberSet.h"

ComponentUserControllable::
ComponentUserControllable(UID uid, ScopedEventHandler *parentScope)
		: Component(uid, parentScope) {
	w = s = a = d = false;
	
	REGISTER_HANDLER(ComponentUserControllable::handleEventPlayerNumberSet);
	REGISTER_HANDLER(ComponentUserControllable::handleActionDeleteActor);
	REGISTER_HANDLER(ComponentUserControllable::handleEventPositionUpdate);
	REGISTER_HANDLER(ComponentUserControllable::handleInputKeyPress);
	REGISTER_HANDLER(ComponentUserControllable::handleInputKeyDown);
	REGISTER_HANDLER(ComponentUserControllable::handleInputKeyUp);
	REGISTER_HANDLER(ComponentUserControllable::handleInputMouseMove);
	REGISTER_HANDLER(ComponentUserControllable::handleInputMouseDownRight);
	REGISTER_HANDLER(ComponentUserControllable::handleInputMouseDownLeft);
	REGISTER_HANDLER(ComponentUserControllable::handleInputMouseUpLeft);
}

void ComponentUserControllable::resetMembers() {
	playerNumber = -1;
	lastReportedPosition.zero();
	mouseSensitivity = 100.0f;
}

void ComponentUserControllable::update(float) {
	CharacterAction action = getAction(w, a, s, d);
	requestPerformAction(action);
	
	// Reset key states for the next tick
	w = s = a = d = false;
}

void ComponentUserControllable::handleActionDeleteActor( const ActionDeleteActor *actor ) {
	ActorID my_uid = getActorID();
	
	if (actor->id == my_uid) {
		resetMembers();
	}
}

void ComponentUserControllable::handleEventPlayerNumberSet( const EventPlayerNumberSet *event ) {
	playerNumber = event->playerNumber;
}

void ComponentUserControllable::onKeyPressUse() {
	UID actorUID = getActorID();
	ActionUseObject m(actorUID);
	sendGlobalAction(&m);
}

void ComponentUserControllable::onKeyPressAttack() {
	ActionPerformAction m(EndChargeUp);
	sendAction(&m);
}

void ComponentUserControllable::onKeyPressChargeUp() {
	ActionPerformAction m(BeginChargeUp);
	sendAction(&m);
}

void ComponentUserControllable::handleEventPositionUpdate( const EventPositionUpdate *event ) {
	lastReportedPosition = event->position;
}

void ComponentUserControllable::load(const PropertyBag &data) {
	mouseSensitivity = data.getFloat("mouseSensitivity");
}

void ComponentUserControllable::onKeyPressSuicide() {
	ActionPerformAction m(Suicide);
	sendAction(&m);
}

CharacterAction ComponentUserControllable::getAction(bool w, bool a,
  bool s, bool d) {
	CharacterAction action;
	
	if (w && d)
		action = StepForwardRight;
	else if (w && a)
		action = StepForwardLeft;
	else if (s && d)
		action = StepBackWardRight;
	else if (s && a)
		action = StepBackWardLeft;
	else if (w)
		action = StepForward;
	else if (s)
		action = StepBackWard;
	else if (a)
		action = StepLeft;
	else if (d)
		action = StepRight;
	else
		action = Stand;
		
	return action;
}

void ComponentUserControllable::requestPerformAction(CharacterAction action) {
	ActionPerformAction m(action);
	sendAction(&m);
}

void ComponentUserControllable::handleInputKeyPress( const InputKeyPress *input ) {
	switch (input->key) {
	case SDLK_f:
		onKeyPressSuicide();
		break;
	case SDLK_e:
		onKeyPressUse();
		break;
	case SDLK_SPACE:
		onKeyPressChargeUp();
		break;
	};
}

void ComponentUserControllable::handleInputKeyDown( const InputKeyDown *input ) {
	switch (input->key) {
	case SDLK_w:
		w = true;
		break;
	case SDLK_s:
		s = true;
		break;
	case SDLK_a:
		a = true;
		break;
	case SDLK_d:
		d = true;
		break;
	};
}

void ComponentUserControllable::handleInputKeyUp( const InputKeyUp *input ) {
	switch (input->key) {
	case SDLK_SPACE:
		onKeyPressAttack();
		break;
	};
}

void ComponentUserControllable::handleInputMouseMove( const InputMouseMove *input ) {
	float dFacingAngle = (input->delta).x / mouseSensitivity;
	ActionTurn m(-dFacingAngle);
	sendAction(&m);
}

void ComponentUserControllable::handleInputMouseDownRight(const InputMouseDownRight *) {
	onKeyPressUse();
}

void ComponentUserControllable::handleInputMouseDownLeft(const InputMouseDownLeft *) {
	onKeyPressChargeUp();
}

void ComponentUserControllable::handleInputMouseUpLeft(const InputMouseUpLeft *) {
	onKeyPressAttack();
}
