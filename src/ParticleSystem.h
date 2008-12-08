#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "ParticleElement.h"
#include "ResourceBuffer.h"
#include "Mesh.h"

class ParticleEmitter; // Opaque pointers FTW

/**
System of particles and particle emitters.
Manages a collection of particle emitters and a fixed
number of particles (per system) spawned from those emitters.
*/
class ParticleSystem {
private:
	typedef vector<ParticleEmitter*> EmitterSet;
	typedef list<ParticleElement> ParticleSet;
	
	/** Particle data and the graphics resources to render them */
	struct ParticleBatch {
		ParticleSet elements;
		Mesh geometry;
	};
	
	/** Organizes set of particles by material */
	typedef map<Material*, ParticleBatch> ParticleBuckets;
	
public:
	/** Destructor */
	~ParticleSystem();
	
	/**
	Constructor
	@param fileName file describing the system
	@param textureFactory Texture loader
	@param position Initial world-space position of the particle system
	@param rotation Rotation of the system (radians) about the Z-axis. Used
	                by emitters that use direct initial velocities to point
	                them in some direction.
	*/
	ParticleSystem(const FileName &fileName,
	               TextureFactory &textureFactory,
	               const vec3 &position,
	               float rotation);
	               
	/** Emit geometry for the particle batch */
	void getGeometryChunks(vector<GeometryChunk> &chunks);
	
	/**
	Updates the particle system
	@param deltaTime Milliseconds since the last update
	@param camera Camera to align particle billboards against
	*/
	void update(float deltaTime, Camera &camera);
	
	/**
	Spawns a new particle
	@param element Particle to copy when spawning the new one
	*/
	void spawn(const ParticleElement &element);
	
	/**
	Retrieves a particle template given its name
	@param name Name of the particle template
	*/
	const ParticleElement& getTemplate(const string &name);
	
	/**
	Gets the material handle when given a material name
	@param name Name of the material
	@return handle to the material
	*/
	Material* getMaterialPtr(const string &name) {
		return &materials[name];
	}
	
	/**
	Retrieves a material given its handle
	@param materialHandle Handle of the material
	*/
	Material& getMaterial(const string &name) {
		return materials[name];
	}
	
	/**
	Polls the emitters and returns true if all emitters are dead,
	else returns false
	*/
	bool isDead() const;
	
	/** Kills each particle emitter */
	void kill();
	
	/**
	Gets the position of the system
	@return position of the system
	*/
	const vec3& getPosition() const {
		return position;
	}
	
	/**
	Sets the position of the body
	@param position Position of the body
	*/
	void setPosition(const vec3 &_position) {
		position = _position;
	}
	
	/** Sets the rotation of the system about the Z-axis (radians) */
	void setRotation(float _rotation) {
		rotation = _rotation;
	}
	
	/**
	Gets the rotation of the system about the Z-axis (radians)
	@return rotation of the system about the Z-axis (radians)
	*/
	float getRotation() const {
		return rotation;
	}
	
private:
	/**
	Updates the particle batches
	@param modl model-view matrix that billboards are aligned against
	*/
	void updateParticleBatches(const mat4 &modl);
	
	/** destroy all particle emitters */
	void destroyEmitters();
	
	/** Updates all particles */
	void updateParticles(float milliseconds);
	
	/** Updates all emitters */
	void updateEmitters(float milliseconds);
	
	/** Resets the entire particle system; frees resources */
	void reset();
	
	/**
	Loads particle materials
	@param data Data source
	@param textureFactory Texture loader
	*/
	void loadParticleMaterials(const PropertyBag &data,
	                           TextureFactory &textureFactory);
	                           
	/**
	Loads particle templates used by emitters
	@param data Data source
	*/
	void loadParticleTemplates(const PropertyBag &data);
	
	/**
	Loads particle emitters
	@param data Data source
	*/
	void loadParticleEmitters(const PropertyBag &data);
	
private:
	/** Particle-Template's Name -> Particle-Template */
	map<string, ParticleElement> templatesByName;
	
	/**
	Particle materials available for a template to specify
	Material Name -> Material
	*/
	map<string, Material> materials;
	
	/** Particle emitters managed by the ParticleSystem */
	EmitterSet emitters;
	
	/**
	Hard maximum number of particles that may be maintained
	by the ParticleSystem
	*/
	size_t maxNumberOfParticles;
	
	/**
	Particles allocated for the ParticleSystem, organized by
	particle material
	*/
	ParticleBuckets buckets;
	
	/** World-space position of the system */
	vec3 position;
	
	/**
	Rotation of the system (radians) about the Z-axis. Used by emitters that
	use direct initial velocities to point them in some direction.
	*/
	float rotation;
};

#endif
