#ifndef _MD3LOADER_H_
#define _MD3LOADER_H_

#include "ModelLoaderSingle.h"

/** MD3 model loader */
class ModelLoaderMD3 : public ModelLoaderSingle
{
public:
	ModelLoaderMD3();

private:
	/** Max length of file names in the MD3 model */
	static const S32 MAX_QPATH = 64;

	/** The max number of frames that may be in the model */
	static const S32 MD3_MAX_FRAMES;

	/** The max number of surfaces that may be in the model */
	static const S32 MD3_MAX_SURFACES;

	/** The max number of tags */
	static const S32 MD3_MAX_TAGS;

	/** The max number of shaders */
	static const S32 MD3_MAX_SHADERS;

	/** The max number of vertices */
	static const S32 MD3_MAX_VERTS;

	/** The max number of triangles */
	static const S32 MD3_MAX_TRIANGLES;

	/** Scaling for vertices */
	static const float MD3_XYZ_SCALE;

	/** Header to a MD3 file */
	struct Header
	{
		/** Magic number.  Should read "IDP3".  Will not be null-terminated */
		U8 magicNumber[4];

		/** MD3 version number */
		S32 version;

		/** Name of the MD3 object, usually its file name.  Null-terminated */
		U8 name[MAX_QPATH];

		/** ??? */
		S32 flags;

		/** Number of frame objects */
		S32 numFrames;

		/** Number of tag objects */
		S32 numTags;

		/** Number of surface objects */
		S32 numSurfaces;

		/** Number of skin objects (unused) */
		S32 numSkins;

		/** Offset from the start of the MD3 object where the Frame objects start */
		S32 framesOffset;

		/** Offset from the start of the MD3 object where the Tag objects start */
		S32 tagsOffset;

		/** Offset from the start of the MD3 object where the Surfaces objects start */
		S32 surfacesOffset;

		/** Offset from the start of the MD3 object where the MD3 object ends */
		S32 eofOffset;
	};

	/** Structure to store a Frame object */
	struct Frame
	{
		/** First corner of the bounding box */
		VEC3 minBounds;

		/** Second corner of the bounding box */
		VEC3 maxBounds;

		/** Local origin */
		VEC3 localOrigin;

		/** Radius of bounding sphere */
		F32 radius;

		/** Name of the Frame */
		U8 name[16];
	};

	/** Structure to hold a Tag object */
	struct Tag
	{
		/** Name of Tag object */
		U8 name[MAX_QPATH];

		/** Coordinates of Tag object */
		VEC3 origin;

		/** Orthonormal basis of the Tag */
		VEC3 axis[3];
	};

	/** MD3 surface object */
	class Surface
	{
	public:
		/** Default constructor */
		Surface();

		/** Destructor */
		~Surface();

		/**
		Loads a Surface from file
		@param stream The file to load the surface from.
		Assume the file cursor is set at the start of the surface already
		*/
		void create(FILE *stream);

		/**
		Allocates memory for a mesh object and copies the Surface into it.
		@param frame Frame index
		@return the mesh object.  memory-management is the responsibility of the caller.

		@todo trignometry (an issue of loading time)

		There are 255 discrete values for encodedNormal.  So
		sine/cosine precision is not really an issue.  Do we
		need to use the std lib?  A little McClaurin series
		would be much more efficient.
		*/
		Mesh* getObject(size_t frame) const;

	private:
		/** Structure to hold the header to a Surface object */
		struct SurfaceHeader
		{
			/** Magic number.  Should read "IDP3".  Will not be null-terminated */
			U8 magicNumber[4];

			/** Name of the Surface object.  null-terminated */
			U8 name[MAX_QPATH];

			/** ??? */
			S32 flags;

			/** Number of animation frames.  This should match numFrames in the header */
			S32 numFrames;

			/** Number of shader objects within the Surface */
			S32 numShaders;

			/** Number of Vertex objects defined in this Surface */
			S32 numVerts;

			/** Number of Triangle objects defined within this Surface */
			S32 numTris;

			/** Offset from the Surface start where the list of Triangle objects begin */
			S32 trianglesOffset;

			/** Offset from the Surface start where the list of Shader objects begin */
			S32 shadersOffset;

			/** Offset from the Surface start where the list of tex-coords begin */
			S32 texCoordsOffset;

			/** Offset from the Surface start where the list of vertices begin */
			S32 vertexOffset;

			/** Offset from the Surface start where the Surface will end */
			S32 endOffset;
		};

		/** Structure to hold a Shader object*/
		struct Shader
		{
			/** Pathname of the shader  Null-terminated */
			U8 name[MAX_QPATH];

			/** The index of the shader */
			S32 shaderIndex;
		};

		/** Structure to hold a Triangle object */
		struct Triangle
		{
			/** Offset values into the list of verte objects */
			S32 indices[3];
		};

		/** Structure to hold a TexCoord object */
		struct TexCoord
		{
			/** Texture coodinate */
			F32 st[2];
		};

		/** Structure to hold an MD3 vertex */
		struct Vertex
		{
			S16 x, y, z, encodedNormal;
		};

	public:
		/** Header of the surface */
		SurfaceHeader header;

		/** Shaders list */
		Shader* shaders;

		/** Triangles list */
		Triangle* triangles;

		/** TexCoord list */
		TexCoord* texCoords;

		/** Vertex list */
		Vertex* vertices;

        /** Rotate and scale the vertex position*/
        static vec3 transformVertex(const Vertex &v);

        /** Decode the compressed normal */
        static vec3 transformNormal(const Vertex &v);

        /** Reflect the tex-coord to work correctly with OpenGL */
        static vec2 transformTexCoord(const TexCoord &uv);

        /** Get offsets into the geometry arrays for one face */
        static Face transformTriangle(const Triangle &triangle);

        /** Transform array of MD3 vertices into our vertices */
        static vector<vec3> getVertices(const Vertex *v, size_t count);

        /** Transform array of MD3 vertices into our normals */
        static vector<vec3> getNormals(const Vertex *v, size_t count);

        /** Transform array of MD3 tex-coords into our tex-coords */
        static vector<vec2> getTexCoords(const TexCoord *uv,
                                         size_t count);

        /** Transform array of MD3 faces into our face structures */
        static vector<Face> getFaces(const Triangle *triangles,
                                     size_t count);
	};

private:
	/**
	Loads all key frames from an MD3 file
	@param fileName File name of some MD3 file
	@param textureFactory Texture factory tracks loaded textures
	@return key frames
	*/
	virtual vector<KeyFrame> loadKeyFrames(const FileName &fileName,
	                                       const FileName &skinName,
	                                       TextureFactory &tex) const;

	/** Builds a key frame from the surfaces */
	static vector<KeyFrame> buildKeyFrame(Surface * surfaces,
	                                      const FileName &fileName,
	                                      const FileName &skinName,
										  const Header &header,
										  TextureFactory &textureFactory);

private:
	static Surface* readSurfaces(FILE* stream, const Header &header);
	static Tag* readTags(FILE* stream, const Header &header);
	static Frame* readFrames(FILE* stream, const Header &header);
	static Header readHeader(FILE* stream);
};

#endif
