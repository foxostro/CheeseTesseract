#include "stdafx.h"
#include "World.h"
#include "searchfile.h"
#include "Application.h"
#include "Frustum.h"
#include "Dimmer.h"
#include "ComponentPhysics.h"
#include "ComponentMovement.h"
#include "ComponentHealth.h"

#include "ActionDeleteActor.h"

#include "EventPlayerNumberSet.h"
#include "EventExplosionOccurred.h"
#include "EventGameOver.h"

World::World(UID uid,
             ScopedEventHandler *parentScope,
             shared_ptr<class Renderer> _renderer,
             TextureFactory &_textureFactory,
             Camera *_camera)
		: camera(_camera),
		name("(nil)"),
		textureFactory(_textureFactory),
		renderer(_renderer),
		cameraMode(THIRD_PERSON_CAMERA),
		physicsRunning(true),
		displayDebugData(false),
		yaw(0.0f),
		pitch(0.0f),
		w(false),
		s(false),
		a(false),
		d(false),
		i(false),
		j(false),
		k(false),
		l(false),
		mapChangeRequested(false),
		ScopedEventHandler(uid, parentScope) {
	particleEngine = shared_ptr<ParticleEngine>(new ParticleEngine(genName(), this));
	registerSubscriber(particleEngine.get());
	
	terrain = shared_ptr<Terrain>(new Terrain(genName(), this));
	registerSubscriber(terrain.get());
	
	REGISTER_HANDLER(World::handleActionChangeMap);
	REGISTER_HANDLER(World::handleActionDebugEnable);
	REGISTER_HANDLER(World::handleActionDebugDisable);
	REGISTER_HANDLER(World::handleInputKeyPress);
	REGISTER_HANDLER(World::handleInputKeyDown);
}

void World::handleActionChangeMap(const ActionChangeMap *action) {
	mapChangeRequested = true;
	nextMap = action->nextMap;
}

void World::loadFromFile(const FileName &_fileName) {
	fileName = _fileName;
	load(PropertyBag::fromFile(fileName));
}

void World::load(const PropertyBag &bag) {
	PropertyBag mapBag, terrainBag;
	
	// Destroy any old instance of this world
	destroy();
	
	// Create a new physics engine instance
	physicsEngine = shared_ptr<PhysicsEngine>(new PhysicsEngine(&objects));
	
	name = bag.getString("name");
	objects.setParentScope(getParentScopePtr());
	objects.load(bag.getBag("objects"), this);
	
	if (bag.get("map", mapBag)) {
		terrain->create(mapBag, renderer, textureFactory, physicsEngine);
	}
	
	broadcastDebugModeDisable();
	cameraLooksAtPlayerStartPoint();
}

void World::draw() const {
	CHECK_GL_ERROR();
	
	if (displayDebugData) {
		physicsEngine->draw(); // draw physics debug information
	}
	
	//drawParticles();
	
	CHECK_GL_ERROR();
}

void World::update(float deltaTime) {
	if (isGameOver()) {
		broadcastGameOverEvent();
	} else {
		handleMapChangeRequest();
		objects.update(deltaTime);
		terrain->emitGeometry();
		recalculateAveragePlayerPosition();
		updateCamera(deltaTime);
		particleEngine->update(deltaTime, *camera);
		particleEngine->emitGeometry();
		updatePhysics(deltaTime);
		resetKeyFlags();
	}
}

ActorPtr World::createPlayer(const vec3 &initialPosition,
                             const PropertyBag &playerData,
                             int playerNumber,
                             int numOfPlayers) {
	const ActorPtr player = objects.create().get<1>();
	
	const PropertyBag base = playerData.getBag("components");
	const PropertyBag spec = PropertyBag(); // empty
	
	const ComponentDataSet components = ComponentDataSet::load(base, spec);
	
	player->load(components, initialPosition, vec3(0,0,0), this);
	
	sendPlayerNumber(playerNumber, player);
	
	if (numOfPlayers > 1) {
		shared_ptr<Component> component = player->getComponent("Physics");
		shared_ptr<ComponentPhysics> physics = dynamic_pointer_cast<ComponentPhysics>(component);
		
		if (physics) {
			const float ratio = (float)playerNumber / numOfPlayers;
			const float angle = 2.0f * (float)M_PI * ratio;
			const vec3 offset = vec3(cosf(angle), sinf(angle), 0.0f) * 1.2f;
			const vec3 &position = physics->getPosition();
			
			physics->setPosition(position + offset);
		}
	}
	
	return player;
}

