#include "stdafx.h"
#include "searchfile.h"
#include "FileFuncs.h"
#include "ScreenShot.h"
#include "devil_wrapper.h"

static FileName getScreenShotFileName(const string &prefix)
{
	int highestNumber = 0;

	const string ext = string(".jpg");

	vector<FileName> files = SearchFile(FileName("sshots/"), ext);

	for(vector<FileName>::const_iterator iter=files.begin();
		iter!=files.end();
		++iter)
	{
		const string &fileName = iter->str();
		const string strNum = fileName.substr(prefix.length(),
			fileName.length() - prefix.length() - ext.length());
		const int number = stoi(strNum);

		if(number>highestNumber)
		{
			highestNumber = number;
		}
	}

	string numberPart = fitToFieldSize(itos(++highestNumber), '0', 5, JUSTIFY_RIGHT);

	return FileName(string("sshots/") + prefix + numberPart + ext);
}

void takeScreenShot(const string prefix)
{
	ILuint handle=0;
	ilGenImages(1, &handle);

	ilBindImage(handle);
	ilutGLScreen();

	createDirectory(FileName("sshots/"));

	char *pszScreenShotFileName = strdup(getScreenShotFileName(prefix).str());
	ilSaveImage(pszScreenShotFileName);
	delete[] pszScreenShotFileName;

	ilDeleteImage(handle);
}
