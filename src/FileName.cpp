#include "Core.h"
#include "FileName.h"

const char PATH_SEPARATOR = '/';

FileName FileName::append(const FileName &fileName) const
{
	return append(str(), fileName.str());
}

FileName FileName::fix() const
{
	return fix(str());
}

string FileName::getExtension() const
{
	return getExtension(str());
}

FileName FileName::getPath() const
{
	return getPath(str());
}

FileName FileName::stripPath() const
{
	return stripPath(str());
}

string FileName::append(const string &path, const string &fileName)
{
	const char lastChar = path.at(path.length()-1);

	if(lastChar != '/' && lastChar != '\\')
	{
		return fix(path + PATH_SEPARATOR + fileName);
	}
	else
	{
		return fix(path + fileName);
	}
}

string FileName::fix(const string &fileName)
{
	return replace(fileName, "\\", "/");
}

size_t FileName::findExtensionDelimeter(const string &fileName)
{
	for(size_t i=0; i<fileName.length(); ++i)
	{
		char c = fileName[fileName.length() - i - 1];

		if(c == '.')
		{
			// Found the delimiter, return its index
			return fileName.length() - i - 1;
		}

		if(c == '\\' || c == '/')
		{
			// Found that there was no delimiter, return the index of the end of the string
			return fileName.length();
		}
	}

	// Found that there was no delimiter, return the index of the end of the string
	return fileName.length();
}

string FileName::stripExtension(const string &fileName)
{
	return fileName.substr(0, findExtensionDelimeter(fileName));
}

string FileName::getExtension(const string &fileName)
{
	return fileName.substr(findExtensionDelimeter(fileName), fileName.length());
}

string FileName::getPath(const string &fileName)
{
	size_t i;
	string in = fix(fileName);

	for(i=in.size()-1; i>0 && in.at(i)!=PATH_SEPARATOR; --i);

	return in.substr(0, i+1);
}

string FileName::stripPath(const string &fileName)
{
	string in = fix(fileName);
	string out;

	for(size_t i=in.size()-1; i>0 && in.at(i)!=PATH_SEPARATOR; --i)
	{
		out = in.at(i) + out;
	}

	return out;
}
