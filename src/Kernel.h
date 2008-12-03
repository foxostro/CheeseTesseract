#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "Task.h"

/** Collects and processes tasks that are run every tick */
class Kernel
{
public:
	typedef list< shared_ptr<Task> > TaskList;

	/** Destructor */
	~Kernel();

	/**
	Adds the task to the list of live tasks.
	@param task A game task. The task will be memory managed by the kernel
	*/
	void addTask(Task *task);

	/**
	Updates all tasks
	@param deltaTime seconds since the last update
	*/
	void update(float deltaTime);

	/** Deletes all tasks and resets state */
	void destroy();

private:
	/**
	Frees and removes dead tasks from the task queue
	@param tasks The task queue
	@return the updated task list
	*/
	static TaskList pruneDeadTasks(TaskList tasks);

	/**
	Update a single task
	@param task the task to update
	@param deltaTime The seconds since the last update
	*/
	static void updateTask(shared_ptr<Task> &task, float deltaTime);

private:
	/**	The list of live tasks */
	TaskList tasks;
};

#endif