void World::playersEnter(int numOfPlayers) {
	detroyAllPlayers();
	
	const vec3 playerPosition = getPlayerStartPoint();
	
	const PropertyBag playerData =
	 PropertyBag::fromFile(FileName("data/actorDefs/startingPlayer.xml"));
	 
	int numOfJoy = SDL_NumJoysticks();
	numOfPlayers = (numOfPlayers>numOfJoy) ? numOfJoy : numOfPlayers;
	numOfPlayers = (numOfPlayers<1) ? 1 : numOfPlayers;
	
	for (int i = 0; i < numOfPlayers; ++i) {
		ActorPtr player = createPlayer(playerPosition,
		                               playerData,
		                               i,
		                               numOfPlayers);
		players.addReference(player);
	}
	
	recalculateAveragePlayerPosition(); // Set initially...
	updateCamera(0); // Set initially...
}

void World::updateCamera_Overhead() {
	const size_t numOfPlayers = players.size();
	static const float minCameraDistance = 10.0f;
	
	if (numOfPlayers==0) {
		setCameraLook((float)(M_PI / 2.0), 0.0f, minCameraDistance, averagePlayerPosition);
		return;
	}
	
	const vec3 averagePlayerPosition = getAveragePlayerPosition();
	float distance = minCameraDistance;
	float cameraAngleZ = 0.0f;
	
	if (numOfPlayers>1) {
		float maxPlayerDistance=0.0f;
		for (ActorSet::iterator i=players.begin(); i!=players.end(); ++i) {
			ActorPtr player = i->second;
			ASSERT(player, "Null pointer: pl");
			
			shared_ptr<Component> component = player->getComponent("Physics");
			shared_ptr<ComponentPhysics> physics = dynamic_pointer_cast<ComponentPhysics>(component);
			
			if (physics) {
				const vec3 position = physics->getPosition();
				vec3 delta = position - averagePlayerPosition;
				const float distanceToLocus = delta.getMagnitude();
				maxPlayerDistance = max(maxPlayerDistance, distanceToLocus);
			}
		}
		
		distance = max(minCameraDistance,maxPlayerDistance+minCameraDistance);
	} else {
		ActorPtr player = players.begin()->second;
		ASSERT(player, "Null pointer: player");
		
		shared_ptr<Component> component = player->getComponent("Movement");
		shared_ptr<ComponentMovement> movement = dynamic_pointer_cast<ComponentMovement>(component);
		
		if (movement) {
			cameraAngleZ = movement->getFacingAngle();
		}
	}
	
	setCameraLook((float)(M_PI / 2.0),
	              cameraAngleZ,
	              distance,
	              averagePlayerPosition);
}

void World::updateCamera_FirstPerson(float deltaTime) {
	float dTime = deltaTime / 1000.0f;
	float yawSpeed = ((float)M_PI * 4.0f) * dTime;
	float pitchSpeed = ((float)M_PI * 2.0f) * dTime;
	float moveSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	
	if (a) yaw += yawSpeed;
	else if (d) yaw -= yawSpeed;
	
	if (w) pitch -= pitchSpeed;
	else if (s) pitch += pitchSpeed;
	
	if (i) moveSpeed = 5.0f * dTime;
	else if (k) moveSpeed = -5.0f * dTime;
	
	if (j) strafeSpeed = 5.0f * dTime;
	else if (l) strafeSpeed = -5.0f * dTime;
	
	pitch = max(-1.570796f, min(1.570796f, pitch));
	yaw = fmod(yaw, (float)M_PI * 2.0f);
	
	const vec3 look = vec3(cosf(yaw), sinf(yaw), sinf(pitch)).getNormal();
	
	vec3 up = vec3(0,0,1);
	vec3 eye = camera->eye + (look*moveSpeed) + (up.cross(look)*strafeSpeed);
	vec3 center = camera->eye + look;
	
	camera->lookAt(eye, center, up);
}

void World::updateCamera(float deltaTime) {
	switch (cameraMode) {
	case FIRST_PERSON_CAMERA:
		updateCamera_FirstPerson(deltaTime);
		break;
	case OVERHEAD_CAMERA:
		updateCamera_Overhead();
		break;
	case THIRD_PERSON_CAMERA:
		updateCamera_ThirdPerson();
		break;
	}
}

