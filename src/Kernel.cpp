#include "stdafx.h"
#include "Task.h"
#include "Kernel.h"

Kernel::~Kernel()
{
	destroy();
}

void Kernel::addTask(Task *pTask)
{
	ASSERT(pTask, "Parameter \"pTask\" was NULL");
	shared_ptr<Task> task(pTask);
	tasks.push_back(task);
}

void Kernel::update(float deltaTime)
{
	for(TaskList::const_iterator i=tasks.begin(); i!=tasks.end(); ++i)
	{
		shared_ptr<Task> task(*i);
		updateTask(task, deltaTime);
	}

	tasks = pruneDeadTasks(tasks);
}

void Kernel::destroy()
{
	tasks.clear();
}

Kernel::TaskList Kernel::pruneDeadTasks(TaskList tasks)
{
	TaskList::iterator iter = tasks.begin();

	while(iter != tasks.end())
	{
		shared_ptr<Task> task(*iter);

		if((task && task->dead) || !task)
		{
			iter = tasks.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	return tasks;
}

void Kernel::updateTask(shared_ptr<Task> &task, float deltaTime)
{
	if(task && !task->dead && !task->paused)
	{
		task->update(deltaTime);
	}
}
