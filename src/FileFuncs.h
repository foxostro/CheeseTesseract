#ifndef _FILE_FUNCS_H_
#define _FILE_FUNCS_H_

/**
Creates a directory
@param path Directory to create
*/
void createDirectory(const FileName &path);

/**
Use this command to set the current working directory
@param path The new directory to set as the current working directory
@return true if the current working directory was changed, false otherwise
*/
bool setWorkingDirectory(const FileName &path);

/**
Gets the current working directory
@return The current working directory
*/
FileName getWorkingDirectory();

/**
Gets the home directory of the current user
@return The home directory
*/
FileName getUserSettingsDirectory();

/**
Gets the directory where the application is located
@return Application's directory
*/
FileName getApplicationDirectory();

/**
Determine if a file exists on disk
@param fileName Name of the file to check access permissions on
@return true if the file is on disk, false otherwise
*/
bool isFileOnDisk(const FileName &fileName);

#endif
