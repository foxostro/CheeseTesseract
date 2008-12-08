#include "stdafx.h"
#include "GrassLayer.h"
#include "RenderMethodTags.h"
#include "ActionQueueRenderInstance.h"
#include "Frustum.h"
#include "Renderer.h"

extern Frustum g_Frustum;

GrassLayer::~GrassLayer() {
	delete [] cells;
}

GrassLayer::GrassLayer(shared_ptr<class Renderer> renderer,
                       TextureFactory &textureFactory) {
	numCellColumns = 0;
	numCellRows = 0;
	cells = 0;
	
	useShader=false; // default
	
	if (renderer) {
		/*
		If the renderer chose a shader based method for grass,
		we need to supply the appropriate geometry.
		*/
		RENDER_METHOD method = renderer->getMethodFromTag(TAG_GRASS);
		useShader = method != METHOD_GRASS_FFP;
	}
	
	// Setup the grass material
	grassMaterial = Material(textureFactory.load(FileName("data/textures/grassPack.dds")));
	grassMaterial.glow = false;
	grassMaterial.Ka = color(0.2f, 0.2f, 0.2f, 1.0f);
	grassMaterial.Kd = color(1.0f, 1.0f, 1.0f, 1.0f);
	grassMaterial.Ks = color(0.0f, 0.0f, 0.0f, 1.0f); // no specular
	grassMaterial.shininess = 16.0f;
	
	// Grass generation parameters
	spacing = vec2(0.35f, 0.35f);
	displacement = 0.20f;
	size_mu = 1.0f;
	size_sigma = 0.2f;
	size_constant_offset = 0.0f;
	size_multiplier = 0.5f;
}

void GrassLayer::generate(const vec2 &center,
                          float size,
                          function<tuple<float,vec3>(vec2)> elevationFunc) {
	int s = (int)ceilf(size / 8.0f);
	numCellColumns = s;
	numCellRows = s;
	cells = new struct GrassCell[numCellRows * numCellColumns];
	
	float cellSpacing = size / s;
	float cellSize = cellSpacing / 2.0f;
	
	for (int col = 0; col < numCellColumns; ++col) {
		for (int row = 0; row < numCellRows; ++row) {
			GrassCell &cell = cells[numCellRows*col + row];
			generateCell(cell,
			             vec2(col + 0.5f, row + 0.5f) * cellSpacing,
			             cellSize,
			             elevationFunc);
		}
	}
}

void GrassLayer::generateCell(struct GrassCell &cell,
                              const vec2 &center,
                              float size,
                              function<tuple<float,vec3>(vec2)> elevationFunc) {
	vec2 bounds_min = center - vec2(size, size);
	vec2 bounds_max = center + vec2(size, size);
	
	cell.size = size;
	cell.center = center;
	
	shared_ptr<Mesh> mesh = generateMesh(generateTufts(bounds_min,
	                                     bounds_max,
	                                     elevationFunc));
	                                     
	mesh->getGeometryChunk(cell.renderInstance.gc);
	
	// Use grass material generated earlier
	cell.renderInstance.gc.material = grassMaterial;
	
	// If we generated shader geometry, request the shader
	if (useShader) {
		cell.renderInstance.specificRenderMethod = METHOD_GRASS_CG;
		cell.renderInstance.metaRenderMethod = TAG_UNSPECIFIED;
	} else {
		cell.renderInstance.specificRenderMethod = METHOD_GRASS_FFP;
		cell.renderInstance.metaRenderMethod = TAG_UNSPECIFIED;
	}
}

shared_ptr<Mesh> GrassLayer::generateMesh( const vector<GrassTuft> &elements ) {
	vector<vec3> verticesArray;
	vector<vec3> normalsArray;
	vector<vec2> texcoordsArray;
	vector<color> colorsArray;
	vector<Face> facesArray;
	
	int idx=0;
	
	for (vector<GrassTuft>::const_iterator i = elements.begin();
	     i != elements.end(); ++i) {
		const GrassTuft &tuft = *i;
		
		if (useShader) {
			// Add a single quad and the VS will billboard it for us
			addQuad(tuft,
			        idx,
			        0.0f,
			        verticesArray,
			        normalsArray,
			        texcoordsArray,
			        colorsArray,
			        facesArray);
		} else {
			// Add several crossed quads to make a tuft
			float baseRotation = FRAND_RANGE(0.0f, (float)(M_PI * 2.0));
			for (int quad=0; quad<3; ++quad) {
				addQuad(tuft,
				        idx,
				        angle_clamp((quad/4.0f) + baseRotation),
				        verticesArray,
				        normalsArray,
				        texcoordsArray,
				        colorsArray,
				        facesArray);
			}
		}
	}
	
	// Generate mesh object from this geometry
	shared_ptr<Mesh> mesh;
	mesh = shared_ptr<Mesh>(new Mesh(verticesArray,
	                                 normalsArray,
	                                 texcoordsArray,
	                                 colorsArray,
	                                 facesArray,
	                                 true)); // completely static buffers
	return mesh;
}

