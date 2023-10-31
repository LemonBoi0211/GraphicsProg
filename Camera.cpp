#include "Transforms.hpp"
#include "Camera.h"
#include <glm\glm.hpp>

using namespace glm;

Camera::Camera(float fov,float aspect,float zNear,float zFar)
{
	m_PerspecProjec = perspective(fov, aspect, zNear, zFar);
}

mat4 Camera::GetViewMatrix()
{
	m_forward = normalize(vec3(0) - m_transform.getPos());

	m_right = normalize(cross(vec3(0, 1, 0), m_forward));
	m_up = cross(m_forward, m_right);

	return lookAt(m_transform.getPos(), vec3(0), m_up);
}

Camera::~Camera()
{

}
