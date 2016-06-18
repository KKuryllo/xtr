// This class contains helper definitions for multithreading

#pragma once

#include <atomic>

//#include <boost/thread/thread.hpp>
#include <thread>
#include <mutex>
#include <shared_mutex>
//#include <boost/bind.hpp>
//#include <boost/thread/locks.hpp>
#include <boost/lockfree/spsc_queue.hpp>
//#include <boost/atomic.hpp>

#include <tbb/tbb.h>
#include <tbb/spin_mutex.h>
#undef HEAP_NO_SERIALIZE
#include <tbb/concurrent_vector.h>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_unordered_set.h>

#include "SpinLock.h"

//typedef mutable std::shared_mutex SharedLock;
typedef std::mutex SingleMutex;
typedef std::unique_lock<SingleMutex> UniqueLock;
//typedef std::unique_lock< SharedLock > WriteLock;
//typedef std::unique_lock< SharedLock > ConcurrentWriteLock;
//typedef std::shared_lock< SharedLock > ReadLock;
typedef std::lock_guard<std::mutex> ScopedLock;
typedef std::atomic_bool AtomicBool;
typedef std::atomic_ushort AtomicUShort;
typedef std::atomic_ullong AtomicULLong;
typedef std::atomic_ulong AtomicULong;
typedef std::atomic_uint AtomicUInt;
typedef std::atomic_int AtomicInt;
typedef std::atomic_long AtomicLong;
typedef std::atomic_llong AtomicLLong;
typedef std::atomic_short AtomicShort;
typedef std::condition_variable ConditionVariable;
typedef std::thread SingleThread;
#define concurrent_vector tbb::concurrent_vector
#define concurrent_set tbb::concurrent_unordered_set
#define concurrent_map tbb::concurrent_unordered_map
#define SPSCQueue boost::lockfree::spsc_queue 
//typedef tbb::spin_mutex SpinLock;
typedef xtr::SpinLock SpinLock;  // An alternative SpinLock that may be faster than TBB.  Use TBB version currently to prevent Intel Inspector from registering spinlock errors



