#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

struct Vertex
{
	vec3 Position;
	vec2 TextureCoord;

	Vertex(const float X, const  float Y, const  float Z)
	{
		this->Position.x = X;
		this->Position.y = Y;
		this->Position.z = Z;

		TextureCoord = { 0,0 };
	}

	Vertex(const vec3 position) : Vertex(position.x, position.y, position.z)
	{

	}

	Vertex(const vec3 position, const vec2 texCoord)
	{
		this->Position = position;
		this->TextureCoord = texCoord;
	}
};

#endif // !VERTEX_H