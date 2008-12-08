#include "stdafx.h"
#include "AnimationControllerFactory.h"
#include "Actor.h"
#include "ComponentDeathBehavior.h"
#include "ComponentRenderAsModel.h"
#include "RenderMethodTags.h"
#include "EventRadiusUpdate.h"

extern shared_ptr<AnimationControllerFactory> g_ModelFactory; // TODO: Remove global var?

ComponentRenderAsModel::
ComponentRenderAsModel(UID _uid, ScopedEventHandler *_parentScope)
		: Component(_uid, _parentScope),
		model(0),
		dead(false),
		highlightMode(HighlightDisable),
		highlightIntensity(1.0f),
		hightlightIntensityRate(100.0f) {
	REGISTER_HANDLER(ComponentRenderAsModel::handleEventCharacterRevived);
	REGISTER_HANDLER(ComponentRenderAsModel::handleEventCharacterHasDied);
	REGISTER_HANDLER(ComponentRenderAsModel::handleEventPositionUpdate);
	REGISTER_HANDLER(ComponentRenderAsModel::handleEventDeathBehaviorUpdate);
	REGISTER_HANDLER(ComponentRenderAsModel::handleEventOrientationUpdate);
	REGISTER_HANDLER(ComponentRenderAsModel::handleEventHeightUpdate);
	
	REGISTER_HANDLER(ComponentRenderAsModel::handleActionSetModel);
	REGISTER_HANDLER(ComponentRenderAsModel::handleActionChangeAnimation);
	REGISTER_HANDLER(ComponentRenderAsModel::handleActionLookAt);
	REGISTER_HANDLER(ComponentRenderAsModel::handleActionEnableModelHighlight);
	REGISTER_HANDLER(ComponentRenderAsModel::handleActionDisableModelHighlight);
}

void ComponentRenderAsModel::resetMembers() {
	// defaults
	dead = false;
	lastReportedDeathBehavior = Corpse;
	lastReportedPosition.zero();
	lastReportedOrientation.identity();
	lastReportedHeight = 1.0f;
	modelHeight = 42.42f;
	independentModelOrientation = false;
	highlightMode = HighlightDisable;
	highlightIntensity = 1.0f;
	hightlightIntensityRate = 400.0f;
	outlineWidth = 2.0f;
	highlightTimeRemaining = 0.0f;
	colora = yellow;
	colorb = white;
}

void ComponentRenderAsModel::loadModel(const FileName &fileName) {
	ASSERT(g_ModelFactory, "modelFactory is null");
	model = g_ModelFactory->createFromFile(fileName);
	
	changeAnimation("idle"); // default animation
	modelHeight = model->calculateHeight(); // Calculate height of unscaled model
	
	// rescale the vertices directly
	model->uniformScale(lastReportedHeight / modelHeight);
	
	modelHeight = model->calculateHeight(); // Calculate height of properly scaled model
	ASSERT(fabsf(lastReportedHeight-modelHeight)<0.01f, "Model height is wrong");
	
	broadcastModelRadius();
}

void ComponentRenderAsModel::update(float milliseconds) {
	ASSERT(model, "Null pointer: model");
	
	model->update(milliseconds);
	
	// Pass mesh to the renderer
	vector<RenderInstance> m;
	emitGeometry(m);
	for (vector<RenderInstance>::iterator i=m.begin(); i!=m.end(); ++i) {
		ActionQueueRenderInstance action(*i);
		getParentScope().sendGlobalAction(&action);
	}
	
	if (highlightMode != HighlightDisable) {
		// update highlighting intensity
		highlightIntensity += milliseconds/hightlightIntensityRate;
		highlightIntensity = (highlightIntensity>1.0f) ? 0.0f
		                     : highlightIntensity;
		                     
		// Allow highlight to end after a period of time
		highlightTimeRemaining -= milliseconds;
		
		if (highlightTimeRemaining <= 0.0f) {
			highlightTimeRemaining = 0.0f;
			highlightMode = HighlightDisable;
		}
	}
}

bool ComponentRenderAsModel::changeAnimation(const string &name) {
	ASSERT(model, "Null pointer: model");
	return model->requestAnimationChange(name);
}

void ComponentRenderAsModel::handleActionSetModel(const ActionSetModel *message ) {
	loadModel(message->fileName);
}

