#ifndef _FILE_H_
#define _FILE_H_

#include "FileName.h"

class File
{
protected:
	FileName fileName;

	FILE *stream;

public:
	
	/** Seek origin marker */
	enum FILE_SEEK
	{
		FILE_SEEK_BEGIN,
		FILE_SEEK_CURRENT,
		FILE_SEEK_END
	};

	/** File operation mode */
	enum FILE_MODE
	{
		FILE_MODE_READ,
		FILE_MODE_WRITE
	};

	/** Constructor */
	File()
	{}

	/**
	Opens a file and reads its contents into the buffer
	@param fileName The file to open
	@param mode File operation mode
	@return true if the operation succeeded
	*/
	File(const FileName &fileName, FILE_MODE mode)
	{
		openStream(fileName, mode);
	}

	/** Destructor */
	virtual ~File()
	{
		closeStream();
	}
	
	const FileName &getFileName() const
	{
		return fileName;
	}
	
	/** Closes the file stream */
	void closeStream();

	/**
	Opens a file and reads its contents into the buffer
	@param fileName The file to open
	@param mode File operation mode
	@return true if the operation succeeded
	*/
	bool openStream(const FileName &fileName, FILE_MODE mode);

	/**
	Gets the current read/write cursor of the file
	@return offset from the beginning of the stream
	*/
	long tell() const;

	/**
	Determines the current file stream is good and error free
	@return true if the file stream is good
	*/
	bool streamOk() const;

	/**
	Determines whether the cursor has exceeded or met the end of the file.
	@return If EOF then true, otherwise false
	*/
	bool endOfFile() const;

	/**
	Sets the cursor position of the file
	@param offset The offset from the origin to seek towards
	@param origin The origin of the offset from some known point in the file
	@return The new cursor position
	*/
	long seek(long offset, FILE_SEEK origin);

	/** Seeks to the beginning of the file */
	void rewind()
	{
		seek(0, FILE_SEEK_BEGIN);
	}

	/**
	Reads the binary file and returns a buffer containing file contents.
	Ownership of the newly allocated buffer passes to the caller.
	@param fileName Name of the binary file
	@return File contents in a buffer; ownership passes to the caller.
	*/
	static U8* readBinaryFile(const FileName &fileName);

protected:
	/**
	Gets the string to enter the file operation mode
	@param mode File operation mode
	@return mode string
	*/
	virtual const char* getModeString(FILE_MODE mode) = 0;
};

#endif
