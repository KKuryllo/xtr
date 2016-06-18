#pragma once
/*
This code adapted by Kacper Kuryllo for use by Digital Diatom
and sublicenced under the MIT licence
Copyright(c) 2015 Kacper Kuryllo

The orginal code can be found at https://github.com/Cleroth/stlx
*/
/*
Cleroths original licence:

The MIT License(MIT)

Copyright(c) 2015 Cleroth

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include <atomic>

namespace xtr {

class lock_guard_flag {	// class with destructor that unlocks an std::atomic_flag mutex
public:
	explicit lock_guard_flag(std::atomic_flag & m)
		: _mutex(m)
	{	// construct and lock
		while (_mutex.test_and_set())
			;
	}

	lock_guard_flag(std::atomic_flag & m, std::memory_order order)
		: _mutex(m)
	{
		while (_mutex.test_and_set(order))
			;
	}

	~lock_guard_flag() _NOEXCEPT
	{	// unlock
		_mutex.clear();
	}

	lock_guard_flag(const lock_guard_flag&) = delete;
	lock_guard_flag& operator=(const lock_guard_flag&) = delete;

//private:
	std::atomic_flag & _mutex;
};

template <typename T>
struct MutexedObjectInstance {
	MutexedObjectInstance(std::atomic_flag & m, T * t)
		: _guard(m), _t(t)
	{
	}
	~MutexedObjectInstance() {}
  MutexedObjectInstance(const MutexedObjectInstance& o) : _guard(o._guard._mutex), _t(nullptr)
	{
    abort();
		//static_assert(false, "This function only exists to provide copy ellision (RVO). This class is NOT copyable.");
	}
  MutexedObjectInstance& operator=(const MutexedObjectInstance& o) = delete;

	T* operator ->() const { return _t; }
	operator T*() const { return _t; }
	T * Get() const { return _t; }

private:
	lock_guard_flag _guard;
	T * const _t;
};

template <class T>
struct MutexedObject {
	using Type = T;

	MutexedObject()
	{
		_mutex.clear();
	}

	MutexedObjectInstance<T> Lock()
	{
		return MutexedObjectInstance<T>(_mutex, &_t);
	}

	T * Unsafe() { return &_t; }
	const T * Unsafe() const { return &_t; }

private:
	T _t;
	std::atomic_flag _mutex;
};

}; // namespace xtr