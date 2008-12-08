#include "stdafx.h"
#include "ParticleElement.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const FileName &fileName,
                               TextureFactory &textureFactory,
                               const vec3 &_position,
                               float _rotation) {
	reset();
	rotation = _rotation;
	const PropertyBag data = PropertyBag::fromFile(fileName);
	maxNumberOfParticles = data.getSizeT("max");
	loadParticleMaterials(data, textureFactory);
	loadParticleTemplates(data);
	loadParticleEmitters(data);
	setPosition(_position);
	
	// allocate memory for all particle batches
	for (map<string, ParticleElement>::const_iterator i=templatesByName.begin(); i!=templatesByName.end(); ++i) {
		const ParticleElement &el = i->second;
		ParticleBatch &batch = buckets[el.getMaterial()];
		
		batch.geometry.colorsArray   -> recreate(4 * (int)maxNumberOfParticles, 0, DYNAMIC_DRAW);
		batch.geometry.vertexArray   -> recreate(4 * (int)maxNumberOfParticles, 0, DYNAMIC_DRAW);
		batch.geometry.texCoordArray -> recreate(4 * (int)maxNumberOfParticles, 0, DYNAMIC_DRAW);
	}
}

void ParticleSystem::destroyEmitters() {
	for (EmitterSet::const_iterator i=emitters.begin(); i!=emitters.end(); ++i) {
		delete *i;
	}
	
	emitters.clear();
}

ParticleSystem::~ParticleSystem() {
	destroyEmitters();
}

void ParticleSystem::update(float milliseconds, Camera &camera) {
	updateEmitters(milliseconds);
	updateParticles(milliseconds);
	
	const mat3 &cameraOrientation = camera.getOrientation();
	mat4 modl(camera.getPosition(),
	          cameraOrientation.getAxisX(),
	          cameraOrientation.getAxisY(),
	          cameraOrientation.getAxisZ());
	updateParticleBatches(modl);
}

void ParticleSystem::spawn(const ParticleElement &el) {
	ParticleBatch &bucket = buckets[el.getMaterial()];
	ParticleSet &set = bucket.elements;
	
	if (set.size() < maxNumberOfParticles) {
		set.insert(set.begin(), el);
	}
}

const ParticleElement& ParticleSystem::getTemplate(const string &name) {
	ASSERT(templatesByName.find(name)!=templatesByName.end(),
	       "Particle template could not be found: " + name);
	       
	return(templatesByName.find(name)->second);
}

bool ParticleSystem::isDead() const {
	for (EmitterSet::const_iterator i=emitters.begin();
	     i!=emitters.end(); ++i) {
		ParticleEmitter *emitter = *i;
		
		if (!emitter->isDead()) {
			return false;
		}
	}
	
	for (ParticleBuckets::const_iterator j=buckets.begin();
	     j!=buckets.end(); ++j) {
		const ParticleBatch &bucket = j->second;
		const ParticleSet &set = bucket.elements;
		
		for (ParticleSet::const_iterator i=set.begin(); i!=set.end(); ++i) {
			if (!i->isDead()) {
				return false;
			}
		}
	}
	
	return true;
}

void ParticleSystem::kill() {
	for (EmitterSet::const_iterator i=emitters.begin();
	     i!=emitters.end(); ++i) {
		ParticleEmitter *emitter = *i;
		emitter->kill();
	}
	
	buckets.clear();
}

void ParticleSystem::updateEmitters( float milliseconds ) {
	for (EmitterSet::const_iterator i=emitters.begin(); i!=emitters.end(); ++i) {
		ParticleEmitter *emitter = *i;
		emitter->update(milliseconds);
	}
}

void ParticleSystem::updateParticles( float milliseconds ) {
	for (ParticleBuckets::iterator j=buckets.begin();
	     j!=buckets.end(); ++j) {
		ParticleBatch &bucket = j->second;
		ParticleSet &set = bucket.elements;
		
		ParticleSet::iterator i=set.begin();
		
		while (i != set.end()) {
			if (i->isDead()) {
				i = set.erase(i);
			} else {
				i->update(milliseconds);
				++i;
			}
		}
	}
}

