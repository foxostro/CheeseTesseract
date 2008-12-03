#ifndef _PHYSICS_ENGINE_H_
#define _PHYSICS_ENGINE_H_

#include "Actor.h"
#include "ActorSet.h"

class PhysicsEngine
{
public:
	typedef function<void (dGeomID, dContact)> CollisionFn;
	typedef map<dGeomID, CollisionFn> CollisionCallBackMap;
	typedef CollisionCallBackMap::iterator CollisionCallBackMapHandle;

private:
	ActorSet *actorSet;
	dWorldID world;
	dSpaceID space;
	dJointGroupID contactGroup;
	dGeomID ray;
	vector<dContactGeom> rayCastResults;
	CollisionCallBackMap collisionCallBacks;

public:
    ~PhysicsEngine();

	PhysicsEngine(ActorSet *actorSe);

	/**
	Update the physics engine
	@param deltaTime Time elapsed since the last update
	*/
	void update(float deltaTime);

	/** Give the physics engine the opportunity to draw debug information */
	void draw() const;

	inline dWorldID getWorld() const
	{
	    return world;
	}

	inline dSpaceID getSpace() const
	{
	    return space;
	}

	inline dJointGroupID getContactGroup() const
	{
	    return contactGroup;
	}

	void nearCallback(dGeomID o1, dGeomID o2);

	void rayCallback(dGeomID ray, dGeomID o);

	/**
	Draws physics geometry object
	@param geom Physics geometry object
	*/
	static void drawGeom(dGeomID geom);

	/**
	Draws the AABB of the physics geometry object
	@param geom Physics geometry object
	*/
	static void drawGeomAABB(dGeomID geom);

	/** Gets the position of the geometry object */
	static vec3 getPosition(dGeomID geom);

	/** Gets the orientation of the geometry object */
	static mat3 getOrientation(dGeomID geom);

	/**
	Ray cast and determine if the given object is the first object to
	"collide" with the ray.
	@param geom Geom to test the ray against
	@param ignore Geom to ignore (e.g. ray origin)
	@param origin Origin of the ray in space
	@param direction Direction of the ray
	@param length Ray terminates after a given length
	*/
	bool rayCast(dGeomID geom,
	             dGeomID ignore,
	             const vec3 &origin,
				 const vec3 &direction,
				 float length);

	/**
	Ray cast and return all ray collision with the map
	@param geom Geom to test the ray against
	@param origin Origin of the ray in space
	@param direction Direction of the ray
	@param length Ray terminates after a given length
	*/
	vector<dContactGeom> rayCast(const vec3 &p,
                                 const vec3 &d,
                                 float length);

	bool rayCast(const vec3 &p,
		const vec3 &d,
		float length,
		dGeomID potentialCollider,
		dContactGeom &contact);

	/**
	Registers a new collision callback handler.  This function is called
	whenever a collision involving the specified geom is detected.
	@param geom Geom for which the callback applies
	@param callback Callback function
	@return handle to the callback
	*/
	CollisionCallBackMapHandle
	registerCollisionCallBack(dGeomID geom, CollisionFn callback)
	{
		collisionCallBacks.insert(make_pair(geom, callback));
		return collisionCallBacks.find(geom);
	}

	/**
	Unregister a collision callback
	@param handle Handle to identify the registered callback
	*/
	void unregisterCollisionCallBack(CollisionCallBackMapHandle handle)
	{
		collisionCallBacks.erase(handle);
	}

	static ActorID getActorFromGeom(dGeomID geom);

private:
	static void drawGeomSphere(dGeomID geom);
	static void drawGeomBox(dGeomID geom);
	static void drawGeomCappedCylinder(dGeomID geom);
	static void drawGeomCylinder(dGeomID geom);
	static void drawGeomTriMesh(dGeomID geom);
	static void drawGeomPlane(dGeomID geom);
	static void drawGeomRay(dGeomID geom);
	static void drawBox(dVector3 sides);

	/** From the ODE sample code */
	static void drawPatch(float p1[3], float p2[3], float p3[3], int level);

	/** From the ODE sample code */
	static void drawSphere();

	/** From the ODE sample code */
	static void drawCapsule(float length, float radius);

	/** From the ODE sample code */
	static void drawCylinder(float length, float radius, float zoffset);

	/** Pass through any collision callbacks */
	void handleCollisionCallBacks(dGeomID o1,
	                              dGeomID o2,
								  dContact contact) const;

	/** Pass actor collision messages to each actor */
	void passActorCollisionMessages(dGeomID o1,
	                                dGeomID o2,
									dContact contact) const;

	/** Pass actor collision message to one specific actor */
	void passActorCollisionMessages(ActorPtr actor,
	                                dGeomID o1,
									dGeomID o2,
									dContact contact) const;
};

#endif
