#include "LightBase.h"
#include <glew.h>

void LightBase::Draw(Camera& Cam)
{

	glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	float* temp = &Cam.GetPerspectiveProjection()[0][0];
	glLoadMatrixf(( temp ));
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 MV = Cam.GetViewMatrix() * m_transform.GetModel();
	glLoadMatrixf((const GLfloat*)&MV[0]);

	glBegin(GL_LINES);
		glm::vec3 p1 = this->m_transform.getPos();
		glm::vec3 p2 = p1;

		glColor3f(1, 0, 0);
		glVertex3fv(&p1.x);
		p2 = p1 + glm::vec3(1, 0, 0) * 0.1f;
		glColor3f(1, 0, 0);
		glVertex3fv(&p2.x);

		glColor3f(0, 1, 0);
		glVertex3fv(&p1.x);
		p2 = p1 + glm::vec3(0, 1, 0) * 0.1f;
		glColor3f(0, 1, 0);
		glVertex3fv(&p2.x);

		glColor3f(0, 0, 1);
		glVertex3fv(&p1.x);
		p2 = p1 + glm::vec3(0, 0, 1) * 0.1f;
		glColor3f(0, 0, 1);
		glVertex3fv(&p2.x);
	glEnd();

}

LightBase::LightBase()
{
	m_colour = vec3(1, 1, 1);
}

LightBase::~LightBase()
{

}

Transform LightBase::GetTransform()
{
	return m_transform;
}

