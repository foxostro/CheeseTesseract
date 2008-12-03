#ifndef _FILE_NAME_H_
#define _FILE_NAME_H_

/** Describes a file path and file name */
class FileName
{
private:
	string fileName;

public:
	FileName()
	{}

	FileName(const FileName &o)
	{
		this->fileName = o.fileName;
	}

	FileName(const string &fileName)
	{
		this->fileName = fix(fileName);
	}

	static FileName fromString(const string &fileName)
	{
		return FileName(fileName);
	}

	static string toString(const FileName &fileName)
	{
		return fileName.str();
	}

	inline const char* c_str() const
	{
		return fileName.c_str();
	}

	inline const string& str() const
	{
		return fileName;
	}

	bool operator==(const FileName &r) const
	{
		return(str() == r.str());
	}

	bool operator==(const string &r) const
	{
		return(str() == r);
	}

	bool operator!=(const FileName &r) const
	{
		return(str() != r.str());
	}

	bool operator!=(const string &r) const
	{
		return(str() != r);
	}

    bool operator<(const FileName& x) const
	{
        return str() < x.str();
	}

public:
	/**
	Appends a file name to a path
	@param path directory name
	@param fileName file name
	@return The composite path
	*/
	FileName append(const FileName &fileName) const;

	/**
	Transforms the given filename to use UNIX style path separators
	@return transformed filename
	*/
	FileName fix() const;

	/**
	Gets the file extension from a file path
	@return Returns the file extension
	*/
	string getExtension() const;

	/**
	Get the directory the file was located in
	@return file path
	*/
	FileName getPath() const;

	/**
	Gets the file name with the path stripped off
	@return file name
	*/
	FileName stripPath() const;

public:
	/**
	Appends a file name to a path
	@param path directory name
	@param fileName file name
	@return The composite path
	*/
	static string append(const string &path, const string &fileName);

	/**
	Transforms the given filename to use UNIX style path separators
	@param fileName File name to transform
	@return transformed filename
	*/
	static string fix(const string &fileName);

	/**
	Finds the extension marker in a string.
	findExtensionDelimeter determines the index of the extension delimiter
	character in a given fileName, when possible. For example, calling with
	the fileName = "image.jpeg" will return 5 and "data/text.txt" will
	return 9.  Also be aware that "data/text.txt.bak" will return 13, as
	this is the last string that is clearly identifiable as a file
	extension.
	@param fileName Name of the file
	@return Index of the extension delimiter character in the fileName.
	If there is no extension, then the length of the filename is returned.
	*/
	static size_t findExtensionDelimeter(const string &fileName);

	/**
	Removes a file extension from the file name
	@param fileName The name of the file to extract the extension from
	@return Removes a file extension from a file path and returns the modified file name.
	*/
	static string stripExtension(const string &fileName);

	/**
	Gets the file extension from a file path
	@param fileName The name of the file to extract the extension from
	@return Returns the file extension
	*/
	static string getExtension(const string &fileName);

	/**
	Get the directory the file was located in
	@param fileName File name
	@return file path
	*/
	static string getPath(const string &fileName);

	/**
	Gets the full file path and name
	@param fileName File name
	@return file name and path
	*/
	static string stripPath(const string &fileName);
};

#endif
