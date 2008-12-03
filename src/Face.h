#ifndef _FACE_H_
#define _FACE_H_

/** Face structure describing a triangle */
struct Face
{
	/** indicies for the vertices of this triangle */
	int vertIndex[3];

	/** indicies for the tex coords of this triangle */
	int coordIndex[3];

	/** indicies for the vertex normals of this triangle */
	int normalIndex[3];

    /** Default Constructor */
    Face()
    {
        for(int i=0; i<3; ++i)
        {
            vertIndex[i] = 0;
            coordIndex[i] = 0;
            normalIndex[i] = 0;
        }
    }
};

#endif
