#ifndef _MY_ASSERT_H_
#define _MY_ASSERT_H_

void MyDebugBreak(void);

/**
Tests an assertion
@param line The line of the assertion
@param pszfileName The file name of the file in which the assertion occurred
@param message A message describing the assertion
@return true If the user decided to enter the debugger
*/
bool assertionFailed(int line,
                     const char *pszfileName,
                     const char *pszExpression,
                     const string &message);

/**
Throws an exception with a call stack and a message
@param line The line of the assertion
@param pszfileName The file name of the file in which the assertion occurred
@param message A message describing the assertion
@throws Intentionally generates and throws a runtime_error exception
*/
void throwException(int lineNumber,
                    const char *pszFileName,
                    const string &message);

#ifdef _UNIT_TEST
#	define ASSERT(expr, msg) for(;;) { if(!(expr)) throwException(__LINE__, __FILE__, msg); break; }
#	define VERIFY(expr, msg) for(;;) { if(!(expr)) throwException(__LINE__, __FILE__, msg); break; }
#else
#	ifndef ASSERT
#		ifdef NDEBUG
#			define ASSERT(ignore_expr, ignore_msg) ((void) 0)
#		else
#			define ASSERT(expr, msg) for(;;) { if(!(expr) && assertionFailed((int)(__LINE__), __FILE__, #expr, (msg))) MyDebugBreak(); break; }
#		endif
#	endif
#	ifndef VERIFY
#		define VERIFY(expr, msg) for(;;) { if(!(expr) && assertionFailed((int)(__LINE__), __FILE__, #expr, (msg))) MyDebugBreak(); break; }
#	endif
#endif

#ifndef FAIL
#define FAIL(msg) for(;;) { throwException((int)(__LINE__), __FILE__, (msg)); break; }
#endif

#endif
