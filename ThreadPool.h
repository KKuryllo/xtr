#pragma once
#include "ctpl.h"
#include "TaskList.h"

namespace xtr {
class ThreadPool {
public:
	static ThreadPool& getInstance();
	void push(ctpl::Task* task)
	{
		pool.push(task);
	}

	void push(const TaskList& list)
	{
		pool.push(list.tasks);
	}

	void push_wait(const TaskList& list)
	{
		pool.push_wait(list.tasks);
	}
	int size()
	{
		return pool.size();
	}
private:
	ThreadPool();
	~ThreadPool();
	ThreadPool(ThreadPool const&); // Don't Implement
	void operator=(ThreadPool const&); // Don't implemen
	ctpl::thread_pool pool;
};
}