vector<GrassLayer::GrassTuft>
GrassLayer::generateTufts(const vec2 &bounds_min,
                          const vec2 &bounds_max,
                          function<tuple<float,vec3>(vec2)> elevationFunc) {
	vector<GrassTuft> tufts;
	vec2 pos;
	
	for (pos.y = bounds_min.y; pos.y < bounds_max.y; pos.y += spacing.y) {
		for (pos.x = bounds_min.x; pos.x < bounds_max.x; pos.x += spacing.x) {
			GrassTuft tuft;
			
			float angle = FRAND_RANGE(0.0f, (float)(M_PI * 2.0));
			vec2 offset = vec2(cos(angle), sin(angle)) * SampleNormal(displacement, 0.0f);
			vec2 tuftPos =  pos + offset;
			
			tuft.size = generateTuftSize();
			tuft.type = IRAND_RANGE(0,3);
			
			tuple<float, vec3> zn = elevationFunc(tuftPos);
			tuft.position = vec3(tuftPos, zn.get<0>());
			tuft.terrainNormal = zn.get<1>();
			
			tufts.push_back(tuft);
		}
	}
	
	return tufts;
}

void GrassLayer::addQuad(const GrassTuft &tuft,
                         int &idx,
                         float angle,
                         vector<vec3> &verticesArray,
                         vector<vec3> &normalsArray,
                         vector<vec2> &texcoordsArray,
                         vector<color> &colorsArray,
                         vector<Face> &facesArray) {
	if (!useShader) {
		float s = tuft.size * 3;
		mat3 r = mat3::fromRotateZ(angle);
		vec3 a = r.transformVector(vec3(+0.5f,0.0f,0.5f));
		vec3 b = r.transformVector(vec3(+0.5f,0.0f,0.0f));
		vec3 c = r.transformVector(vec3(-0.5f,0.0f,0.0f));
		vec3 d = r.transformVector(vec3(-0.5f,0.0f,0.5f));
		
		verticesArray.push_back(tuft.position + a * s);
		verticesArray.push_back(tuft.position + b * s);
		verticesArray.push_back(tuft.position + c * s);
		verticesArray.push_back(tuft.position + d * s);
	} else {
		// The shader will transform these into a billboarded quad for us
		verticesArray.push_back(tuft.position);
		verticesArray.push_back(tuft.position);
		verticesArray.push_back(tuft.position);
		verticesArray.push_back(tuft.position);
	}
	
	// Normals all correspond to the underlying terrain normals
	normalsArray.push_back(tuft.terrainNormal);
	normalsArray.push_back(tuft.terrainNormal);
	normalsArray.push_back(tuft.terrainNormal);
	normalsArray.push_back(tuft.terrainNormal);
	
	// Select texture coords to choose grass out of a texture atlas
	texcoordsArray.push_back(vec2(0.25f + tuft.type*0.25f, 1.0f));
	texcoordsArray.push_back(vec2(0.25f + tuft.type*0.25f, 0.0f));
	texcoordsArray.push_back(vec2(0.00f + tuft.type*0.25f, 0.0f));
	texcoordsArray.push_back(vec2(0.00f + tuft.type*0.25f, 1.0f));
	
	if (useShader) {
		// Colors are used to transfer vertex attributes to the shader program
		// RED channel is the corner indicator
		// GREEN channel is the quad size
		colorsArray.push_back(color(1.0, tuft.size, 1.0, 1.0));
		colorsArray.push_back(color(2.0, tuft.size, 1.0, 1.0));
		colorsArray.push_back(color(3.0, tuft.size, 1.0, 1.0));
		colorsArray.push_back(color(4.0, tuft.size, 1.0, 1.0));
	} else {
		colorsArray.push_back(white);
		colorsArray.push_back(white);
		colorsArray.push_back(white);
		colorsArray.push_back(white);
	}
	
	// Generate the faces of the quad
	{
		Face face;
		face.vertIndex[0] = face.normalIndex[0] = face.coordIndex[0] = idx+0;
		face.vertIndex[1] = face.normalIndex[1] = face.coordIndex[1] = idx+1;
		face.vertIndex[2] = face.normalIndex[2] = face.coordIndex[2] = idx+3;
		facesArray.push_back(face);
	}
	
	{
		Face face;
		face.vertIndex[0] = face.normalIndex[0] = face.coordIndex[0] = idx+2;
		face.vertIndex[1] = face.normalIndex[1] = face.coordIndex[1] = idx+3;
		face.vertIndex[2] = face.normalIndex[2] = face.coordIndex[2] = idx+1;
		facesArray.push_back(face);
	}
	
	idx+=4;
}

float GrassLayer::generateTuftSize() {
	// http://en.wikipedia.org/wiki/Lognormal_distribution
	return SampleLogNormal(size_mu, size_sigma)*size_multiplier
	       + size_constant_offset;
}

void GrassLayer::emitGeometry(vector<ActionQueueRenderInstance> &batches)const {
	batches.reserve(numCellColumns * numCellRows);
	
	for (int col = 0; col < numCellColumns; ++col)
		for (int row = 0; row < numCellRows; ++row) {
			const GrassCell &cell = cells[numCellRows*col + row];
			
			if (g_Frustum.isCubeWithin(vec3(cell.center.x, cell.center.y, 0.0), cell.size)) {
				ActionQueueRenderInstance action(cell.renderInstance);
				batches.push_back(action);
			}
		}
}