void ParticleSystem::reset() {
	destroyEmitters();
	
	templatesByName.clear();
	materials.clear();
	maxNumberOfParticles = 0;
	buckets.clear();
	position.zero();
	rotation = 0;
}

void ParticleSystem::loadParticleMaterials(const PropertyBag &data,
  TextureFactory &textureFactory) {
	const size_t nMaterials = data.getNumInstances("material");
	ASSERT(nMaterials>0, "particle system does not specify any materials");
	for (size_t i=0; i<nMaterials; ++i) {
		PropertyBag MatBag = data.getBag("material", i);
		Material material;
		const string name = MatBag.getString("name");
		const FileName fileName = MatBag.getString("image");
		material.setTexture(textureFactory.load(fileName, false));
		material.glow = MatBag.getBool("glow");
		materials.insert(make_pair(name, material));
	}
	ASSERT(!materials.empty(),
	       "after loading, there are no particle materials in system");
}

void ParticleSystem::loadParticleTemplates(const PropertyBag &data) {
	const size_t n = data.getNumInstances("template");
	for (size_t i=0; i<n; ++i) {
		const PropertyBag templateData = data.getBag("template", i);
		const string templateName = templateData.getString("name");
		const string materialName = templateData.getString("material");
		
		Material *material = getMaterialPtr(materialName);
		
		ParticleElement element(templateData, material);
		
		templatesByName.insert(make_pair(templateName, element));
	}
}

void ParticleSystem::loadParticleEmitters(const PropertyBag &data) {
	const size_t nEmitters = data.getNumInstances("emitter");
	ASSERT(nEmitters>0, "particle system does not specify any emitters");
	for (size_t i=0; i<nEmitters; ++i) {
		const PropertyBag emitterData = data.getBag("emitter", i);
		ParticleEmitter *emitter = new ParticleEmitter(emitterData, this);
		emitters.push_back(emitter);
	}
}

void ParticleSystem::updateParticleBatches(const mat4 &matrix) {
	for (ParticleBuckets::const_iterator j=buckets.begin();
	     j!=buckets.end(); ++j) {
		const ParticleBatch &batch = j->second;
		const ParticleSet &set = batch.elements;
		
		color *colors = (color*)batch.geometry.colorsArray->lock();
		vec2 *texcoords = (vec2*)batch.geometry.texCoordArray->lock();
		vec3 *vertices = (vec3*)batch.geometry.vertexArray->lock();
		
		// Build particle vertices, etc
		for (ParticleSet::const_iterator i=set.begin(); i!=set.end(); ++i) {
			const ParticleElement &el = *i;
			
			// update colors
			colors[0] =
			 colors[1] =
			  colors[2] =
			   colors[3] = el.getColor();
			   
			// updates vertices
			el.buildBillboardVertices(matrix, vertices[0],
			                          vertices[1],
			                          vertices[2],
			                          vertices[3]);
			                          
			// update texcoords
			texcoords[0] = vec2(0.0f, 0.0f);
			texcoords[1] = vec2(1.0f, 0.0f);
			texcoords[2] = vec2(1.0f, 1.0f);
			texcoords[3] = vec2(0.0f, 1.0f);
			
			colors+=4;
			vertices+=4;
			texcoords+=4;
		}
		
		batch.geometry.vertexArray->unlock();
		batch.geometry.texCoordArray->unlock();
		batch.geometry.colorsArray->unlock();
	}
}

void ParticleSystem::getGeometryChunks(vector<GeometryChunk> &chunks) {
	chunks.clear();
	
	for (ParticleBuckets::const_iterator j=buckets.begin();
	     j!=buckets.end(); ++j) {
		const ParticleBatch &batch = j->second;
		Material *material = j->first;
		
		const GLsizei count = (GLsizei)batch.elements.size();
		
		if (count>0) {
			GeometryChunk gc;
			
			batch.geometry.getGeometryChunk(gc);
			
			gc.primitiveMode = GL_QUADS;
			
			if (material) {
				gc.material = *material;
			}
			
			chunks.push_back(gc);
		}
	} // loop
}
