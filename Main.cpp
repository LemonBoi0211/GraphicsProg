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
	light1->GetTransform().setPos(vec3(0, 1, -5));


	cam->m_transform.setPos(vec3(0, 0.5f, -1));

	Shader* basicShader = new Shader("Resources/basic", *cam);
	Shader* depthShader = new Shader("Resources/depthShader", *cam);


	Input* input = new Input( );

	
	
	//Square
	vector<Vertex> SquareVerticies;

	SquareVerticies.push_back( Vertex(vec3(-1.0f, 1.0f, 0), vec2(0, 0))); //0
	SquareVerticies.push_back( Vertex(vec3(1.0f, -1.0f, 0), vec2(1, 1))); //1
	SquareVerticies.push_back(Vertex(vec3(1.0f, 1.0f, 0), vec2(1, 0))); //2
	SquareVerticies.push_back( Vertex(vec3(-1.0f, -1.0f, 0), vec2(0, 1))); //3

	unsigned int SquareIndicies[]
	{
		0,2,1,1,3,0
	};

	//2D plane for floor
	Mesh Square1(&SquareVerticies[ 0 ], SquareVerticies.size( ), &SquareIndicies[ 0 ], 6);
	Square1.m_transform.setScale(Square1.m_transform.getScale( ) * 50.0f);
	Square1.m_transform.setRot(vec3(glm::radians(90.0f), 0.0f, 0.0f));
	Square1.m_transform.setPos(vec3(0, -0.6f, 0));

	LoadTexture("assets/brickwall.jpg");

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

	//create depth texture
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	GLuint shadowMapID;
	int shadowWidth = 2048;
	int shadowHeight = 2048;

	glGenTextures(1, &shadowMapID);
	glBindTexture(GL_TEXTURE_2D, shadowMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[ ] = { 1.0,1.0,1.0,1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE )
		cerr << "ERROR: Frame Buffer is Incomplete" << endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);


	//variable for moving light
	float SinInput = 0;

	while (!input->KeyIsPressed(KEY_ESCAPE))
	{
		input->Update(*cam);
		
		
		//cam move
		if (input->KeyIsPressed(KEY_W))
		{
			cout << "Forward" << endl;
			cam->MoveForwardLocal(0.1f);
		}

		if (input->KeyIsPressed(KEY_S))
		{
			cout << "Back" << endl;
			cam->MoveForwardLocal(-0.1f);
		}

		if (input->KeyIsPressed(KEY_A))
		{
			cout << "Left" << endl;
			cam->MoveRightLocal(0.1f);
		}

		if (input->KeyIsPressed(KEY_D))
		{
			cout << "Right" << endl;
			cam->MoveRightLocal(-0.1f);
		}

		//dEPTH PASS FOR SHADOWS
		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		GLfloat near_plane = 1.0f, far_plane = 100.0f;
		mat4 lightProjection = ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
		vec3 t = light1->GetTransform( ).getPos( );
		mat4 lightView = lookAt(light1->GetTransform( ).getPos( ), vec3(0), vec3(0, 1, 0));
		mat4 lightSpaceMatrix = lightProjection * lightView;

		//bind depth shader
		depthShader->Bind();
		
		//draw cube shadow map
		depthShader->UpdateForShadows(Block1.m_transform, lightSpaceMatrix);
		glCullFace(GL_FRONT);
		Block1.Draw( );
		glCullFace(GL_BACK);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		//dRAW PASS FOR EVERTHIGN ELSE 
		glClearColor(0.0f, 0.15f, 0.3f, 1.0f);
		glViewport(0, 0, 1200, 1000);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		SinInput += 0.03f;
		light1->GetTransform().AddPosition(vec3(sin(SinInput), 0, 0));
		light1->Draw(*cam);
		cam->Update( );

		//bind basic shader
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

		//bind shadow map
		glActiveTexture(GL_TEXTURE2);
		TextureLoc = glGetUniformLocation(basicShader->GetProgram( ), "texture_shadow");
		glUniform1i(TextureLoc, 2);
		glBindTexture(GL_TEXTURE_2D, shadowMapID);

		//draw square plane
		basicShader->Update(Square1.m_transform, *light1, lightSpaceMatrix);
		Square1.Draw();

		//draw cube
		basicShader->Update(Block1.m_transform, *light1, lightSpaceMatrix);
		Block1.Draw();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		SDL_GL_SwapWindow(window);

		SDL_Delay(16);
	}

	SDL_GL_DeleteContext(GLContext);
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
	return 0;
	
}


