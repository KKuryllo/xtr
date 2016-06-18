#include "stdafx.h"
#include "ThreadPool.h"
using namespace xtr;

ThreadPool::ThreadPool() : pool(std::thread::hardware_concurrency())
{

}

ThreadPool::~ThreadPool()
{
	pool.stop();
}

// Initialize and return the instance.  Threadsafe on C++11
ThreadPool& ThreadPool::getInstance()
{
	static ThreadPool instance; // Guaranteed to be destroyed.
							   // Instantiated on first use.
	return instance;
}