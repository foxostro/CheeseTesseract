#include "stdafx.h"
#include "ActionQueueRenderInstance.h"
#include "RenderMethodTags.h"
#include "ParticleSystem.h"
#include "ParticleEngine.h"

void ParticleEngine::update(float milliseconds, Camera &camera) {
	list<ParticleSystem*>::iterator i = particles.begin();
	
	while (i != particles.end()) {
		ParticleSystem *system = *i;
		
		system->update(milliseconds, camera);
		
		if (system->isDead()) {
			i = particles.erase(i);
		} else {
			++i;
		}
	}
}

ParticleEngine::handle ParticleEngine::add(const FileName &fileName,
  const vec3 &position,
  float rotation,
  TextureFactory &textureFactory) {
	return particles.insert(particles.begin(),
	                        new ParticleSystem(fileName,
	                                           textureFactory,
	                                           position,
	                                           rotation));
}

void ParticleEngine::emitGeometry() {
	vector<GeometryChunk> chunks;
	
	for (list<ParticleSystem*>::const_iterator i = particles.begin();
	     i != particles.end(); ++i) {
		ParticleSystem *system = *i;
		
		// Get geometry chunks (and associated materials) for this system
		system->getGeometryChunks(chunks);
		
		for (vector<GeometryChunk>::const_iterator i = chunks.begin();
		     i != chunks.end(); ++i) {
			RenderInstance instance;
			instance.gc = *i;
			instance.specificRenderMethod = METHOD_PARTICLE;
			instance.metaRenderMethod = TAG_UNSPECIFIED;
			
			ActionQueueRenderInstance action(instance);
			sendGlobalAction(&action);
		}
	}
}

void ParticleEngine::remove(handle h) {
	particles.erase(h);
}
