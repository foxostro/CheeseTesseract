#ifndef _CALLBACK_TASK_H_
#define _CALLBACK_TASK_H_

#include "Task.h"

/** After a delay period, a callback function is executed */
template < class FN >
class CallbackTask : public Task
{
public:
	/** Destructor */
	virtual ~CallbackTask()
	{}

	/**
	Constructs the task
	@param delay After this delay, the callback function is called
	@param fn The callback function
	*/
	CallbackTask(float delay, FN fn)
	{
		this->myAge = 0.0f;
		this->delay = delay;
		this->fn = fn;
	}

	/**
	Updates the task every tick as long as the task has not been frozen
	@param deltaTime The millesonds since the last tick
	*/
	void update(float deltaTime)
	{
		myAge += deltaTime;

		if(myAge >= delay)
		{
			dead = true;
			fn();
		}
	}

private:
	/** Tracks the age of the task (milliseconds) */
	float myAge;

	/** The time during which interpolation will occur*/
	float delay;

	/** Callback function */
	FN fn;
};

/**
Creates an interpolator
@param delay The callback function will be called after this delay
@param fn The callback function
*/
template<typename T>
static CallbackTask<T>* makeCallbackTask(float delay, T fn)
{
	return new CallbackTask<T>(delay, fn);
}

#endif
