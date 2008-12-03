#include "Core.h"
#include "FileFuncs.h"

#ifdef _WIN32
#include <io.h>
#else
#include <dirent.h>
#endif

#include "searchfile.h"

vector<FileName> SearchFile(const FileName &_searchDirectory,
                            const string &fileExtension)
{
	vector<FileName> filesFound;

	// Ensure standard file path separators, etc
	FileName searchDirectory = getWorkingDirectory().append(_searchDirectory);

#ifdef _WIN32

    searchDirectory = searchDirectory.append(FileName("*" + fileExtension));

	// Search for all files matching the pattern
	struct _finddata_t file;
	long hFile = (long)_findfirst(searchDirectory.c_str(), &file);

	if(hFile != -1L)
	{
		do
		{
			if(FileName::getExtension(file.name) == fileExtension)
			{
				filesFound.push_back(FileName(file.name));
			}
		}
		while(_findnext( hFile, &file ) == 0);

		// We are done with the handle
		_findclose(hFile);
	}
    else
    {
        switch(errno)
        {
        case EINVAL: ERR("Invalid parameter: filespec or fileinfo was NULL. Or, the operating system returned an unexpected error."); break;
        case ENOENT: ERR("File specification that could not be matched."); break;
        case ENOMEM: ERR("Insufficient memory."); break;
        default: ERR("Unknown error"); break;
        };
    }
#else
	struct dirent *directoryEntry = 0;
	DIR *directory = opendir(searchDirectory.c_str());

	while((directoryEntry = readdir(directory)) != 0)
	{
	    FileName name(directoryEntry->d_name);

		if(name.getExtension() == fileExtension)
		{
			filesFound.push_back(name);
		}
	}

	closedir(directory);
#endif

	return filesFound;
}
