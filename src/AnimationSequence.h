#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "mat4.h"
#include "Mesh.h"
#include "KeyFrame.h"

/** Records keyframe timing and arrangement for ana animation */
class AnimationSequence
{
public:
	/** Destructor */
	~AnimationSequence();

	/**
	Loads the AnimationSequence from an XML source
	@param keyFrames All keyframes in the mesh
	@param name The name of the animation
	@param priority The priority of the animation.
                    If priority > 1, then the animation is "high priority"
	@param looping true if the animation loops
	@param start Index into keyFrames for the start of the animation sequence
	@param length Number of keyframes in the animation sequence
	@param fps The frames per second of the animation sequence
	*/
	AnimationSequence(const vector<KeyFrame> &keyFrames,
                      const string &name,
                      float priority,
                      bool looping,
                      size_t start,
                      size_t length,
                      float fps);

    /**
	Loads the AnimationSequence from an XML source
	@param keyFrames All keyframes in the mesh
	@param name The name of the animation
	@param priority The priority of the animation.
                    If priority > 1, then the animation is "high priority"
	@param looping true if the animation loops
	@param start Index into keyFrames for the start of the animation sequence
	@param length Number of keyframes in the animation sequence
	@param fps The frames per second of the animation sequence
	*/
	AnimationSequence(const vector<KeyFrame> &keyFrames,
                      const string &name,
                      float priority,
                      bool looping,
                      float fps);

	/**
	Copy constructor
	@param animation The animation to copy
	*/
	AnimationSequence(const AnimationSequence &animation);

	/** Gets the name of the animation */
	inline const string& getName() const
	{
		return m_strName;
	}

	/** Gets the animation's priority */
	inline float getPriority() const
	{
		return m_Priority;
	}

	/** Gets the current time into the animation */
	inline float getTime() const
	{
		return m_Time;
	}

	/**
	Gets the length of the animation in milliseconds
	@return milliseconds
	*/
	inline float getLength() const
	{
		return keyFrames.size() * 1000.0f / fps;
	}

	/**
	Indicates whether or not the animation will
	be allowed to always replace the current one.
	@return true if so
	*/
	inline bool isHighPriority() const
	{
		return(m_Priority>1.0f);
	}

	/**
	Gets the animation FPS
	@return fps
	*/
	inline float getFPS() const
	{
		return fps;
	}

	/** Gets the time scalar of the animation */
	inline float getSpeed() const
	{
		return m_TimeScalar;
	}

	/** Indicates whether or not the animation has finished yet */
	inline bool isFinished() const
	{
		return m_bFinished;
	}

	/** Indicates whether or not the animation is looping */
	inline bool isLooping() const
	{
		return m_bLooping;
	}

	/**
	Gets the number of key frames in the animation
	@return The number of key frames
	*/
	inline size_t getNumKeyFrames() const
	{
		return keyFrames.size();
	}

	/** Get all the static meshes for this tick */
	void getGeometryChunks(vector<GeometryChunk> &m) const;

	/**
	Updates the animation
	@param timeDelta The time elapsed since the last update
	*/
	void update(float timeDelta);

	/** Change the time into animation */
	void setTime(float time);

	/** Change the speed of the animation */
	inline void setSpeed(float timeScalar)
	{
		m_TimeScalar = timeScalar;
	}

    inline float calculateCylindricalRadius() const
    {
        return calculateRadius(&Mesh::calculateCylindricalRadius);
    }

    inline float calculateRadius() const
    {
        return calculateRadius(&Mesh::calculateSphericalRadius);
    }

	float calculateHeight() const;

	/** Creates a geometry object in ODE that uses this mesh's geometry */
	tuple<dGeomID,dTriMeshDataID> createGeom(dSpaceID physicsSpace) const
	{
		return meshes[0]->createGeom(physicsSpace);
	}

	void uniformScale(float scale);

private:
    /**
    Calculates the radius of the mesh using the given radius calculation func.
    (e.g. Cylindrical radius or spherical radius)
    @param time Time (seconds) into the animation
    @param fn
    @return Radius of the mesh
    */
    float calculateRadius(function<float(Mesh *)> fn) const;

	/**
	Copy the meshes to create a working set.
	We assume all keyframes have the same number of meshes and each
    corresponding mesh is identical except in vertex placement
	@param model keyframes meshes
	*/
	void createWorkingSetOfMeshes(const vector<Mesh*> &model);

	/**
	Gets the frame of animation for the specified time
	@param millisecondsIntoAnimation The time into the animation
	@return The frame of animation
	*/
	const vector<Mesh*>& getFrame(float millisecondsIntoAnimation) const;

	/**
	Gets the frame of animation by interpolating between the given key frames
	@param lowerFrame Index of the lower key frame
	@param upperFrame Index of the upper key frame
	@param bias 0.0 to 1.0 indicating the bias between the key frames
	@return The frame of animation
	*/
	const vector<Mesh*>& getFrame(size_t lowerFrame,
                                  size_t upperFrame,
                                  float bias) const;

	/**
	Gets the frame of animation for the current time
	@return The frame of animation
	*/
	inline const vector<Mesh*>& getFrame() const
	{
		return getFrame(getTime());
	}

    static void interpolate(const float bias,
	                        Mesh &o,
							const Mesh &a,
							const Mesh &b)
    {
        o.interpolate(bias, a, b);
    }

	/** A working copy of the mesh */
	mutable vector<Mesh*> meshes;

	/** The key frames of the animation */
	vector<KeyFrame> keyFrames;

	/** The name of the animation */
	string m_strName;

	/** Used for determining the chosen animation during request conflicts */
	float m_Priority;

	/** Time into the animation (milliseconds) */
	float m_Time;

	/** Multiply all Time Elapsed values by this to control animation speed */
	float m_TimeScalar;

	/** Whether or not the animation loops at the end of the sequence. */
	bool m_bLooping;

	/** base animation FPS */
	float fps;

	/**
	Flags whether the animation has been completed
	If m_bLooping, then this is always false
	*/
	bool m_bFinished;
};

#endif
