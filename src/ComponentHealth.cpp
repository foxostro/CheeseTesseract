#include "stdafx.h"
#include "GraphicsDevice.h"
#include "World.h"
#include "ComponentPhysics.h"
#include "ComponentHealth.h"

#include "EventCharacterHasDied.h"
#include "EventCharacterRevived.h"
#include "EventDamageReceived.h"
#include "EventHealingReceived.h"

#include "ActionPlaySound.h"
#include "ActionDeleteActor.h"
#include "ActionEnableModelHighlight.h"
#include "ActionDisableModelHighlight.h"

ComponentHealth::ComponentHealth(UID uid, ScopedEventHandler *parentScope)
		: Component(uid, parentScope),
		world(0) {
	REGISTER_HANDLER(ComponentHealth::handleEventCharacterRevived);
	REGISTER_HANDLER(ComponentHealth::handleEventCharacterHasDied);
	REGISTER_HANDLER(ComponentHealth::handleEventDamageReceived);
	REGISTER_HANDLER(ComponentHealth::handleEventHealingReceived);
	REGISTER_HANDLER(ComponentHealth::handleEventPositionUpdate);
	REGISTER_HANDLER(ComponentHealth::handleEventHeightUpdate);
	REGISTER_HANDLER(ComponentHealth::handleEventExplosionOccurred);
	REGISTER_HANDLER(ComponentHealth::handleEventDeathBehaviorUpdate);
	REGISTER_HANDLER(ComponentHealth::handleMessagePassWorld);
	
	resetMembers();
}

void ComponentHealth::resetMembers() {
	health = 100;
	maxHealth = 100;
	dead = false;
	lastReportedPosition.zero();
	lastReportedHeight = 1.0f;
	willResurrectAfterCountDown = false;
	timeSpentDead = 0.0f;
	timeUntilResurrection = 0.0f;
	lastReportedDeathBehavior = Corpse;
}

void ComponentHealth::update(float deltaTime) {
	if (!dead) return;
	
	if ((timeSpentDead += deltaTime) >= timeUntilResurrection) {
		if (getDeathBehavior() == Disappear) {
			ActorID actor = getActorID();
			ActionDeleteActor msg(actor);
			sendGlobalAction(&msg);
		} else if (willResurrectAfterCountDown) {
			EventCharacterRevived msg;
			sendGlobalEvent(&msg);
		}
	}
}

void ComponentHealth::handleEventCharacterRevived(const EventCharacterRevived *) {
	dead = false;
	health = max(maxHealth / 2, 1);
	maxHealth = max(maxHealth, 1);
	timeSpentDead = 0.0f;
}

void ComponentHealth::handleEventCharacterHasDied(const EventCharacterHasDied *) {
	health = 0;
	dead = true;
	timeSpentDead = 0.0f;
}

void ComponentHealth::handleEventDamageReceived(const EventDamageReceived *event) {
	if (!dead) {
		loseHealth(event->damage); // Lose health when damaged
		
		// Flash the model briefly to signal damage
		{
			ActionEnableModelHighlight m(HighlightFlash, 300.0f, red, white);
			sendAction(&m);
		}
	}
}

void ComponentHealth::handleEventHealingReceived( const EventHealingReceived *event ) {
	if (!dead) {
		health += event->healing;
		health = min(health, maxHealth);
	}
}

void ComponentHealth::handleEventPositionUpdate( const EventPositionUpdate *event ) {
	lastReportedPosition = event->position;
}

void ComponentHealth::handleEventHeightUpdate( const EventHeightUpdate *event ) {
	lastReportedHeight = event->height;
}

void ComponentHealth::load(const PropertyBag &data) {
	resetMembers();
	health = data.getInt("health");
	maxHealth = data.getInt("maxHealth");
	willResurrectAfterCountDown = data.getBool("willResurrectAfterCountDown");
	timeUntilResurrection = data.getFloat("timeUntilResurrection");
}

void ComponentHealth::handleEventExplosionOccurred(const EventExplosionOccurred *event) {
	int baseDamage = event->baseDamage;
	
	if (baseDamage <= 0)
		return;
		
	const vec3 &pos = event->position;
	float distance = vec3(lastReportedPosition - pos).getMagnitude();
	float p = powf((float)M_E, -SQR(distance/1.5f));
	int damage = (int)(baseDamage * p);
	
	if (damage <= 0)
		return;
		
	broadcastDamageReceived(damage);
}

void ComponentHealth::handleMessagePassWorld( const MessagePassWorld *message ) {
	world = message->world;
}

void ComponentHealth::loseHealth(int damage) {
	health -= damage;
	
	if (health <= 0) {
		EventCharacterHasDied m;
		sendEvent(&m);
		health = 0;
	}
}

DeathBehavior ComponentHealth::getDeathBehavior() const {
	return lastReportedDeathBehavior;
}

void ComponentHealth::drawHealthBar(const mat3 &m) const {
	/*
	TODO: Render billboard text
	string s = "HP: " + itos(health) + " / " + itos(maxHealth);
	font->billboardWrite(s, white, Font::SizeLarge, m);
	*/
}

void ComponentHealth::drawResurrectCountDown(const mat3 &m) const {
	/*
	TODO: Render billboard text
	static const color c(0.9f, 0.9f, 1.0f, 0.9f);
	float t = timeUntilResurrection - timeSpentDead;
	string s = itos((int)(t / 1000.0f));
	font->billboardWrite(s, c, Font::SizeHuge, m);
	*/
}

void ComponentHealth::broadcastDamageReceived(int damage) {
	EventDamageReceived m(damage);
	sendEvent(&m);
}

void ComponentHealth::handleEventDeathBehaviorUpdate(const EventDeathBehaviorUpdate *event) {
	lastReportedDeathBehavior = event->deathBehavior;
}
