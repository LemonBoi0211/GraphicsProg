#version 450

in vec3 VertexPosition;
in vec2 TextureCoord;
in vec3 Normal;
in vec3 Tangent;
in vec3 BiTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 FragTextureCoord;
out vec3 FragNormal;
out vec3 FragPos;
out mat3 TBN;

void main()
{
	FragTextureCoord = TextureCoord;
	gl_Position =  projection * view * model * vec4(VertexPosition, 1.0);

	FragPos = vec3(model* vec4(VertexPosition, 1.0f));
	FragNormal = mat3(transpose(inverse(model))) *Normal;

	vec3 T = normalize(vec3(model* vec4(Tangent, 0.0)));
	vec3 B = normalize(vec3(model* vec4(BiTangent, 0.0)));
	vec3 N = normalize(vec3(model* vec4(Normal, 0.0)));

	TBN = mat3(T,B,N);
}