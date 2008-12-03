#ifndef COMPONENT_USER_CONTROLLABLE_H
#define COMPONENT_USER_CONTROLLABLE_H

#include "PropertyBag.h"
#include "Component.h"
#include "SDLinput.h"
#include "Actions.h"
#include "EventPlayerNumberSet.h"
#include "ActionDeleteActor.h"
#include "EventPositionUpdate.h"

/**
The object can be controlled by the user.  This may be aggregated with other
components to make up a Player Character object.

And here are some tasty definitions.  These terms are often used
interchangeably, so I'm trying to pin down their exact meaning for the
purposes of this game:
    User   - Person physically playing the game
    Player - User's avatar, also known as the "Player Character"
*/
class ComponentUserControllable : public Component
{
public:
	virtual string getTypeString() const { return "UserControllable"; }

	ComponentUserControllable(UID _uid, ScopedEventHandler *_blackBoard);

	/** Does nothing */
	virtual void load(const PropertyBag &data);

	/** Updates component each tick */
	virtual void update(float milliseconds);

	/** Resets all object members to defaults */
	virtual void resetMembers();

private:
	/**
	Given the state of a set of keys, determine the appropriate action.
	Other actions may be possible, or may be considered more valid, given
	other key states that are unknown to this method.

	Key states are defined as being equal to boolean 'true' if the key is
	depressed and equal to 'false' otherwise.
	@param w State of the key bound to directional movement in the forward
	         direction, relative to the current actor's orientation.
	@param a State of the key bound to directional movement in the left
	         direction, relative to the current actor's orientation.
	@param s State of the key bound to directional movement in the backward
	         direction, relative to the current actor's orientation.
	@param d State of the key bound to directional movement in the right
	         direction, relative to the current actor's orientation.
	@return  Action to take, having considered the states of the input keys.
	*/
	static CharacterAction getAction(bool w, bool a, bool s, bool d);

	/** Sets the player number */
	void handleEventPlayerNumberSet(const EventPlayerNumberSet *event);

	/** Called when the actor is about to be removed from the game world */
	void handleActionDeleteActor(const ActionDeleteActor *actor);

	/** Actor position has been set */
	void handleEventPositionUpdate(const EventPositionUpdate *event);

	void requestPerformAction(CharacterAction action);

	void onKeyPressUse();
	void onKeyPressAttack();
	void onKeyPressChargeUp();
	void onKeyPressSuicide();

	void handleInputKeyPress(const InputKeyPress *input);
	void handleInputKeyDown(const InputKeyDown *input);
	void handleInputKeyUp(const InputKeyUp *input);
	void handleInputMouseMove(const InputMouseMove *input);
	void handleInputMouseDownRight(const InputMouseDownRight *input);
	void handleInputMouseDownLeft(const InputMouseDownLeft *input);
	void handleInputMouseUpLeft(const InputMouseUpLeft *input);

private:
	float mouseSensitivity;
	vec3 lastReportedPosition;
	int playerNumber;
	bool w, s, a , d;
};

#endif
