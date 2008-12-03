#ifndef _LINEAR_INTERPOLATOR_H_
#define _LINEAR_INTERPOLATOR_H_

#include "Task.h"

/**
Interpolates a value over time.  Kernel task to cause a value
to be interpolated between a starting and ending value over a
period of time.  Once its started, it carries out its
execution seamlessly and without intervention.

While the interpolator is running, the value should be
considered read only.  Writes to the value will be overwritten
by the interpolator task on its next update.
*/
class LinearInterpolator : public Task
{
public:
	/**
	Constructs the interpolator
	@param value Points to the value to modify
	@param startingValue The starting value
	@param endingValue The ending value
	@param timeLimit The value will reach the end value at this time (milliseconds)
	*/
	LinearInterpolator(float *value, float startingValue, float endingValue, float timeLimit)
	{
		this->value = value;
		this->startingValue = startingValue;
		this->endingValue = endingValue;
		this->myAge = 0.0f;
		this->timeLimit = timeLimit;
	}

	/**
	Updates the task every tick as long as the task has not been frozen
	@param deltaTime The millesonds since the last tick
	*/
	void update(float deltaTime)
	{
		myAge += deltaTime;

		if(myAge < timeLimit)
		{
			float bias = myAge / timeLimit;

			(*value) = startingValue + bias*(endingValue - startingValue);
		}
		else
		{
			dead = true;

			(*value) = endingValue;
		}
	}

private:
	/** The value to be modified */
	float *value;

	/** The starting value*/
	float startingValue;

	/** The ending value */
	float endingValue;

	/** Tracks the age of the task (milliseconds) */
	float myAge;

	/** The time during which interpolation will occur*/
	float timeLimit;
};

#endif
