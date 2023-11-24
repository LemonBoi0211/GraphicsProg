#include "Transforms.hpp"
#include "Camera.h"
#include <glm\glm.hpp>
#include "Input.h"
#include <iostream>
#include <glew.h>

using namespace std;

using namespace glm;


Camera::Camera(float fov,float aspect,float zNear,float zFar)
{
	m_PerspecProjec = perspective(fov, aspect, zNear, zFar);

	//m_forward = normalize(vec3(0) - m_transform.getPos());
}

mat4 Camera::GetViewMatrix()
{
	if(m_useTarget )
		m_forward = normalize(Target - m_transform.getPos());

	m_right = normalize(cross(vec3(0, 1, 0), m_forward));
	m_up = cross(m_forward, m_right);

	return lookAt(m_transform.getPos(), vec3(m_transform.getPos() + m_forward), m_up);
}

void Camera::Update()
{

	if ( m_useTarget )
	{
		vec3 camFront;

		camFront.x = m_transform.getPos( ).x;
		camFront.y = m_transform.getPos( ).y;
		camFront.z = m_transform.getPos( ).z + 0.1f;

		m_forward = normalize(camFront);
	}
}

bool firstMove = true;
GLint lastX;
GLint lastY;

//1200, 1000

void Camera::MouseMoveTarget(SDL_Event *e)
{
	if ( !m_useTarget )
	{
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		//cout << mouseX << " " << mouseY<<endl;

		if ( firstMove )
		{
			lastX = mouseX;
			lastY = mouseY;
			firstMove = false;
		}

		GLfloat xOffset = mouseX - lastX;
		GLfloat yOffset = lastY - mouseY;
		lastX = mouseX;
		lastY = mouseY;

		//cout <<"ofset X Y" << xOffset << " " << yOffset << endl;

		GLfloat senstivity = 0.5;

		xOffset *= senstivity;
		yOffset *= senstivity;

		m_transform.getRot( ).y += xOffset; //yaw
		m_transform.getRot( ).x += yOffset; //pitch

		//cout << "tranRot" << xOffset << " " << yOffset << endl;

		if ( m_transform.getRot( ).x > 89.0f )
			m_transform.getRot( ).x == 89.0f;
		if ( m_transform.getRot( ).x < -89.0f )
			m_transform.getRot( ).x == -89.0f;

		vec3 front;
		front.x = cos(radians(m_transform.getRot( ).y)) * cos(radians(m_transform.getRot( ).x));
		front.y = sin(radians(m_transform.getRot( ).x));
		front.z = sin(radians(m_transform.getRot( ).y)) * cos(radians(m_transform.getRot( ).x));

		//cout << front.x << " " << front.y << " " << front.z << endl;

		m_forward = normalize(front);
	}
}

Camera::~Camera()
{

}
