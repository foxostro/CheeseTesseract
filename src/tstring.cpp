#include <boost/lexical_cast.hpp>
#include "Core.h"

char* strdup(const string &src) {
	char *dst = new char[src.size()+1];
	
	for (size_t i=0; i<src.size(); ++i) dst[i] = src[i];
	dst[src.size()] = 0; // null-terminate the string
	
	ASSERT(strcmp(dst, src.c_str())==0,
	       "Failed to duplicate string: \"" + src + "\"");
	       
	return dst;
}

int stoi(const string &s) {
	if (s.empty())
		return -1;
		
	return atoi(s.c_str());
}

float stof(const string &s) {
	return (float)atof(s.c_str());
}

string itos(int i) {
#ifdef _WIN32
	char buffer[64] = {0};
	_itoa_s(i, buffer, 10);
	return string(buffer);
#else
	return lexical_cast<string>(i);
#endif
}

string sizet_to_string(size_t i) {
	return itos((int)i);
}

string ftos(float f, int dec) {
	string ret;
	stringstream s;
	
	s.precision(dec);
	s << f;
	s >> ret;
	
	return ret;
}

string dtos(double f, int dec) {
	string ret;
	stringstream s;
	
	s.precision(dec);
	s << f;
	s >> ret;
	
	return ret;
}

string f2tos(float f) {
	return ftos(f, 2);
}

string fitToFieldSize(const string &in,
                      char padWith,
                      size_t fieldSize,
                      JUSTIFY justify) {
	if (in.size() > fieldSize) {
		return in.substr(0, fieldSize);
	} else if (in.size() == fieldSize) {
		return in;
	} else {
		size_t charsRemaining = fieldSize-in.size();
		size_t leftCharsRemaining = (size_t)floor((fieldSize-in.size()) / 2.0);
		size_t rightCharsRemaining = (size_t)ceil((fieldSize-in.size()) / 2.0);
		
		string pad;
		for (size_t i=0; i<charsRemaining; ++i) pad += padWith;
		string padLeftHalf;
		for (size_t i=0; i<leftCharsRemaining; ++i) padLeftHalf += padWith;
		string padRightHalf;
		for (size_t i=0; i<rightCharsRemaining; ++i) padRightHalf += padWith;
		
		switch (justify) {
		case JUSTIFY_CENTER:
			return padLeftHalf + in + padRightHalf;
		case JUSTIFY_RIGHT:
			return pad + in;
		case JUSTIFY_LEFT:
		default:
			return in + pad;
		};
	}
}

string toLowerCase(const string &in) {
	string str(in);
	
	for (string::iterator iter = str.begin(); iter != str.end(); ++iter)
		(*iter) = (char)tolower(*iter);
		
	return str;
}

string replace(const string &source,
               const string &find,
               const string &replace) {
	string output = source;
	
	for (size_t j = 0; (j=source.find(find, j)) != string::npos; ++j) {
		output.replace(j, find.length(), replace);
	}
	
	return output;
}
