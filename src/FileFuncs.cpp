#include "Core.h"

#include <sys/types.h>
#include <sys/stat.h>

#include <shlobj.h>  // SHGetFolderPath
#include <shlwapi.h> // PathAppend
#include <direct.h>  // _tchdir and _tgetcwd

#define PATH_SEPARATOR ( '\\' )

#ifndef W_OK
#define W_OK _S_IREAD
#endif

#ifndef R_OK
#define R_OK _S_IWRITE
#endif

void createDirectory(const FileName &path)
{
    int result = _mkdir(path.c_str());

	if(result < 0)
	{
		if(errno != EEXIST) // directory exists is OK
		{
			perror("mkdir() failed");
			ERR(string("mkdir() failed: ") + strerror(errno));
		}
	}
}

bool setWorkingDirectory(const FileName &path)
{
    return _chdir(path.c_str()) >= 0;
}

FileName getWorkingDirectory()
{
	char *pszWorkingDirectory = _getcwd(0,0);

	string workingDirectory = pszWorkingDirectory;

	free(pszWorkingDirectory);

	return workingDirectory;
}

FileName getUserSettingsDirectory()
{
	FileName finalPath("./");

	char homeDir[MAX_PATH] = {0};


	if (SUCCEEDED(SHGetFolderPath(NULL,
	                              CSIDL_APPDATA | CSIDL_FLAG_CREATE,
	                              NULL,
	                              0,
	                              homeDir)))
	{
		finalPath = FileName::append(homeDir, PROJECT_NAME);
	}

	// Ensure that the directory exists
	createDirectory(finalPath);

	return finalPath;
}

FileName getApplicationDirectory()
{
	char pathBuffer[_MAX_PATH];

	if(GetModuleFileName(GetModuleHandle(NULL), pathBuffer, _MAX_PATH-1) != 0)
	{
		// Strip off the filename and extension
		size_t i;
		for(i = strlen(pathBuffer) - 1; i > 0; --i)
		{
			if(pathBuffer[i] == '\\')
			{
				pathBuffer[i]=0;
				break;
			}
		}

		if(i != 0)
		{
			return pathBuffer;
		}
	}

	return ".\\";
}

bool isFileOnDisk(const FileName &fileName)
{
	struct stat info;

	// if we can stat the file, then it does exist
	return (stat(fileName.c_str(), &info) == 0);
}