vec3 World::findAveragePlayerPosition() const {
	ASSERT(players.size()>=1, "There are no players in this world!");
	
	int count=0;
	vec3 averagePlayerPosition = vec3(0,0,0);
	
	for (ActorSet::const_iterator i=players.begin(); i!=players.end(); ++i) {
		const ActorPtr player = i->second;
		ASSERT(player, "Null parameter: player");
		
		shared_ptr<Component> component = player->getComponent("Physics");
		shared_ptr<ComponentPhysics> physics = dynamic_pointer_cast<ComponentPhysics>(component);
		
		if (physics) {
			averagePlayerPosition=averagePlayerPosition+physics->getPosition();
			count++;
		}
	}
	
	averagePlayerPosition = averagePlayerPosition * (1.0f/count);
	
	return averagePlayerPosition;
}

void World::onKeyDownFwd() {
	i = (cameraMode==FIRST_PERSON_CAMERA);
}

void World::onKeyDownRev() {
	k = (cameraMode==FIRST_PERSON_CAMERA);
}

void World::onKeyDownStrafeLeft() {
	j = (cameraMode==FIRST_PERSON_CAMERA);
}

void World::onKeyDownStrafeRight() {
	l = (cameraMode==FIRST_PERSON_CAMERA);
}

void World::onKeyDownUp() {
	w = (cameraMode==FIRST_PERSON_CAMERA);
}

void World::onKeyDownDown() {
	s = (cameraMode==FIRST_PERSON_CAMERA);
}

void World::onKeyDownLeft() {
	a = (cameraMode==FIRST_PERSON_CAMERA);
}

void World::onKeyDownRight() {
	d = (cameraMode==FIRST_PERSON_CAMERA);
}

void World::onKeyToggleCamera() {
	switch (cameraMode) {
	case FIRST_PERSON_CAMERA:
		cameraMode = OVERHEAD_CAMERA;
		break;
	case OVERHEAD_CAMERA:
		cameraMode = THIRD_PERSON_CAMERA;
		break;
	case THIRD_PERSON_CAMERA:
		cameraMode = FIRST_PERSON_CAMERA;
		break;
	}
	
	playSound(FileName("data/sound/flashlight-toggle.wav"));
}

void World::onKeyTogglePhysics() {
	physicsRunning = !physicsRunning;
	playSound(FileName("data/sound/flashlight-toggle.wav"));
}

void World::onKeyToggleDebugRendering() {
	if (displayDebugData) {
		broadcastDebugModeDisable();
	} else {
		broadcastDebugModeEnable();
	}
}

void World::playSound(const FileName &sound) {
	if (sound != FileName("")) {
		ActionPlaySound m(sound);
		sendAction(&m);
	}
}

void World::destroy() {
	mapChangeRequested = false;
	terrain->clear();
	objects.destroy();
	players.clear();
	physicsEngine.reset();
}

void World::setCameraLook(float angleY,
                          float angleZ,
                          float distance,
                          const vec3 &center) {
	mat3 z = mat3::fromRotateZ(angleZ);
	mat3 y1 = mat3::fromRotateY(angleY);
	mat3 y2 = mat3::fromRotateY(angleY + (float)(M_PI / 2.0));
	mat3 r1 = z * y1;
	mat3 r2 = z * y2;
	
	vec3 offset = r1.transformVector(vec3(distance, 0.0f, 0.0f));
	vec3 up = r2.transformVector(vec3(1.0f, 0.0f, 0.0f));
	
	camera->lookAt(center + offset, center, up);
}

void World::generateExplosion(ActorID originator,
                              const vec3 &position,
                              float rotation,
                              int baseDamage,
                              const FileName &soundFileName,
                              const FileName &particlesFileName) {
	playSound(soundFileName);
	
	particleEngine->add(particlesFileName,
	                    position,
	                    rotation,
	                    getTextureFactory());
	                    
	// Allow actors to apply splash damage and do ray-checking
	sendExplosionEvent(position, baseDamage, originator);
}

bool World::isGameOver() {
	for (ActorSet::iterator i=players.begin(); i!=players.end(); ++i) {
		ActorPtr player = i->second;
		ASSERT(player, "Null player: player");
		
		shared_ptr<Component> component = player->getComponent("Health");
		shared_ptr<ComponentHealth> health = dynamic_pointer_cast<ComponentHealth>(component);
		
		if (health && health->isDead()) {
			return true;
		}
	}
	
	return false;
}

