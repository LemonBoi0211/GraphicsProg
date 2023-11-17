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
#include "OBJLoader.h"

#undef SDL_main

using namespace std;

GLuint LoadTexture(string TextureLocation)
{
	GLuint textureID;

	int width, height, numComponents;

	unsigned char* ImageData = stbi_load(TextureLocation.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);

	if (ImageData == NULL)
	{
		cerr << "Texture Loading Failed For Texture: " << TextureLocation << endl;
	}

	GLenum format{};
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

	return textureID;
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

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	if (status != GLEW_OK)
	{
		std::cout << "GLEW failed to initialize!" << endl;
	}

	Camera* cam = new Camera(90.0f, 1200.0f/1000.0f, 0.1f, 1000.0f);

	LightBase* light1 = new LightBase();
	
	cam->m_transform.setPos(vec3(0, 0, -1));

	Shader* basicShader = new Shader("Resources/basic", *cam);

	Input* input = new Input( );

	/*GLuint DiffuseTextureID = LoadTexture("assets/brickwall.jpg");
	GLuint NormalTextureID = LoadTexture("assets/brickwall_normal.jpg");*/
	
	////Square
	//vector<Vertex> SquareVerticies;

	//SquareVerticies.push_back( Vertex(vec3(-1.0f, 1.0f, 0), vec2(0, 0))); //0
	//SquareVerticies.push_back( Vertex(vec3(1.0f, -1.0f, 0), vec2(1, 1))); //1
	//SquareVerticies.push_back(Vertex(vec3(1.0f, 1.0f, 0), vec2(1, 0))); //2
	//SquareVerticies.push_back( Vertex(vec3(-1.0f, -1.0f, 0), vec2(0, 1))); //3

	//unsigned int SquareIndicies[]
	//{
	//	0,2,1,1,3,0
	//};

	////square mesh
	//Mesh Square1(&SquareVerticies[0], SquareVerticies.size(), &SquareIndicies[0], 6);

	//Square1.m_transform.setPos(vec3(0, 0, 1.0f));

	//LoadTexture("assets/brickwall.jpg");

	//block obj
	string AmbiantLoc = "brick_wall_01_col.tga";
	string DiffuseLoc = "brick_wall_01_col.tga";
	string SpecLoc = "brick_wall_spec.tga";
	string NormalLoc = "brick_wall_03b_norm.tga";

	vector<uint> Indices;

	vector<Vertex> LoadedVerts = OBJLoader::LoadOBJ("../GraphicsProg/assets/Block", "blocks_01.obj",
		AmbiantLoc, DiffuseLoc, SpecLoc, NormalLoc, Indices);

	GLuint AmbiantTextureID = LoadTexture("../GraphicsProg/assets/Block/" + AmbiantLoc);
	GLuint DiffuseTextureID = LoadTexture("../GraphicsProg/assets/Block/" + DiffuseLoc);
	GLuint SpecularTextureID = LoadTexture("../GraphicsProg/assets/Block/" + SpecLoc);
	GLuint NormalTextureID = LoadTexture("../GraphicsProg/assets/Block/" + NormalLoc);

	//block mesh
	Mesh Block1(&LoadedVerts[0], LoadedVerts.size(), &Indices[0], Indices.size());
	Block1.m_transform.setPos(vec3(0, -0.5f, 1));
	Block1.m_transform.setScale(vec3(0.025f, 0.025f, 0.025f));

	float SinInput = 0;

	while (!input->KeyIsPressed(KEY_ESCAPE))
	{
		input->Update();
		

		//cam move
		if (input->KeyIsPressed(KEY_W))
		{
			cout << "Forward" << endl;
			cam->MoveForwardLocal(-0.1f);
		}

		if (input->KeyIsPressed(KEY_S))
		{
			cout << "Back" << endl;
			cam->MoveForwardLocal(0.1f);
		}

		if (input->KeyIsPressed(KEY_A))
		{
			cout << "Left" << endl;
			cam->MoveRightLocal(-0.1f);
		}

		if (input->KeyIsPressed(KEY_D))
		{
			cout << "Right" << endl;
			cam->MoveRightLocal(0.1f);
		}

		//viewport and lifhrs stuff
		glClearColor(0.3f, 0.3f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, 1200, 1000);
		SinInput += 0.03f;
		light1->GetTransform().setPos(vec3(sin(SinInput), 0, 0));
		light1->Draw(*cam);
		cam->Update( );


		//bind shader
		basicShader->Bind();

		//bind texture
		glActiveTexture(GL_TEXTURE0);
		GLuint TextureLoc = glGetUniformLocation(basicShader->GetProgram(), "texture_diffuse");
		glUniform1i(TextureLoc, 0);
		glBindTexture(GL_TEXTURE_2D, DiffuseTextureID);

		//bind texture normal
		glActiveTexture(GL_TEXTURE1);
		TextureLoc = glGetUniformLocation(basicShader->GetProgram(), "texture_normal");
		glUniform1i(TextureLoc, 1);
		glBindTexture(GL_TEXTURE_2D, NormalTextureID);

		/*Square1.m_transform.setRot(vec3(180, 0, 0), true);
		basicShader->Update(Square1.m_transform, *light1);
		Square1.Draw();*/

		basicShader->Update(Block1.m_transform, *light1);
		Block1.Draw( );

		SDL_GL_SwapWindow(window);

		SDL_Delay(16);
	}

	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	return 0;
	
}


