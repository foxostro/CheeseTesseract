#ifdef _WIN32
#include <windows.h>
#else
#include <SDL/SDL.h>
#endif

#include "Core.h"
#include "FrameTimer.h"

Timer::Timer()
		: TicksPerSecond(0),
		Count(0),
		PrevTicks(0),
		Length(0),
		SectionStart(0),
		SectionEnd(0),
		SectionTiming(false),
		lastSectionTime(0.0) {
#ifdef _WIN32
	QueryPerformanceFrequency((LARGE_INTEGER*)&TicksPerSecond);
#else
	TicksPerSecond = 1000; // SDL_GetTicks has resolution on the order of a MS
	Count = SDL_GetTicks();
#endif
	
	PrevTicks = Count;
}

Timer::ticks_t Timer::getTicks() {
	ticks_t count;
	
#ifdef _WIN32
	QueryPerformanceCounter((LARGE_INTEGER*)&count);
#else
	count = SDL_GetTicks();
#endif
	
	return count;
}

void Timer::update() {
	Count = getTicks();
	Length = Count - PrevTicks;
	PrevTicks = Count; // save for next time
}

double Timer::getLengthSeconds() const {
	return (double)Length / (double)TicksPerSecond;
}

double Timer::getElapsedTimeMS() const {
	ticks_t Now, ElapsedTicks;
	double ElapsedSeconds, ElapsedMilliseconds;
	
	Now = getTicks();
	
	ElapsedTicks = Now - PrevTicks;
	ElapsedSeconds = ElapsedTicks / (double)TicksPerSecond;
	ElapsedMilliseconds = ElapsedSeconds * 1000.0;
	
	return ElapsedMilliseconds;
}

void Timer::beginTiming() {
	ASSERT(!SectionTiming, "Cannot nest timing blocks with a single Timer object.  Use another timer!");
	SectionTiming = true;
	SectionStart = getTicks();
}

double Timer::endTiming() {
	ASSERT(SectionTiming, "Cannot nest timing blocks with a single Timer object.  Use another timer!");
	SectionTiming = false;
	SectionEnd = getTicks();
	lastSectionTime = (double)(SectionEnd - SectionStart) / TicksPerSecond * 1000.0;
	return lastSectionTime;
}
