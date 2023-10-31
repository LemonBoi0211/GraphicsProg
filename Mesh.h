#pragma once
#ifndef MESH_H
#define MESH_H

#include <glm\glm.hpp>
#include <glew.h>
#include "Transforms.hpp"


class Vertex;

enum
{
	POSITION_VB,
	TEXCOORD_VB,
	INDEX_VB,
	NUM_BUFFERS,
};

class Mesh
{
public:
	Mesh(Vertex* verts, unsigned int vertCount, unsigned int *indices, unsigned int numIndices);
	void Draw();
	~Mesh();

	Transform m_transform;

private:
	GLuint m_vertexBufferObjects[NUM_BUFFERS];
	GLuint m_vertexArrayObject = 0;
	uint m_drawCount;
};


#endif