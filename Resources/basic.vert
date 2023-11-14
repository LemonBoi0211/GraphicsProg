#version 450

in vec3 VertexPosition;
in vec2 TextureCoord;
in vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 FragTextureCoord;
out vec3 FragNormal;
out vec3 FragPos;

void main()
{
	FragTextureCoord = TextureCoord;
	gl_Position =  projection * view * model * vec4(VertexPosition, 1.0);

	FragPos = vec3(model* vec4(VertexPosition, 1.0f));
	FragNormal = mat3(transpose(inverse(model))) *Normal;
}