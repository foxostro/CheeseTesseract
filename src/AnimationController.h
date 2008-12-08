#ifndef _ANIMATION_CONTROLLER_H_
#define _ANIMATION_CONTROLLER_H_

#include "AnimationSequence.h"

/** Manages a set of animation sequences */
class AnimationController {
private:
	/** Stores a list of all the model's animations */
	vector<AnimationSequence> animations;
	
	/**
	Records which of the above animation objects is currently being
	utilized by the model.
	*/
	size_t current;
	
public:
	/** Constructs a blank animation controller */
	AnimationController();
	
	/** Constructs a copy of another animation controller */
	AnimationController(const AnimationController &animationController);
	
	/** Get all the static meshes for this tick */
	void getGeometryChunks(vector<GeometryChunk> &m) const;
	
	/**
	Updates the current animation
	@param timeStep time since the last tick
	*/
	void update(float timeStep);
	
	/**
	Gets the specified animation
	@param handle the animation to retrieve
	*/
	const AnimationSequence& getAnimation(size_t handle) const;
	
	/**
	Gets the current animation
	@return The current animation
	*/
	const AnimationSequence& getAnimation() const;
	
	/**
	Gets the current animation
	@return The current animation
	*/
	AnimationSequence& getAnimation();
	
	/**
	Adds an animation sequence to the controller
	@param animation Animation sequence to add
	@return The animation handle
	*/
	size_t addAnimation(AnimationSequence &animation);
	
	/**
	Gets the handle of the current animation
	@return Current animation's handle
	*/
	size_t getAnimationHandle() {
		return current;
	}
	
	/**
	Gets the number of animations contained within the controller
	@return Number of animations
	*/
	size_t getNumAnimations() const {
		return animations.size();
	}
	
	/**
	Looks up an animation handle
	@param name The name of the animation
	@return The animation handle
	@throws std::invalid_argument When the animation does not exist
	*/
	size_t getAnimationHandle(const string &name) const;
	
	/**
	Records an animation request
	@param name The name of the animation
	@param speed The speed of the animation
	*/
	inline bool requestAnimationChange(const string &name, float speed=1.0f) {
		return requestAnimationChange(getAnimationHandle(name), speed);
	}
	
	/**
	Records an animation request
	@param handle The handle of the animation
	@param speed The speed of the animation
	*/
	bool requestAnimationChange(size_t handle, float speed=1.0f);
	
	/**
	Gets the time into the current animation
	@return time into the animation
	*/
	float getTime() const {
		return getAnimation().getTime();
	}
	
	/**
	Gets the duration of the current animation
	@return duration of the current animation
	*/
	float getDuration() const {
		return getAnimation().getLength();
	}
	
	/**
	Calculates and returns the radius of the smallest sphere entirely
	 enclosing the model.
	@param time The time into the current animation
	@return radius
	*/
	float calculateRadius();
	
	/**
	Calculates and returns the radius of the smallest cylinder entirely
	 enclosing the model.
	@param time The time into the current animation
	@return cylinder radius
	*/
	float calculateCylindricalRadius();
	
	/**
	Calculates and returns the height of the model
	@return height
	*/
	float calculateHeight();
	
	/** Creates a copy of this animation controller */
	inline shared_ptr<AnimationController> clone() const {
		AnimationController *a = new AnimationController(*this);
		return shared_ptr<AnimationController>(a);
	}
	
	void uniformScale(float scale);
	
private:
	/** Reset all variables to their default state */
	void clear();
};

#endif
