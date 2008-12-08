#include "Core.h"

void buildBillboardVertices(const mat4 & matrix,
                            const vec3 &position,
                            float size,
                            vec3 &A,
                            vec3 &B,
                            vec3 &C,
                            vec3 &D) {
	float x0 = matrix[0] - matrix[1];
	float x1 = matrix[0] + matrix[1];
	float y0 = matrix[4] - matrix[5];
	float y1 = matrix[4] + matrix[5];
	float z0 = matrix[8] - matrix[9];
	float z1 = matrix[8] + matrix[9];
	
	A = position + vec3(-x1, -y1, -z1) * size;
	B = position + vec3( x0,  y0,  z0) * size;
	C = position + vec3( x1,  y1,  z1) * size;
	D = position + vec3(-x0, -y0, -z0) * size;
}
