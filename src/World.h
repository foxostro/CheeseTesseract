#ifndef _WORLD_H_
#define _WORLD_H_

#include "vec4.h"
#include "PropertyBag.h"

#include "ActorSet.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Camera.h"
#include "Fog.h"
#include "PhysicsEngine.h"
#include "ParticleEngine.h"
#include "SDLinput.h"

#include "ActionChangeMap.h"
#include "ActionDebugEnable.h"
#include "ActionDebugDisable.h"

class GraphicsDevice;

/**
Game world.
Contains the game world map, the player, game objects, and other entities
*/
class World : public ScopedEventHandler
{
public:
	/**
	Constructor
	@param uid UID to identify this blackboard subscriber
	@param parentBlackBoard Communication with game subsystems
	@param textureFactory Tracks loaded textures
	*/
	World(UID uid,
		  ScopedEventHandler *parentBlackBoard,
		  shared_ptr<class Renderer> renderer,
		  TextureFactory &textureFactory,
		  Camera *camera);

	/** Loads the world from file */
	void loadFromFile(const FileName &fileName);

	/**
	Retrieves the name of the realm
	@return Name of the realm
	*/
	inline const string& getName() const
	{
		return name;
	}

	/**
	Sets the name of the realm
	@param name New name of the realm
	*/
	inline void setName(const string &name)
	{
		this->name = name;
	}

	/**
	Gets the object database
	@return object database
	*/
	inline ActorSet& getObjects()
	{
		return objects;
	}

	/**
	Gets the object database
	@return object database
	*/
	inline const ActorSet& getObjects() const
	{
		return objects;
	}

	/** Destroys all game world assets and resets the game world */
	void destroy();

	/**
	Update the World
	@param deltaTime Time elapsed since the last update
	*/
	void update(float deltaTime);

	/** Draws the scene */
	void draw() const;

	/**
	Gets the most recently calculated mean player position
	@return mean player position
	*/
	inline const vec3& getAveragePlayerPosition() const
	{
		return averagePlayerPosition;
	}

	inline shared_ptr<PhysicsEngine> getPhysicsEngine() const
	{
	    return physicsEngine;
	}

	TextureFactory& getTextureFactory()
	{
		return textureFactory;
	}

	const TextureFactory& getTextureFactory() const
	{
		return textureFactory;
	}

	/**
	Generates an explosion in the game world
	@param originator actor that originated the explosion (immune)
	@param position Position of the explosion
	@param rotation Rotation (radians) of the particle system about the Z-axis
	@param baseDamage Base damage before distance falloff
	@param soundFileName File name of the explosion sound effect
	@param particlesFileName Filename of the particle system
	*/
	void generateExplosion(ActorID originator,
	                       const vec3 &position,
	                       float rotation,
						   int baseDamage,
	                       const FileName &soundFileName,
	                       const FileName &particlesFileName);

	/**
	Players enter the game world; recreate players in the game world
	@param numPlayers Number of players entering the game
	*/
	void playersEnter(int numPlayers);

	/** Given an actor, determine if it is one of the world's players */
	bool isAPlayer(ActorID actor) const;

private:
	/** Do not call the assignment operator */
	World operator=(const World &rh);

	/** Do not call the copy constructor */
	World(const World &world);

	void onKeyDownUp();
	void onKeyDownDown();
	void onKeyDownLeft();
	void onKeyDownRight();
	void onKeyDownFwd();
	void onKeyDownRev();
	void onKeyDownStrafeLeft();
	void onKeyDownStrafeRight();
	void onKeyToggleCamera();
	void onKeyToggleDebugRendering();
	void onKeyTogglePhysics();

	void handleActionChangeMap(const ActionChangeMap *action);
	void handleActionDebugEnable(const ActionDebugEnable *action);
	void handleActionDebugDisable(const ActionDebugDisable *action);

	void broadcastGameOverEvent();
	
	/** Plays a sound */
	void playSound(const FileName &sound);

	/**
	Loads the world state
	@param xml data source
	*/
	void load(const PropertyBag &xml);

	/**
	Search for the player start point and set the initial camera position
	to focus on that location
	*/
	void cameraLooksAtPlayerStartPoint();

