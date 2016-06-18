#pragma once
#include "ctpl.h"
namespace xtr{
struct TaskList {
	~TaskList()
	{
		clear();
	}
	void add(ctpl::Task* task)
	{
		task->recycle = true;
		tasks.push_back(task);
	}
	void clear()
	{
		for (auto&& task : tasks) delete task;
		tasks.clear();
	}
	std::vector<ctpl::Task*> tasks;
};
}
