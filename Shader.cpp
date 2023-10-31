#include "Shader.h"
#include <iostream>

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		cerr << errorMessage << ": '" << error << "'" << endl;
	}
}

static GLuint CreateShader(const string& ShaderSource, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
	{
		cerr << "Error: Shader Creation Failed!" << endl;
	}

	const char* tempSourceCode = ShaderSource.c_str();

	glShaderSource(shader, 1, &tempSourceCode, NULL);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader Compilation Failed: ");
	return shader;
}

static string LoadShader(const string& fileName) 
{
	std::ifstream file;
	const char* fileNameChar = fileName.c_str();
	file.open(fileNameChar, ifstream::in);

	string output;
	string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		cerr << "Unable To Load Shader: " << fileName << endl;
	}
	return output;
}

Shader::Shader(const string FileLocation, Camera& camera)
{
	m_camera = &camera;
	Name = FileLocation;
	m_program = glCreateProgram();

	m_Shaders[VERTEX] = CreateShader(LoadShader(FileLocation + ".vert"), GL_VERTEX_SHADER);
	m_Shaders[FRAGMENT] = CreateShader(LoadShader(FileLocation + ".frag"), GL_FRAGMENT_SHADER);

	for (GLuint i = 0; i < NUM_SHADER_TYPES; i++)
	{
		glAttachShader(m_program, m_Shaders[i]);
	}

	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error: Program Linking Failed: ");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program Is Invalid: ");

	m_Uniforms[MODEL_U] = glGetUniformLocation(m_program, "model");
	m_Uniforms[PROJECTION_U] = glGetUniformLocation(m_program, "projection");
	m_Uniforms[VIEW_U] = glGetUniformLocation(m_program, "view");

	for (GLuint i = 0; i < NUM_UNIFORMS; i++)
	{
		if (m_Uniforms[i] == GL_INVALID_INDEX)
			cout << "Shader: " << Name << " Uniform Invalid Index:" << static_cast<UniformNames>(i)
			<< " (Might Be Optimized Out If Not Used)" << endl;
	}
}

void Shader::Bind()
{
	glUseProgram(m_program);
}

GLuint Shader::GetProgram()
{
	return m_program;
}

void Shader::Update(Transform& transform)
{
	mat4 projection = m_camera->GetPerspectiveProjection();
	mat4 view = m_camera->GetViewMatrix();
	mat4 model = transform.GetModel();

	glUniformMatrix4fv(m_Uniforms[MODEL_U], 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(m_Uniforms[PROJECTION_U], 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(m_Uniforms[VIEW_U], 1, GL_FALSE, &view[0][0]);
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADER_TYPES; i++)
	{
		glDetachShader(m_program, m_Shaders[i]);
		glDeleteShader(m_Shaders[i]);
	}
	glDeleteProgram(m_program);
}

