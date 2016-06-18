#pragma once

/*  High res clock for use in VS2013 to fix bug with low time percision.

This is borrowed code from user "Dave" on stack overflow with permission.
Link to post: http://stackoverflow.com/questions/16299029/resolution-of-stdchronohigh-resolution-clock-doesnt-correspond-to-measureme
*/
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <chrono>

namespace xtr {
struct HighResClock
{
	typedef long long                               rep;
	typedef std::nano                               period;
	typedef std::chrono::duration<rep, period>      duration;
	typedef std::chrono::time_point<HighResClock>   time_point;
	static const bool is_steady = true;
	static time_point now();
};
}

#endif
