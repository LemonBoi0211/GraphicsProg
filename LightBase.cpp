#include "LightBase.h"
#include <glew.h>


LightBase::LightBase()
{
	/*glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const GLfloat*)&Cam->GetPerspectiveProjection());
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 MV = Cam->GetViewMatrix() * m_transform.GetModel();
	glLoadMatrixf((const GLfloat*)&MV[0]);*/
}

LightBase::~LightBase()
{

}
