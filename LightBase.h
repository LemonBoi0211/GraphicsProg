#pragma once
#ifndef LIGHTBASE_H
#define LIGHTBASE_H

#include "Transforms.hpp"
#include "Camera.h"

class LightBase
{
public:
	LightBase();
	~LightBase();

	Transform& GetTransform();

	void Draw(Camera& Cam);

	vec3 m_colour;

private:
	Transform m_transform;
};

#endif // !LIGHTBASE_H
