#ifndef _TSTRING_H_
#define _TSTRING_H_

/**
Copies the given string into a new C-string allocated on the heap.d
@param s source string
@return C-string (Null-terminated) allocated in the heap. Free it with delete[]
*/
char* strdup(const string &s);

/** Represents some value as a string */
string itos(int i);

/** Represents some value as a string */
string sizet_to_string(size_t i);

/** Represents some value as a string */
string dtos(double f, int dec=4);

/** Represents some value as a string */
string ftos(float f, int dec=4);

/** Represents some value as a string */
string f2tos(float f);

/** Converts a value from a string to a integer */
int stoi(const string &s);

/** Converts a value from a string to a float */
float stof(const string &s);

/** Specifies how a field should be justified in the fitToFieldSize method */
enum JUSTIFY {
	JUSTIFY_LEFT,
	JUSTIFY_RIGHT,
	JUSTIFY_CENTER
};

/**
Pads a string and justifies it if it is less than the field size. If it is
larger than the field size, then the string is cropped at the field length
*/
string fitToFieldSize(const string &in,
                      char padWith,
                      size_t fieldSize,
                      JUSTIFY justify);

/**
Makes a string all lowercase
@param in The input string
@return The lower case string
*/
string toLowerCase(const string &in);

/**
Replaces all occurrences of a substring with a replacement string.
@param source Source string
@param find substring to search for
@param replace Replacement string.  Substituted over the 'find' string
@return result string
*/
string replace(const string &source,
               const string &find,
               const string &replace);

#endif
