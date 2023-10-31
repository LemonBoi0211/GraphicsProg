#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

using namespace glm;

class Transform
{
private:
	vec3 m_position;
	vec3 m_rotation;
	vec3 m_scale;

public:
	Transform(const vec3& pos = vec3(),
		const vec3& rot = vec3(),
		const vec3& scale = vec3(1)):
		m_position(pos),
		m_rotation(rot),
		m_scale(scale)
	{}
	~Transform()
	{}

public:
	void setPos(vec3 p) { m_position = p; }
	vec3 getPos() const { return m_position; }

	void setRot(vec3 r) { m_rotation = r; }
	vec3 getRot() const { return m_rotation; }

	void setScale(vec3 s) { m_scale = s; }
	vec3 getScale() const { return m_scale; }


	inline mat4 GetModel() const
	{
		mat4 posMatrix = translate(m_position);
		mat4 rotXMatrix = rotate(m_rotation.x, vec3(1, 0, 0));
		mat4 rotYMatrix = rotate(m_rotation.y, vec3(0, 1, 0));
		mat4 rotZMatrix = rotate(m_rotation.z, vec3(0, 0, 1));
		mat4 scaleMatrix = scale(m_scale);

		mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return posMatrix * rotMatrix * scaleMatrix;
	}

};

#endif