void World::handleMapChangeRequest() {
	if (mapChangeRequested) {
		int numOfPlayers = (int)players.size();
		loadFromFile(nextMap);
		playersEnter(numOfPlayers);
		mapChangeRequested = false;
	}
}

void World::updatePhysics( float deltaTime ) {
	if (physicsRunning) {
		physicsEngine->update(deltaTime);
	}
}

void World::detroyAllPlayers() {
	players.destroy();
	objects.reapZombieActors();
}

void World::cameraLooksAtPlayerStartPoint() {
	averagePlayerPosition = getPlayerStartPoint();
	updateCamera(0);
}

vec3 World::getPlayerStartPoint() const {
	/*
	Iterate across all actors in the world to find an actor that can serve as
	the start point for the player party.
	*/
	for (ActorSet::const_iterator i=objects.begin(); i!=objects.end(); ++i) {
		ActorPtr a = i->second;
		
		if (a->hasComponent("PlayerStartMarker")) {
			shared_ptr<const Component> component;
			shared_ptr<const ComponentPhysics> physics;
			
			component = a->getComponent("Physics");
			physics = dynamic_pointer_cast<const ComponentPhysics>(component);
			
			if (physics) {
				vec3 p = vec3(physics->getPosition().xy(), 1.5f);
				TRACE("player start = " + vec3::toString(p));
				return p;
			}
		}
	}
	
	FAIL("No player start point found!");
	return vec3(0,0,0);
}

void World::handleActionDebugEnable(const ActionDebugEnable *) {
	displayDebugData = true;
	playSound(FileName("data/sound/activate.wav"));
}

void World::handleActionDebugDisable(const ActionDebugDisable *) {
	displayDebugData = false;
	playSound(FileName("data/sound/deactivate.wav"));
}

bool World::isAPlayer(ActorID actor) const {
	return players.isMember(actor);
}

void World::broadcastGameOverEvent() {
	EventGameOver m;
	sendEvent(&m);
}

void World::sendPlayerNumber( int playerNumber, const ActorPtr &player ) {
	EventPlayerNumberSet m(playerNumber);
	player->recvMessage(&m);
}

void World::broadcastDebugModeEnable() {
	ActionDebugEnable m;
	sendGlobalAction(&m);
}

void World::broadcastDebugModeDisable() {
	ActionDebugDisable m;
	sendGlobalAction(&m);
}

void World::sendExplosionEvent(const vec3 & position,
                               int baseDamage,
                               ActorID originator) {
	EventExplosionOccurred m(position, baseDamage, originator);
	getObjects().recvEvent(&m);
}

void World::handleInputKeyPress(const InputKeyPress *input) {
	switch (input->key) {
	case SDLK_F1:
		onKeyToggleCamera();
		break;
	case SDLK_F2:
		onKeyToggleDebugRendering();
		break;
	case SDLK_F3:
		onKeyTogglePhysics();
		break;
	}
}

void World::handleInputKeyDown(const InputKeyDown *input) {
	switch (input->key) {
	case SDLK_UP:
		onKeyDownUp();
		break;
	case SDLK_DOWN:
		onKeyDownDown();
		break;
	case SDLK_LEFT:
		onKeyDownLeft();
		break;
	case SDLK_RIGHT:
		onKeyDownRight();
		break;
	case SDLK_i:
		onKeyDownFwd();
		break;
	case SDLK_k:
		onKeyDownRev();
		break;
	case SDLK_j:
		onKeyDownStrafeLeft();
		break;
	case SDLK_l:
		onKeyDownStrafeRight();
		break;
	}
}

void World::updateCamera_ThirdPerson() {
	ActorPtr player;
	
	shared_ptr<Component> component;
	shared_ptr<ComponentMovement> movement;
	shared_ptr<ComponentPhysics> physics;
	
	if (players.size() == 0) goto failure;
	
	player = players.begin()->second;
	ASSERT(player, "Null pointer: player");
	
	component = player->getComponent("Movement");
	movement = dynamic_pointer_cast<ComponentMovement>(component);
	
	if (!movement) goto failure;
	
	component = player->getComponent("Physics");
	physics = dynamic_pointer_cast<ComponentPhysics>(component);
	
	if (!physics) goto failure;
	
	setCameraLook((float)(M_PI / 4.0),
	              movement->getFacingAngle(),
	              10.0f,
	              physics->getPosition());
	              
	return;
	
failure:
	setCameraLook((float)(M_PI / 2.0), 0.0f, 10.0f, averagePlayerPosition);
}
