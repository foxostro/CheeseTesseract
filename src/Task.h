#ifndef _TASK_H_
#define _TASK_H_

/**
Task to be executed by the game engine
The kernel handles the game loop and calls the appropriate update functions every tick
*/
class Task
{
public:
    virtual ~Task(){}

	Task()
	{
		paused=false;
		dead=false;
	}

	/**
	Updates the task every tick as long as the task has not been frozen
	@param deltaTime The millesonds since the last tick
	*/
	virtual void update(float deltaTime)=0;

public:
	/**
	When true, the task is frozen by the kernel
	When false, the task is executed by the kernel
	*/
	bool paused;

	/** When true, the task is removed from the kernel */
	bool dead;
};

#endif
