#include "stdafx.h"
#include "Mesh.h"
#include "ModelLoaderOBJ.h"

vector<Mesh*> ModelLoaderOBJ::loadKeyFrame(const FileName &fileName) const
{
	FileText::LINES rawLines = FileText::readLines(fileName);

	const LINES lines = getLines(rawLines);

    const LINES nc = stripComments(lines);

    const vector<vec3> verticesArray  = read<vec3>(nc,"v", parseVertex);
    const vector<vec3> normalsArray   = read<vec3>(nc,"vn",parseVertex);
    const vector<vec2> texcoordsArray = read<vec2>(nc,"vt",parseTexCoord);
    const vector<Face> facesArray     = read<Face>(nc,"f", parseFace);

	return createKeyFrame(new Mesh(verticesArray,
                                   normalsArray,
                                   texcoordsArray,
                                   facesArray));
}

vec3 ModelLoaderOBJ::parseVertex(const TOKENS &tokens)
{
    ASSERT(isLineOfType(tokens, "v") || isLineOfType(tokens, "vn"),
           "Line does not contain a vertex or a normal");

	TOKENS::const_iterator i = tokens.begin();

    vec3 vec;

	vec.x = stof(*(++i));
	vec.y = stof(*(++i));
	vec.z = stof(*(++i));

    return vec;
}

vec2 ModelLoaderOBJ::parseTexCoord(const TOKENS &tokens)
{
    ASSERT(isLineOfType(tokens, "vt"), "Line does not contain a texcoord");

	TOKENS::const_iterator i = tokens.begin();

    vec2 vec;

	vec.x = stof(*(++i));
	vec.y = stof(*(++i));

    return vec;
}

Face ModelLoaderOBJ::parseFace(const TOKENS &tokens)
{
    ASSERT(isLineOfType(tokens, "f"), "Line does not contain a face");

    Face result;

	TOKENS::const_iterator i = tokens.begin();

    for(int j=0; j<3; ++j)
    {
        tuple<int, int, int> index = parseIndex(*(++i));

		result.vertIndex[j] = index.get<0>();
		result.coordIndex[j] = index.get<1>();
		result.normalIndex[j] = index.get<2>();

        ASSERT(result.vertIndex[j] >= 0, "Vertex index is negative!");
        ASSERT(result.coordIndex[j] >= 0, "Tex-Coord index is negative!");
        ASSERT(result.normalIndex[j] >= 0, "Normal index is negative!");
    }

    return result;
}

tuple<int, int, int> ModelLoaderOBJ::parseIndex(const string &s)
{
    int v=-1, vt=-1, vn=-1;

    tokenizer< char_separator<char> > indices(s, char_separator<char>("/"));

    tokenizer< char_separator<char> >::const_iterator i = indices.begin();

    v  = stoi(*(i++)) - 1;
    vt = stoi(*(i++)) - 1;
    vn = stoi(*(i++)) - 1;

    return make_tuple(v, vt, vn);
}

ModelLoaderOBJ::TOKENS ModelLoaderOBJ::parseTokens(const string &line)
{
    TOKENS result;

    tokenizer< char_separator<char> >
    tokens(line, char_separator<char>(" \t\n"));

    for(tokenizer< char_separator<char> >::const_iterator i = tokens.begin();
        i != tokens.end();
        ++i)
    {
        result.push_back(*i);
    }

    return result;
}

ModelLoaderOBJ::LINES ModelLoaderOBJ::getLines(const FileText::LINES &lines)
{
    LINES result;

    for(FileText::LINES::const_iterator i = lines.begin();
        i != lines.end();
        ++i)
    {
        result.push_back(parseTokens(*i));
    }

    return result;
}

bool ModelLoaderOBJ::isLineCommented(const TOKENS &tokens)
{
    return tokens.empty() || (!tokens.empty() && tokens[0].at(0) == '#');
}

ModelLoaderOBJ::LINES ModelLoaderOBJ::stripComments(const LINES &lines)
{
    LINES result;

    for(LINES::const_iterator i = lines.begin();
        i != lines.end();
        ++i)
    {
        if(!isLineCommented(*i))
        {
            result.push_back(*i);
        }
    }

    return result;
}

ModelLoaderOBJ::LINES ModelLoaderOBJ::filterAllowLines(const LINES &lines,
                                                       const string &type)
{
    LINES result;

    for(LINES::const_iterator i = lines.begin();
        i != lines.end();
        ++i)
    {
        if(isLineOfType(*i, type))
        {
            result.push_back(*i);
        }
    }

    return result;
}
