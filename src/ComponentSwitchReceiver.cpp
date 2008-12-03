#include "stdafx.h"
#include "Actor.h"
#include "ComponentSwitchReceiver.h"
#include "EventUsesObject.h"

ComponentSwitchReceiver::
ComponentSwitchReceiver(UID uid, ScopedEventHandler *parentScope)
: Component(uid, parentScope)
{
	REGISTER_HANDLER(ComponentSwitchReceiver::handleEventSwitchToggled);
}

void ComponentSwitchReceiver::handleEventSwitchToggled( const EventSwitchToggled *event )
{
	const int categoryID = event->categoryID;
	
	ASSERT(categoryID<MAX_SWITCHES && categoryID>=0,
		"Message field \"categoryID\" is invalid: " + itos(categoryID));	
	switchState[categoryID] = !switchState[categoryID];

	if(expr_on_switch.eval<bool>())
	{
		for(int i=0; i<MAX_SWITCHES; ++i)
		{
			switchStateOld[i] = switchState[i];
		}

		EventUsesObject m(event->requesterID);
		sendEvent(&m);
	}
}

void ComponentSwitchReceiver::resetMembers()
{
	expr_on_switch = sexpr_t();

	for(int i=0; i<MAX_SWITCHES; ++i)
	{
		switchState[i] = switchStateOld[i] = false;
	}
}

sexpr_t::tree_node_ptr ComponentSwitchReceiver::defun_switchState(sexpr_t::cells_t parameters)
{
	const int switchIdx = sexpr_t::eval_leaf<int>(*parameters.begin());
	ASSERT(switchIdx<MAX_SWITCHES && switchIdx>=0, "\"switchIdx\" invalid" + itos(switchIdx));
	const bool value = switchState[switchIdx];
	return sexpr_t::tree_node_ptr(new sexpr_t::tree_leaf_t<bool>(value));
}

sexpr_t::tree_node_ptr ComponentSwitchReceiver::defun_switchToggled(sexpr_t::cells_t parameters)
{
	const int switchIdx = sexpr_t::eval_leaf<int>(*parameters.begin());
	ASSERT(switchIdx<MAX_SWITCHES && switchIdx>=0, "\"switchIdx\" invalid" + itos(switchIdx));
	const bool curr = switchState[switchIdx];
	const bool prev = switchStateOld[switchIdx];
	return sexpr_t::tree_node_ptr(new sexpr_t::tree_leaf_t<bool>(curr != prev));
}

void ComponentSwitchReceiver::load(const PropertyBag &data)
{
	resetMembers();
	loadExpressions(data);
}

void ComponentSwitchReceiver::loadExpressions(const PropertyBag &data)
{
	expr_on_switch = sexpr_t::fromString(data.getString("expr_on_switch"));
	expr_on_switch.addBinding("switchState",   bind(&ComponentSwitchReceiver::defun_switchState, this, _1));
	expr_on_switch.addBinding("switchToggled", bind(&ComponentSwitchReceiver::defun_switchToggled, this, _1));
}

void ComponentSwitchReceiver::update(float)
{
	/* Do Nothing */
}
