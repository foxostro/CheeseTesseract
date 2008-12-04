#include "Core.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifndef _WIN32
#define IDIGNORE (1)
#define IDRETRY  (2)
#define IDABORT  (3)
#endif

#include "StackWalker.h"
#include "myassert.h"

void MyDebugBreak(void) {
#ifndef _WIN32
	asm ("int $3") ;
#else
	DebugBreak();
#endif
}

string generateMessage(int lineNumber,
					   const char *pszFileName,
					   const string &message)
{
	const string fullMessage = message +
                               "\n" +
                               "\nFile: " + pszFileName +
                               "\nLine: " + itos(lineNumber)
#ifndef _UNIT_TEST
							   + "\n\nCall stack:"
                               "\n-------------------"
                               "\n" + getCallStack(10)
#endif
							   ;

	return fullMessage;
}

void throwException(int lineNumber,
					const char *pszFileName,
					const string &message)
{
	string s = generateMessage(lineNumber, pszFileName, message);
	throw runtime_error(s.c_str());
}

bool assertionFailed(int lineNumber,
                     const char *pszFileName,
                     const char *pszExpression,
                     const string &message)
{
	bool result = false;
	int response = 0;

	const string fullMessage = generateMessage(lineNumber,
	                                           pszFileName,
											   message +
											   "\n\nExpr: " + pszExpression);

#ifndef _WIN32
	std::cout << fullMessage << endl
	          << endl
	          << "Choose a response:" << endl
	          << "\t1) Ignore - Continues execution, risking a crash" << endl
	          << "\t2) Retry  - Trips the debugger, if attached" << endl
	          << "\t3) Abort  - Aborts execution immediately" << endl
	          << ">";
	std::cout.flush();
	std::cin >> response;
	std::cout << endl;
#else
	{
		char *pszFullMessage = strdup(fullMessage);

		response = MessageBox(NULL,
                              pszFullMessage,
                              "Assert",
                              MB_ABORTRETRYIGNORE |
                              MB_ICONERROR |
                              MB_SETFOREGROUND |
                              MB_TOPMOST);

		delete [] pszFullMessage;
		pszFullMessage=0;
	}
#endif

	switch(response)
	{
	case IDIGNORE:
		result = false; // allow execution to continue
		Log(fullMessage, "Assertion, Ignoring", FileName(pszFileName), lineNumber);
		break;

	case IDRETRY:
		result = true; // signals that we should call DebugBreak
		Log(fullMessage, "Assertion, Breaking", FileName(pszFileName), lineNumber);
		break;

	case IDABORT:
		Log(fullMessage, "Assertion, Aborting", FileName(pszFileName), lineNumber);
		abort();
		break;

	default:
		Log(fullMessage, "Assertion, Unexpected input. Aborting!", FileName(pszFileName), lineNumber);
		abort();
		break;
	};

	return result;
}
