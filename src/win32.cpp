#ifdef _WIN32

#include <Windows.h>

int mymain(void); /* defined in Application.cpp */

int main(void) {
	return mymain();
}

#endif