void ComponentRenderAsModel::handleEventHeightUpdate( const EventHeightUpdate *event ) {
	if (event->height != lastReportedHeight) {
		lastReportedHeight = event->height;
		
		// rescale the vertices directly
		model->uniformScale(lastReportedHeight / modelHeight);
		
		modelHeight = model->calculateHeight(); // Recalculate height of unscaled model
		broadcastModelRadius();
	}
}

void ComponentRenderAsModel::handleEventOrientationUpdate(const EventOrientationUpdate *message ) {
	if (!independentModelOrientation) {
		lastReportedOrientation = message->orientation;
	}
}

void ComponentRenderAsModel::handleEventPositionUpdate(const EventPositionUpdate *message) {
	lastReportedPosition = message->position;
}

void ComponentRenderAsModel::handleEventDeathBehaviorUpdate(const EventDeathBehaviorUpdate *message) {
	lastReportedDeathBehavior = message->deathBehavior;
}

void ComponentRenderAsModel::handleActionChangeAnimation(const ActionChangeAnimation *message ) {
	changeAnimation(message->animationName);
}

void ComponentRenderAsModel::handleActionLookAt(const ActionLookAt *message ) {
	if (independentModelOrientation) {
		lastReportedOrientation = mat3::fromRotateZ(message->facingAngle);
	}
}

void ComponentRenderAsModel::load(const PropertyBag &data) {
	resetMembers();
	const FileName modelFileName = data.getFileName("model");
	loadModel(modelFileName);
	data.get("independentModelOrientation", independentModelOrientation);
}

void ComponentRenderAsModel::handleEventCharacterRevived(const EventCharacterRevived*) {
	dead = false;
}

void ComponentRenderAsModel::handleEventCharacterHasDied(const EventCharacterHasDied*) {
	dead = true;
}

DeathBehavior ComponentRenderAsModel::getDeathBehavior() const {
	return lastReportedDeathBehavior;
}

void ComponentRenderAsModel::handleActionEnableModelHighlight(const ActionEnableModelHighlight *message) {
	highlightMode = message->mode;
	highlightTimeRemaining = message->time;
	colora = message->colora;
	colorb = message->colorb;
}

void ComponentRenderAsModel::handleActionDisableModelHighlight(const ActionDisableModelHighlight*) {
	highlightMode = HighlightDisable;
}

void ComponentRenderAsModel::broadcastModelRadius() {
	const float modelScale = lastReportedHeight / modelHeight;
	float modelRadius = model->calculateCylindricalRadius();
	EventRadiusUpdate m(modelRadius * modelScale);
	sendEvent(&m);
}

mat4 ComponentRenderAsModel::getTransformation() const {
	float modelScale = 1.0f; // lastReportedHeight / modelHeight;
	
	vec3 pos = lastReportedPosition - vec3(0, 0, lastReportedHeight/2.0f);
	
	vec3 x = lastReportedOrientation.getAxisX().getNormal();
	vec3 y = lastReportedOrientation.getAxisY().getNormal();
	vec3 z = lastReportedOrientation.getAxisZ().getNormal();
	
	mat4 unscaled = mat4(pos, x, y, z);
	
	mat4 scaleMat; // NOTE: Column matrix
	
	scaleMat.m[0] = modelScale;
	scaleMat.m[4] = 0.0f;
	scaleMat.m[8] = 0.0f;
	scaleMat.m[12]= 0.0f;
	
	scaleMat.m[1] = 0.0f;
	scaleMat.m[5] = modelScale;
	scaleMat.m[9] = 0.0f;
	scaleMat.m[13]= 0.0f;
	
	scaleMat.m[2] = 0.0f;
	scaleMat.m[6] = 0.0f;
	scaleMat.m[10]= modelScale;
	scaleMat.m[14]= 0.0f;
	
	scaleMat.m[3] = 0.0f;
	scaleMat.m[7] = 0.0f;
	scaleMat.m[11]= 0.0f;
	scaleMat.m[15]= 1.0f;
	
	mat4 transformation = unscaled * scaleMat;
	
	return transformation;
}

void ComponentRenderAsModel::emitGeometry(vector<RenderInstance> &m) const {
	const mat4 transformation = getTransformation();
	
	vector<GeometryChunk> chunks;
	model->getGeometryChunks(chunks);
	
	for (vector<GeometryChunk>::iterator i=chunks.begin(); i!=chunks.end();++i) {
		RenderInstance instance;
		instance.gc = *i;
		instance.gc.transformation = transformation;
		m.push_back(instance);
	}
}