	/** Searches for the player start point and returns its position */
	vec3 getPlayerStartPoint() const;

	/**
	Creates a single player entity from data (Adds it to "objects" set).
	Please note that the position described in the playerData may be
	interpreted as the starting position of the player (single-player) or as
	the locus of the player party (multi-player)  So, a second step where the
	player's are repositioned around the party locus may be necessary after
	the call to this method has completed.
	@param initialPosition Initial player position
	@param playerData Data describing the starting player configuration
	@param playerNumber The player's number
	@param numOfPlayers Number of players in the party (so we can center about
	                    the party position)
	@return Player
	*/
	ActorPtr createPlayer(const vec3 &initialPosition,
	                      const PropertyBag &playerData,
	                      int playerNumber,
						  int numOfPlayers);

	/** Destroys all player characters in the game world */
	void detroyAllPlayers();

	/** Updates the camera positions */
	void updateCamera(float deltaTime);

	/** Harmonizes the camera with the current player positions */
	void updateCamera_Overhead();

	/**
	Sets the camera position and looks at the average player position
	@param theta Camera angle (radians) about the X axis
	@param angleZ Camera angle (radians) about the Z axis
	@param distance Greatest distance from a player to the average position
	@param averagePlayerPosition Average of all player positions
	*/
	void setCameraLook(float theta,
	                   float angleZ,
	                   float distance,
	                   const vec3 &averagePlayerPosition);

	/** Flying 1st person camera */
	void updateCamera_FirstPerson(float deltaTime);

	/** Third person camera focused on player 0 */
	void updateCamera_ThirdPerson();

	/** Periodically calculates and caches the average player position */
	inline void recalculateAveragePlayerPosition()
	{
		averagePlayerPosition = findAveragePlayerPosition();
	}

	/**
	Finds the mean position of all the players in the game
	@return average position of all the players
	*/
	vec3 findAveragePlayerPosition() const;

	inline void resetKeyFlags()
	{
		w = s = a = d = i = k = j = l = false;
	}

	/** Updates game physics */
	void updatePhysics( float deltaTime );

	/** Process any pending requests to change the map */
	void handleMapChangeRequest();

	/**
	Directly polls the players in the world to determine whether
	game over conditions have occurred.
	*/
	bool isGameOver();

	void sendPlayerNumber(int playerNumber, const ActorPtr &player);

	void broadcastDebugModeEnable();

	void broadcastDebugModeDisable();

	void sendExplosionEvent(const vec3 & position,
	                        int baseDamage,
							ActorID originator);
	
	void handleInputKeyPress(const InputKeyPress *input);

	void handleInputKeyDown(const InputKeyDown *input);

public:
	/** References to the players */
	ActorSet players;

	/** Particle engine manages all particle systems */
	shared_ptr<ParticleEngine> particleEngine;

private:
	/** Reference to the active camera for the application */
	Camera *camera;

	/** Name of the World */
	string name;
	
	/** Filename of most recent map data source */
	FileName fileName;

	/** Set of objects that reside within this World */
	ActorSet objects;

	/** Height map based terrain */
	shared_ptr<Terrain> terrain;

	/** Tracks loaded textures */
	TextureFactory &textureFactory;

	/** Periodically calculates and caches the average player position */
	vec3 averagePlayerPosition;

	/** Physics engine subsystem */
	shared_ptr<PhysicsEngine> physicsEngine;

	/** Indicates that the camera is in FP mode (true) or game mode (false) */
	enum
	{
		FIRST_PERSON_CAMERA,
		OVERHEAD_CAMERA,
		THIRD_PERSON_CAMERA,
	} cameraMode;

	/** Indicates that the physics engine is running */
	bool physicsRunning;

	/** Indicates that the debug rendering should be used */
	bool displayDebugData;

	/** Camera yaw and pitch while in FP camera mode */
	float yaw, pitch;

	bool w, s, a, d, i, j, k, l;

	/**
	Only valid when mapChangeRequested is true.
	Indicates the map to change to
	*/
	FileName nextMap;
	
	/** Indicates that a map change was requested in the previous tick */
	bool mapChangeRequested;

	/** References the game's renderer */
	shared_ptr<class Renderer> renderer;
};

#endif
