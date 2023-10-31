#pragma once
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

using namespace glm;

class Camera
{
public:
	Camera(float fov, float aspect, float zNear, float zFar);
	~Camera();

	mat4 GetViewMatrix();

	Transform m_transform;

	mat4 m_PerspecProjec;

	inline mat4 GetPerspectiveProjection() const
	{
		return m_PerspecProjec;
	}

private:
	vec3 m_forward;
	vec3 m_right;
	vec3 m_up;
};