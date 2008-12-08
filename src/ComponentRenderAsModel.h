#ifndef COMPONENT_RENDER_AS_MODEL_H
#define COMPONENT_RENDER_AS_MODEL_H

#include "PropertyBag.h"
#include "AnimationController.h"
#include "Component.h"

#include "EventCharacterRevived.h"
#include "EventOrientationUpdate.h"
#include "EventPositionUpdate.h"
#include "EventDeathBehaviorUpdate.h"
#include "EventCharacterHasDied.h"
#include "EventHeightUpdate.h"

#include "ActionDisableModelHighlight.h"
#include "ActionEnableModelHighlight.h"
#include "ActionSetModel.h"
#include "ActionLookAt.h"
#include "ActionChangeAnimation.h"
#include "ActionQueueRenderInstance.h"

class ComponentRenderAsModel : public Component {
public:
	virtual string getTypeString() const {
		return "RenderAsModel";
	}
	
	ComponentRenderAsModel(UID _uid, ScopedEventHandler *_blackBoard);
	
	/** Loads component data from the pool of all object data */
	virtual void load(const PropertyBag &data);
	
	/** Updates the object */
	virtual void update(float milliseconds);
	
	/** Resets all object members to defaults */
	virtual void resetMembers();
	
	/**
	Loads a model for the object
	@param fileName The file name of a model's XML definition
	*/
	void loadModel(const FileName &fileName);
	
	/**
	Requests that the animation be changed to the specified animation at the next available opportunity
	@param name The name of the new animation
	*/
	bool changeAnimation(const string &name);
	
	const AnimationController &getModel() const {
		ASSERT(model, "Actor does not have model!");
		return *model;
	}
	
private:
	void handleEventCharacterRevived(const EventCharacterRevived *message);
	void handleEventCharacterHasDied(const EventCharacterHasDied *message);
	void handleEventOrientationUpdate(const EventOrientationUpdate *message);
	void handleEventPositionUpdate(const EventPositionUpdate *message);
	void handleEventDeathBehaviorUpdate(const EventDeathBehaviorUpdate *message);
	
	void handleActionLookAt(const ActionLookAt *message);
	void handleActionSetModel(const ActionSetModel *message);
	void handleActionChangeAnimation(const ActionChangeAnimation *message);
	void handleEventHeightUpdate(const EventHeightUpdate *event);
	void handleActionEnableModelHighlight(const ActionEnableModelHighlight *message);
	void handleActionDisableModelHighlight(const ActionDisableModelHighlight *message);
	
	/**
	Sends an update event to notify listeners of
	the model's cylindrical radius.
	*/
	void broadcastModelRadius();
	
	/** Gets the death behavior of the character */
	DeathBehavior getDeathBehavior() const;
	
	/** Get all the static meshes for this tick */
	void emitGeometry(vector<RenderInstance> &m) const;
	
	/** Gets the transformation of the actor */
	mat4 getTransformation() const;
	
private:
	AnimationController *model;
	vec3 lastReportedPosition;
	mat3 lastReportedOrientation;
	float lastReportedHeight;
	float modelHeight;
	bool independentModelOrientation;
	bool dead;
	DeathBehavior lastReportedDeathBehavior;
	
	HighlightMode highlightMode;
	float highlightIntensity;
	float hightlightIntensityRate;
	float outlineWidth;
	float highlightTimeRemaining;
	color colora;
	color colorb;
};

#endif
