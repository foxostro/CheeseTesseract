#include "Core.h"
#include "File.h"

bool File::openStream(const FileName &fileName, FILE_MODE mode)
{
	this->fileName = fileName;

	const char *pszMode = getModeString(mode);

	stream = fopen(fileName.c_str(), pszMode);

	if(stream==0 || ferror(stream))
	{
		// Failed to open file stream
		return false;
	}

	return true;
}

long File::tell() const
{
	return ftell(stream);
}

void File::closeStream()
{
	fclose(stream);
}

bool File::streamOk() const
{
	return stream && !ferror(stream);
}

bool File::endOfFile() const
{
	return feof(stream)!=0;
}

long File::seek(long offset, FILE_SEEK fileSeekOrigin)
{
	int origin = 0;

	switch(fileSeekOrigin)
	{
	case FILE_SEEK_BEGIN:   origin = SEEK_SET; break;
	case FILE_SEEK_CURRENT: origin = SEEK_CUR; break;
	case FILE_SEEK_END:     origin = SEEK_END; break;
	};

	fseek(stream, offset, origin);

	return ftell(stream);
}

U8* File::readBinaryFile(const FileName &fileName)
{
	FILE *stream = fopen(fileName.c_str(), "rb");

	VERIFY(stream && !ferror(stream),
		"File operation failed: " + fileName.str());

	fseek(stream, 0, SEEK_END);

	const long fileSize = ftell(stream);

	fseek(stream, 0, SEEK_SET);

	U8 *buffer = new U8[fileSize];

	fread(buffer, fileSize, sizeof(U8), stream);

	fclose(stream);

	return buffer;
}
