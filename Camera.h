#pragma once
#include <glm\glm.hpp>
#include "Transforms.hpp"
#include <SDL.h>

using namespace glm;

class Camera
{
public:
	Camera(float fov, float aspect, float zNear, float zFar);
	~Camera();

	mat4 GetViewMatrix();

	void Update();

	void MouseMoveTarget(SDL_Event *e);

	Transform m_transform;

	vec3 Target = vec3(0);
	

	bool m_useTarget =false;

	mat4 m_PerspecProjec;

	inline mat4 GetPerspectiveProjection() const
	{
		return m_PerspecProjec;
	}

	void MoveForwardLocal(float scalar)
	{
		vec3 OldPos = this->m_transform.getPos();
		this->m_transform.setPos(OldPos + (m_forward * scalar));
		Target += (m_forward * scalar);
	}

	void MoveRightLocal(float scalar)
	{
		vec3 OldPos = this->m_transform.getPos();
		this->m_transform.setPos(OldPos + (m_right * scalar));
		Target += (m_right* scalar);
	}


private:
	vec3 m_forward;
	vec3 m_right;
	vec3 m_up;
};