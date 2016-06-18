#include "stdafx.h"
#include <inttypes.h>
#include <thread>
#include "Timer.h"
using namespace xtr;

Timer::Timer() :
    maxFPS(60.0), actualFPS(0), cap(false), calculateRate(true), fps_tick(false), fcount(0), time_per_frame(0.0){}

Timer::~Timer(){}

void Timer::startTimer(){
	frame_end_tick = start_time = HighResClock::now();
}

// Calculates time per frame, frame rate and handles frame rate capping.
void Timer::frameRateRegulator()
{
	// calculate the number of frames per second if they are to be displayed
	if (calculateRate) {
		tcount += time_per_frame;
		fcount++;
		if (tcount.count() > 1.0) {
			actualFPS = fcount;
			tcount = std::chrono::duration<double>(0.0);
			fcount = 0;
			fps_tick = true;
		}
	}
	// Calculate time since the last frame ended and record the current time
	std::chrono::time_point<HighResClock> current_time = HighResClock::now();
	time_per_frame = current_time - frame_end_tick;
	// what to do if frame rate is to be capped
	if (cap) {
		auto max_time_per_frame = 1.0 / double(maxFPS);
		// Calculate time since last frame
		double delay = (max_time_per_frame - time_per_frame.count());
		if (delay > 0 && delay < max_time_per_frame) std::this_thread::sleep_for(std::chrono::milliseconds(int(delay * 1000.0)));
		// after delay we update both the time since the last frame and record the curret time
		std::chrono::time_point<HighResClock> current_time = HighResClock::now();
		time_per_frame = current_time - frame_end_tick;
	}
	frame_end_tick = HighResClock::now();
}

// Toggles the display of frame rate
void Timer::calculateFrameRate(bool arg){
    calculateRate = arg;
}

// Toggles whether frame rate is to be capped
void Timer::capFrameRate(bool arg){
    cap = arg;
}

// Weather frame rate is being capped or not
bool Timer::isCapped(){
    return cap;
}

// Returns the current time in seconds
double Timer::getTime(){
	std::chrono::time_point<HighResClock> time = HighResClock::now();
	std::chrono::duration<double> how_long = time - start_time;
	return how_long.count();
}

// returns the amount of time per frame in seconds
double Timer::getFrameTiming(){
	return time_per_frame.count() / 1000.0;
}

// returns the last calculated FPS
int Timer::getFPS(){
    return actualFPS;
}

// returns the maximum capped FPS
int Timer::getMaxFPS() {
	return (int)maxFPS;
}

// set the FPS cap
void Timer::setMaxFPS(int max){
    maxFPS = max;
}

// returns true if the FPS has been updated since this was last called.
bool Timer::FPSupdated(){
    if (fps_tick){
        fps_tick = false;
        return true;
    }
    else
        return false;
}
