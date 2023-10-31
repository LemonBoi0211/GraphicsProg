#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include <glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>
#include <iostream>
#include "Mesh.h"
#include "Transforms.hpp"
#include "Vertex.h"
#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "stb_image.h"
#include "Input.h"
#include "LightBase.h"

#undef SDL_main

using namespace std;

GLuint textureID;

void LoadTexture(string TextureLocation)
{
	int width, height, numComponents;

	unsigned char* ImageData = stbi_load(TextureLocation.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);

	if (ImageData == NULL)
	{
		cerr << "Texture Loading Failed For Texture: " << TextureLocation << endl;
	}

	GLenum format;
	if (numComponents == 1)
		format = GL_RED;
	if (numComponents == 3)
		format = GL_RGB;
	if (numComponents == 4)
		format = GL_RGBA;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(ImageData);
}

int main(int argc, char* argv[]) 
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	SDL_Window* window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1200, 1000, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext GLContext = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	GLenum status = glewInit();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	if (status != GLEW_OK)
	{
		std::cout << "GLEW failed to initialize!" << endl;
	}

	Camera* cam = new Camera(90.0f, 1200.0f/1000.0f, 0.1f, 100.0f);
	LightBase* light1 = new LightBase();
	cam->m_transform.setPos(vec3(0, 0, 1));

	Shader* basicShader = new Shader("Resources/basic", *cam);
	
	Input* input = new Input();

	//Square
	vector<Vertex> SquareVerticies;

	SquareVerticies.push_back( Vertex(vec3(-1.0f, 1.0f, 1.0f), vec2(0, 0)));
	SquareVerticies.push_back( Vertex(vec3(1.0f, 1.0f, 1.0f), vec2(1, 0)));
	SquareVerticies.push_back( Vertex(vec3(1.0f, -1.0f, 1.0f), vec2(1, 1)));
	SquareVerticies.push_back( Vertex(vec3(-1.0f, -1.0f, 1.0f), vec2(0, 1)));

	unsigned int SquareIndicies[]
	{
		0,1,2,0,3,2
	};


	//square mesh
	Mesh Square1(&SquareVerticies[0], SquareVerticies.size(), &SquareIndicies[0], 6);

	Square1.m_transform.setPos(vec3(0, 0, -1.0f));


	LoadTexture("assets/brickwall.jpg");

	while (!input->KeyIsPressed(KEY_ESCAPE))
	{
		input->Update();
		light1->Draw(*cam);

		//display Square
		glClearColor(0.3f, 0.3f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, 1200, 1000);

		basicShader->Bind();

		glActiveTexture(GL_TEXTURE0);
		GLuint TextureLoc = glGetUniformLocation(basicShader->GetProgram(), "texture_diffuse");
		glUniform1i(TextureLoc, 0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		basicShader->Update(Square1.m_transform);
		Square1.Draw();

		SDL_GL_SwapWindow(window);

		SDL_Delay(16);
	}

	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	return 0;
	
}


