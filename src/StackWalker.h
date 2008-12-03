#ifndef _STACK_WALKER_H_
#define _STACK_WALKER_H_

/**
Gets the call stack at the point when the function is called
@param maxlines Maximum number of lines before cutting callstack off
@return Call stack
*/
string getCallStack(int maxlines);

#endif
