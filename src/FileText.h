#ifndef _FILE_TEXT_H_
#define _FILE_TEXT_H_

#include "File.h"

class FileText : public File
{
public:
	typedef vector<string> LINES;

	/**
	Gets a single character from the file, then increments the cursor
	@return Single character from the file
	*/
	virtual unsigned char getChar();
	
	/**
	Writes a string to file
	@param s The string to write
	*/
	virtual void write(const string &s);

	/**
	Gets the full text of the file without modifying the file cursor position.
	The getFullText method fails when there are lines that are over 1024 characters long!
	@return full text of the file
	*/
	virtual string getFullText();

	/**
	Reads the full contents of a text file, closing the file upon returning.
	Fails when there are lines that are over 1024 characters long!
	@param fileName The file to read
	@return full text of the file
	*/
	static string readFile(const FileName &fileName);

	/**
	Reads the lines of text from a file, closing the file upon returning.
	Fails when there are lines that are over 1024 characters long!
	@return full text of the file
	*/
	static LINES readLines(const FileName &fileName);

protected:
	/**
	Gets the string to enter the file operation mode
	@param mode File operation mode
	@return mode string
	*/
	virtual const char* getModeString(FILE_MODE mode);
};

#endif
