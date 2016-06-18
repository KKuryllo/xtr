#pragma once
#include <atomic>

namespace xtr {
	class SpinLock {
		std::atomic_flag locked;
	public:
		SpinLock() {
			locked.clear();
		}
		void lock() {
			while (locked.test_and_set(std::memory_order_acquire)) { ; }
		}
		void unlock() {
			locked.clear(std::memory_order_release);
		}
		bool try_lock() {
			return !locked.test_and_set(std::memory_order_acquire);
		}
	};
}