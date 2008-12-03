#ifndef _MD2LOADER_H_
#define _MD2LOADER_H_

#include "ModelLoaderSingle.h"

/**
MD2 model loader
Source for specs: <http://tfc.duke.free.fr/coding/md2-specs-en.html>
*/
class ModelLoaderMD2 : public ModelLoaderSingle
{
public:
	ModelLoaderMD2();

private:
	static const S32 MD2_MAX_TRIANGLES;
	static const S32 MD2_MAX_VERTS;
	static const S32 MD2_MAX_TEX_VERTS;
	static const S32 MD2_MAX_FRAMES;
	static const S32 MD2_MAX_SKINS;
	static const VEC3 anorms_table[162];

	/** MD2 header */
	struct Header
	{
		S32 magicNumber;            /** magic number: "IDP2" */
		S32 version;                /** version: must be 8 */

		S32 skinWidth;              /** texture width */
		S32 skinHeight;             /** texture height */

		S32 frameSize;              /** size in bytes of a frame */

		S32 numOfSkins;             /** number of skins */
		S32 numOfVertices;          /** number of vertices per frame */
		S32 numOfSt;                /** number of texture coordinates */
		S32 numOfTris;              /** number of triangles */
		S32 numOfGL;                /** number of opengl commands */
		S32 numOfFrames;            /** number of frames */

		S32 offsetSkins;            /** offset skin data */
		S32 offsetTexCoords;        /** offset texture coordinate data */
		S32 offsetTriangles;        /** offset triangle data */
		S32 offsetFrames;           /** offset frame data */
		S32 offsetGL;               /** offset OpenGL command data */
		S32 offsetEnd;              /** offset end of file */
	};

	/** Texture name */
	struct Skin
	{
		U8 name[64];
	};

	/* Texture coords */
	struct TexCoord
	{
		S16 s, t;
	};

	/* Triangle */
	struct Triangle
	{
		U16 indices[3];
		U16 st[3];
	};

	/** Structure to hold an MD2 Vertex */
	struct Vertex
	{
		U8 x, y, z, encodedNormal;
	};

	/** Model frame */
	struct Frame
	{
		VEC3 scale;        /** scale factor */
		VEC3 translate;    /** translation vector */
		U8 name[16];       /** frame name */
		Vertex *vertices;  /** list of frame's vertices */
	};

private:
	/**
	Loads all keyframes from an MD2 file
	@param fileName File name of the MD2 file
	@param skinName Default skin for the model
	@param textureFactory Texture factory tracks loaded textures
	@return key frames
	*/
	virtual vector<KeyFrame> loadKeyFrames(const FileName &fileName,
	                                       const FileName &skinName,
	                                       TextureFactory &tex) const;

	/**
	Build all keyframes from the given model data
	@param fileName File name of the MD2 file
	@param skin MD2 skin
	@param header MD2 model information.
	              Specifies sizes for the arrays that follow below.
	@param texCoords Array of all tex coords in the MD2
	@param triangles Array of all Triangles/Faces in the MD2
	@param frames Array of all frames in the MD2
	@return key frames
	*/
	static vector<KeyFrame> buildKeyFrames(const FileName &fileName,
	                                       const Material &skin,
	                                       const Header &header,
	                                       TexCoord *texCoords,
										   Triangle *triangles,
										   Frame *frames);

	/**
	Build all keyframes from the given model data
	@param skin MD2 skin
	@param header MD2 model information.
	Specifies sizes for the arrays that follow below.
	@param texCoords Array of all tex coords in the MD2
	@param triangles Array of all Triangles/Faces in the MD2
	@param frames Array of all frames in the MD2
	@return key frame mesh
	*/
	static Mesh* buildKeyFrame(const Material &skin,
						       const Header &header,
						       TexCoord *texCoords,
						       Triangle *triangles,
						       const Frame &frame);

private:
	static Header readHeader(FILE* stream);
	static Skin* readSkins(FILE* stream, const Header &header);
	static TexCoord* readTexCoords(FILE* stream, const Header &header);
	static Triangle* readTriangles(FILE* stream, const Header &header);
	static Frame* readFrames(FILE* stream, const Header &header);

	static vec3 transformVertex(const Vertex &v, VEC3 *scale, VEC3 *translate);
	static vec3 transformNormal(const Vertex &v);
	static vec2 transformTexCoord(const TexCoord &uv, S32 w, S32 h);
	static Face transformTriangle(const Triangle &triangle);

	static vector<vec3> getVertices(const Vertex *v, const size_t count, VEC3 scale, VEC3 translate);
	static vector<vec3> getNormals(const Vertex *v, const size_t count);
	static vector<vec2> getTexCoords(const TexCoord *uv, const size_t count, S32 w, S32 h);
	static vector<Face> getFaces(const Triangle *tris, const size_t count);
};

#endif
