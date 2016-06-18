#pragma once
#include <chrono>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "HighResClock.h"
#else
typedef std::chrono::high_resolution_clock HighResClock;
#endif

namespace xtr{
	class Timer{
	public:
		Timer();
		virtual ~Timer();
		void startTimer();
		void frameRateRegulator();
		void calculateFrameRate(bool state);
		void capFrameRate(bool state);
		bool isCapped();
		void setMaxFPS(int max);
		double getTime();
		double getFrameTiming();
		int getFPS();
		int getMaxFPS();
		bool FPSupdated(); // returns true if the FPS calculation has been updated since this method was last called.
	private:
		// For calculating and regulating frame rate
		double maxFPS;
		int actualFPS;
		bool cap;
		bool calculateRate;
		bool fps_tick;
		std::chrono::duration<double> tcount; // used to determine if enough time has passed to regulate frame rate
		int fcount; // used to count frames per x time
		std::chrono::time_point<HighResClock> start_time;
		std::chrono::time_point<HighResClock> frame_end_tick;
		std::chrono::duration<double> time_per_frame;
	};
}

