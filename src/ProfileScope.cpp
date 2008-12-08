#include "stdafx.h"
#include "FrameTimer.h"
#include "Application.h"
#include "ProfileScope.h"

extern shared_ptr<Timer> g_FrameTimer;
extern Application *g_Application;

ProfileScope::ProfileScope(const string &_tag) {
	tag = _tag;
	start = g_FrameTimer->getElapsedTimeMS();
}

ProfileScope::~ProfileScope() {
	double end = g_FrameTimer->getElapsedTimeMS();
	double elapsed = end - start;
	g_Application->record_profile_entry(tag, elapsed);
}
