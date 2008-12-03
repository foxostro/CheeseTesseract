#ifndef _MESH_H_
#define _MESH_H_

#include "mat4.h"
#include "Face.h"
#include "Material.h"
#include "PropertyBag.h"
#include "ResourceBuffer.h"
#include "RenderInstance.h"

/** Holds mesh data and can render that data */
class Mesh
{
public:
	/** Destructor */
	~Mesh();

    /** Default constructor */
	Mesh();

	/**
	Copy constructor
	@param obj The object to copy from
	*/
	Mesh(const Mesh &obj);

	/**
	Copy constructor
	@param obj The object to copy from
	*/
	Mesh(const Mesh *obj);

	/**
	Constructor creates a mesh from a few vectors containing geometry data.
	@param verticesArray Vertices
	@param normalsArray Vertex normals
	@param texcoordsArray Tex-Coords
	@param facesArray Triangle indices
	@param completelyStatic The mesh will be created and then never modified.
	*/
	Mesh(const vector<vec3> &verticesArray,
	     const vector<vec3> &normalsArray,
	     const vector<vec2> &texcoordsArray,
	     const vector<Face> &facesArray,
		 bool completelyStatic=false);

		/**
		Constructor creates a mesh from a few vectors containing geometry data.
		@param verticesArray Vertices
		@param normalsArray Vertex normals
		@param texcoordsArray Tex-Coords
		@param facesArray Triangle indices
		@param completelyStatic The mesh will be created and then never modified.
		*/
	Mesh(const vector<vec3> &verticesArray,
	     const vector<vec3> &normalsArray,
	     const vector<vec2> &texcoordsArray,
	     const vector<color> &colorsArray,
		 const vector<Face> &facesArray,
		 bool completelyStatic=false);

	/**
	Assignment operator
	@param obj The object to copy from
	@return The assigned value
	*/
	Mesh& operator=(const Mesh &obj);

	/** Gets the static mesh backing this object */
	void getGeometryChunk(GeometryChunk &gc) const;

	/**
	Calculates the radius of the smallest sphere that encloses the object.
	@return radius
	*/
	inline float calculateSphericalRadius() const
    {
        return calculateRadius(&Mesh::radiusSpherical);
    }

	/**
	Calculates the radius of the smallest cylinder that encloses the object.
	@return radius
	*/
	inline float calculateCylindricalRadius() const
    {
        return calculateRadius(&Mesh::radiusCylindrical);
    }

	/**
	Calculates the height of the object.  This is done by calculating the
	difference between the maximum Y value and minimum Y value.
	@return height
	*/
	float calculateHeight() const;

	/**
	Calculates the height of the portions of the object below the y=0 plane
	(y=0 plane is ground)
	@return height
	*/
	float calculateHeightBelowGround() const;

	/**
	Creates an object interpolated from two other existing objects
	@param bias Interpolation bias between 0.0 and 1.0
	@param a The first keyframe
	@param b The second keyframe
	*/
	void interpolate(float bias, const Mesh &a, const Mesh &b);

    /**
    Sets the material to use for this mesh
    @param material Material to use
    @return The given material is passed through
    */
    inline const Material& setMaterial(const Material &_material)
    {
        return(material = _material);
    }

	/** Creates a geometry object in ODE that uses this mesh's geometry */
	tuple<dGeomID,dTriMeshDataID> createGeom(dSpaceID physicsSpace) const;

	void uniformScale(float scale);

private:
	/**
	Copies the object
	@param obj The object to copy from
	*/
	void copy(const Mesh &obj);

    typedef function<float(const vec3 &)> RadiusCalculator;

    static float radiusCylindrical(const vec3 &vertex)
    {
        return sqrtf(SQR(vertex.x) + SQR(vertex.y));
    }

    static float radiusSpherical(const vec3 &vertex)
    {
        return vertex.getMagnitude();
    }

    /** Calculates a radius for the mesh using the given calculator */
    float calculateRadius(RadiusCalculator radiusFn) const;

	/** Creates a geometry object in ODE that uses this mesh's geometry */
    static tuple<dGeomID,dTriMeshDataID> createGeom(dSpaceID physicsSpace,
                                                    const dVector3 *verts,
	                                                int numVertices,
	                                                const int *indices,
	                                                int numIndices);

public:
	Material material;
	ResourceBufferVerticesPtr vertexArray;
	ResourceBufferVerticesPtr normalArray;
	ResourceBufferTexCoordsPtr texCoordArray;
	ResourceBufferColorsPtr colorsArray;
    ResourceBufferIndicesPtr indexArray;
	GLenum polygonWinding;
};

#endif
