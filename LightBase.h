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

	void Draw(Camera* Cam);

private:
	Transform m_transform;
};

#endif // !LIGHTBASE_H
