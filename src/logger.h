#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "FileName.h"

/**
Prints a string directly to the log stream.
Also prints it to stdout and (if possible) the debugger output window.
@param s String to log
*/
void PrintStringToLog(const string &s);

/**
Prints a message to the log stream with a string to identify the message origin.
@param message Message string
@param function Message origin.  Usually you pass __FUNCTION__ as the value
@param file File of message origin.  Usually you pass __FILE__ as the value
@param line Line of message origin.  Usually you pass __LINE__ as the value
*/
void Log(const string &message,
                  const string &function,
                  const FileName &file,
                  const int line);

#define TRACE(msg) Log(msg, __FUNCTION__, FileName(__FILE__), __LINE__);
#define ERR(msg)   Log(msg, __FUNCTION__, FileName(__FILE__), __LINE__);

#endif
