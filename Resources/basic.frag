#version 450

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform sampler2D texture_shadow;

uniform vec3 FragLightColor;
uniform vec3 FragLightPos;
uniform vec3 FragCamPos;

in vec3 FragNormal;
in vec3 FragPos;
in mat3 TBN;

in vec2 FragTextureCoord;
out vec4 frag_colour;
in vec4 fragPosLightSpace;

float CalculateShadowValue()
{
	vec3 projCoords = fragPosLightSpace.xyz/ fragPosLightSpace.w;
	projCoords = projCoords * 0.5 +0.5;

	float closestDepth = texture(texture_shadow, projCoords.xy).r;

	float currentDepth = projCoords.z;

	float bias = 0.0005;


	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(texture_shadow, 0);
	for(int x = -1; x<= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(texture_shadow, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	if(projCoords.z >1.0)
	{
		shadow = 0;
	}

	return shadow;
}

void main()

{	

	//ambient
	float ambientStrength = 0.5f;
	vec3 ambient = ambientStrength * FragLightColor;

	//diffuse & normal
	//vec3 normal = normalize(FragNormal);

	vec3 normal = normalize(texture2D(texture_normal, FragTextureCoord).rgb);
	normal = normalize(normal * 2.0-1.0);
	normal = normalize(TBN*normal);

	vec3 lightDir = normalize(FragLightPos - FragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * FragLightColor;

	//specular
	float specularStrength = 1.0f;
	vec3 viewDir = normalize(FragCamPos - FragPos);
	vec3 reflectDir = reflect(lightDir, normal);
	float spec = pow(max(dot(normal, reflectDir), 0.0f), 32.0f);
	vec3 specular = vec3(specularStrength * spec);

	float shadow = CalculateShadowValue();

	vec4 result = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb +(ambient +(1.0 -shadow)) * (diffuse + specular),0);
	//result = vec4(vec3((1.0 -shadow)),0);


	frag_colour = result;
	
}