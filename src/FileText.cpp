#include "Core.h"
#include "FileText.h"

const char* FileText::getModeString(FILE_MODE mode)
{
	switch(mode)
	{
	case FILE_MODE_READ:  return "rt"; break;
	case FILE_MODE_WRITE: return "wt"; break;
	};

	return 0; // not reachable
}

unsigned char FileText::getChar()
{
	return (unsigned char)fgetc(stream);
}

void FileText::write(const string &s)
{
	fprintf(stream, "%s", s.c_str());
	fflush(stream);
}

string FileText::getFullText()
{
	long pos = tell();

	seek(0, FILE_SEEK_BEGIN);

	string contents;

	const int maxLineLength = 1024;

	char line[maxLineLength];

	while(fgets(line, maxLineLength, stream) != 0)
	{
		contents += line;
	}

	seek(pos, FILE_SEEK_BEGIN); // restore file position

	return contents;
}

string FileText::readFile(const FileName &fileName)
{
	FileText file;

	file.openStream(fileName, FILE_MODE_READ);

	return file.getFullText();
}

FileText::LINES FileText::readLines(const FileName &fileName)
{
    const string fileText = readFile(fileName);

    const char_separator<char> delim("\n");
	const tokenizer< char_separator<char> > lines(fileText, delim);

    LINES result;

    for_each(lines.begin(), lines.end(), bind(&LINES::push_back, &result, _1));

    return result;
}
