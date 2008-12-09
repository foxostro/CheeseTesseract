#include "Core.h"


#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#	include <shlobj.h>  // SHGetFolderPath
#	include <shlwapi.h> // PathAppend
#	include <direct.h>  // _tchdir and _tgetcwd
#	define PATH_SEPARATOR ( '\\' )
#	ifndef W_OK
#	define W_OK _S_IREAD
#	endif
#	ifndef R_OK
#	define R_OK _S_IWRITE
#	endif
#	define mkdir(path, mode) _mkdir(path)
#	define chdir _chdir
#	define getcwd _getcwd
#else
#	include <unistd.h>
#	include <cstdio>
#endif

void createDirectory(const FileName &path) {
	if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0) {
		if (errno != EEXIST) { // directory exists is OK
			perror("mkdir() failed");
			ERR(string("mkdir() failed: ") + strerror(errno));
		}
	}
}

bool setWorkingDirectory(const FileName &path) {
	return chdir(path.c_str()) >= 0;
}

FileName getWorkingDirectory() {
	char *pszWorkingDirectory = getcwd(0,0);
	
	string workingDirectory = pszWorkingDirectory;
	
	free(pszWorkingDirectory);
	
	return workingDirectory;
}

FileName getUserSettingsDirectory() {
	FileName finalPath("./");
	FileName homeDir("~/");
	
#ifdef _WIN32
	char szHomeDir[MAX_PATH];
	
	SHGetFolderPath(NULL,
	                CSIDL_APPDATA | CSIDL_FLAG_CREATE,
	                NULL,
	                0,
	                szHomeDir);
	                
	homeDir = FileName(szHomeDir);
#endif
	
	finalPath = homeDir.append(FileName(PROJECT_NAME));
	
	// Ensure that the directory exists
	createDirectory(finalPath);
	
	return finalPath;
}

FileName getApplicationDirectory() {
#ifdef _WIN32
	char pathBuffer[_MAX_PATH];
	
	if (GetModuleFileName(GetModuleHandle(NULL), pathBuffer, _MAX_PATH-1) != 0) {
		// Strip off the filename and extension
		size_t i;
		for (i = strlen(pathBuffer) - 1; i > 0; --i) {
			if (pathBuffer[i] == '\\') {
				pathBuffer[i]=0;
				break;
			}
		}
		
		if (i != 0) {
			return pathBuffer;
		}
	}
	
	return FileName(".\\");
#else
	return FileName("./");
#endif
}

bool isFileOnDisk(const FileName &fileName) {
	struct stat info;
	
	// if we can stat the file, then it does exist
	return (stat(fileName.c_str(), &info) == 0);
}
