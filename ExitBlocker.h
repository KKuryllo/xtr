#pragma once
#include <cstdlib>
namespace xtr {
class ExitBlocker {
public:
	~ExitBlocker()
	{
		if (!allow_exit) std::abort();
	}
	void allowExit(const bool state = true)
	{
		allow_exit = state;
	}
private:
	bool allow_exit = false;
};
}
