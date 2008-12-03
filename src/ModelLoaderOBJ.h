#ifndef _MODEL_LOADER_OBJ_H_
#define _MODEL_LOADER_OBJ_H_

#include "ModelLoaderMulti.h"
#include "FileText.h"
#include "vec3.h"
#include "vec2.h"
#include "Face.h"

/** OBJ model loader */
class ModelLoaderOBJ : public ModelLoaderMulti
{
private:
	/**
	Loads a single model from file
	@param fileName File name of the model
	@return vector<Mesh*>
	*/
	vector<Mesh*> loadKeyFrame(const FileName &fileName) const;

    typedef vector<string> TOKENS;

    typedef vector<TOKENS> LINES;

    /**
    @param lines Lines of text from the file
    @param type First token on applicable lines of text
    @param parseLine Function to parse applicable lines of text into data
    @return pair of values: the first is the array of data, second is the
            number of items in the array.  Ownership passes to the caller.
    */
    template<typename T>
    static vector<T> read(const LINES &lines,
                          const string &type,
                          function<T (const TOKENS &)> parseLine)
    {
        const LINES input = filterAllowLines(lines, type);
        vector<T> data(input.size());
        transform(input.begin(), input.end(), data.begin(), parseLine);
        return data;
    }

    /**
    Convert a line of text into a vector of tokens
    @param line Line of text
    @return tokens separated by whitespace
    */
    static TOKENS parseTokens(const string &line);

    /**
    Convert lines of text parsed from a text file into a regular vector of
    strings.
    @param lines Lines parsed from text file
    @return lines of text
    */
    static LINES getLines(const FileText::LINES &lines);

    /**
    Determines whether the line is commented or blank
    @param lines Lines from the file
    @return true if the line is commented or the line is blank
    */
    static bool isLineCommented(const TOKENS &line);

    /**
    Strips out commented lines
    @param lines Lines from the file
    */
    static LINES stripComments(const LINES &lines);

    /**
    Strips out commented lines
    @param line A line from a text file
    @param type The first token of the line
    */
    inline static bool isLineOfType(const TOKENS &line, const string &type)
    {
        return line[0] == type;
    }

    /**
    Passes through lines beginning with the specified token
    @param lines Lines from the file
    @param type First token of the lines to keep
    */
    static LINES filterAllowLines(const LINES &lines, const string &type);

    /**
    Reads a vertex from a line of text
    @param line vertex information
    @return vertex
    */
    static vec3 parseVertex(const TOKENS &line);

    /**
    Reads a vertex from a line of text
    @param line vertex information
    @return vertex
    */
    static vec2 parseTexCoord(const TOKENS &line);

    /**
    Reads a face from a line of text
    @param line face information
    @return face
    */
    static Face parseFace(const TOKENS &line);

    /**
    @param s Token that contains index information (slash delimited)
    @return Tuple of index values:
            1) Vertex index
            2) Tex-Coord index
            3) Normal index

             If a value is -1, then use the order in which the vertices appear
             instead of an index value.
    */
    static tuple<int, int, int> parseIndex(const string &s);
};

#endif
