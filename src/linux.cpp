#ifndef _WIN32

#if 1
int mymain(void); /* defined in Application.cpp */
int main(void) {
	return mymain();
}
#else
#include "stdafx.h"
#include "StackWalker.h"
#include <iostream>

int mymain(); // from Application.cpp

#include <signal.h>
#include <execinfo.h>

extern "C" void printBackTrace(FILE *fp)
{
	const string callstack = getCallStack(10);

	fprintf(fp, "################### Back trace ###################\n");
	fprintf(fp, "%s\n", callstack.c_str());
	fprintf(fp, "##################################################\n");
}

extern "C" void handler(int, siginfo_t*, void*)
{
	printBackTrace(stdout);
	while(true);
}

void setupSignalHandler()
{
	struct sigaction SignalAction;

    TRACE("Setting up signal handlers");

	SignalAction.sa_sigaction = handler;
	sigemptyset(&SignalAction.sa_mask);
	SignalAction.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &SignalAction, 0);
	sigaction(SIGABRT, &SignalAction, 0);
}

int main()
{
	//setupSignalHandler();
	return mymain();
}
#endif

#endif
