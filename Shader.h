#pragma 
#include <glew.h>
#include <string>
#include <fstream>
#include "Transforms.hpp"
#include "Camera.h"
#include "LightBase.h"

using namespace std;

enum ShaderTypes
{
	VERTEX,
	FRAGMENT,
	NUM_SHADER_TYPES,
};

enum UniformNames
{
	MODEL_U,
	PROJECTION_U,
	VIEW_U,
	FRAG_LIGHTCOLOR_U,
	FRAG_LIGHTPOS_U,
	FRAG_CAMERAPOS_U,
	NUM_UNIFORMS,
};

class Shader
{
public:
	Shader(const string FileLocation, Camera &camera);
	~Shader();

	void Update(Transform &transform, LightBase& light, mat4& LightSpaceMatrix);
	void UpdateForShadows(Transform& transform, mat4& LightSpaceMatrix);
	void Bind();
	GLuint GetProgram();

private:
	string Name;
	GLuint m_program;
	Camera* m_camera;

	GLuint m_Shaders[NUM_SHADER_TYPES];
	GLuint m_Uniforms[NUM_UNIFORMS];
	
};