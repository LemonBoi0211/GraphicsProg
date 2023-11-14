#version 450

uniform sampler2D texture_diffuse;

uniform vec3 FragLightColor;
uniform vec3 FragLightPos;
uniform vec3 FragCamPos;

in vec3 FragNormal;
in vec3 FragPos;

in vec2 FragTextureCoord;
out vec4 frag_colour;


void main()

{	

	//ambient
	float ambientStrength = 0.5f;
	vec3 ambient = ambientStrength * FragLightColor;

	//diffuse
	vec3 normal = normalize(FragNormal);
	vec3 lightDir = normalize(FragLightPos - FragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * FragLightColor;

	//specular
	float specularStrength = 10.0f;
	vec3 viewDir = normalize(FragCamPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(normal, reflectDir), 0.0f), 32.0f);
	vec3 specular = vec3(specularStrength * spec);

	vec4 result = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb *(ambient + diffuse + spec) , 1);
	frag_colour = result;

}