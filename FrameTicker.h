#pragma once
namespace xtr {
template<unsigned int length>
class FrameTicker {
public:
	bool operator()()
	{
		if (++current_tick >= length) {
			current_tick = 0;
			return true;
		}
		else return false;
	}
private:
	unsigned int current_tick = 0;
};
}
