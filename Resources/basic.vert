#version 450

in vec3 vp;
in vec2 TextureCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 FragTextureCoord;

void main()
{
	FragTextureCoord = TextureCoord;
	gl_Position =  projection * view * model * vec4(vp, 1.0);
}