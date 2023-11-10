#include "Transforms.hpp"
#include "Camera.h"
#include <glm\glm.hpp>
#include "Input.h"
#include <iostream>

using namespace std;

using namespace glm;


Camera::Camera(float fov,float aspect,float zNear,float zFar)
{
	m_PerspecProjec = perspective(fov, aspect, zNear, zFar);

	//m_forward = normalize(vec3(0) - m_transform.getPos());
}

mat4 Camera::GetViewMatrix()
{
	m_forward = normalize(Target - m_transform.getPos());

	m_right = normalize(cross(vec3(0, 1, 0), m_forward));
	m_up = cross(m_forward, m_right);

	return lookAt(m_transform.getPos(), vec3(m_transform.getPos() + m_forward), m_up);
}

void Camera::Update()
{
	vec3 camFront;

	camFront.x = m_transform.getPos().x;
	camFront.y = m_transform.getPos().y;
	camFront.z = m_transform.getPos().z + 0.1f;

	m_forward = normalize(camFront);
}

Camera::~Camera()
{

}
