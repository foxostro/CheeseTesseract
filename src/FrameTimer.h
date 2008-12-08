#ifndef _FRAME_TIMER_H_
#define _FRAME_TIMER_H_

/** Tracks time between frames and frame FPS stats */
class Timer {
private:
#ifdef _WIN32
	typedef __int64 ticks_t;
#else
	typedef Uint32 ticks_t;
#endif
	
	ticks_t TicksPerSecond;
	ticks_t Count;
	ticks_t PrevTicks;
	ticks_t Length;
	ticks_t SectionStart;
	ticks_t SectionEnd;
	
	bool SectionTiming;
	double lastSectionTime;
	
	
public:
	Timer();
	
	/** Update frame counter */
	void update();
	
	/** Begins timing a section of code */
	void beginTiming();
	
	/** Ends timing a section of code, and returns the elapsed time in milliseconds */
	double endTiming();
	
	/** Returns the elapsed time in milliseconds for the last set of beginTiming and endTiming functions */
	inline double getLastSectionTimeMS() const {
		return lastSectionTime;
	}
	
	/** get the time of the last frame, measured in seconds */
	double getLengthSeconds() const;
	
	/** Get time elapsed since the last call to update */
	double getElapsedTimeMS() const;
	
private:
	static ticks_t getTicks();
};

#endif
