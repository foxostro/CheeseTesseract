#ifndef _SEARCHFILE_INCLUDE
#define _SEARCHFILE_INCLUDE

/**
Searches for files in the current directory matching a pattern.
@param searchDirectory Directory to search within
@param fileExtension File extension to search for
@return List of files found
*/
vector<FileName> SearchFile(const FileName &searchDirectory,
                            const string &fileExtension);

#endif
