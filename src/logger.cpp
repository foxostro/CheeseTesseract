#include "Core.h"
#include "FileFuncs.h"

#ifdef _WIN32
#include <windows.h>
#endif

void PrintStringToLog(const string &s)
{
	static bool firstTime = true;
	static fstream stream;

	if(firstTime)
	{
		firstTime = false;

		const FileName logFileName("log.txt");

		stream.open(logFileName.c_str(), ios::out);

		if(!stream)
		{
			cerr << "Failed to create log file: " << logFileName.str() << endl;
		}
		else
		{
			cout << "Redirecting std::clog to file: "
			     << logFileName.str()
			     << endl;
			clog.rdbuf(stream.rdbuf()); // redirect clog to file
			
			cout << "Redirecting std::cerr to file: "
			     << logFileName.str()
			     << endl;
			
			cerr.rdbuf(stream.rdbuf()); // redirect cerr to file
		}

		// Create a header for the log file
		clog << "=============================================" << endl
		     << "=                                           =" << endl
#ifndef NDEBUG
		     << "=               Debug Build                 =" << endl
#else
		     << "=              Release Build                =" << endl
#endif
		     << "=                                           =" << endl
		     << "=============================================" << endl
			 << endl;
	}

#ifdef _WIN32
	// Print message to debugger message window
	OutputDebugString(string(s + "\n\n").c_str());
#endif

	clog << s << endl << endl;
}

void Log(const string &message,
         const string &function,
         const FileName &file,
         const int line)
{
	// Format the time stamp.
	time_t curTime = time(NULL);
	char timestamp[32];
	strftime(timestamp,
	         sizeof(timestamp),
	         "%Y.%m.%dT%H:%M:%S",
	         localtime(&curTime));

	PrintStringToLog
	(
	function + "  ->  " + message +
		   "\n\t" + file.stripPath().str() + ":" + itos(line) +
						 "\n\t" + timestamp +
		   "\n"
	);
}
