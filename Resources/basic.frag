#version 450

uniform sampler2D texture_diffuse;

in vec2 FragTextureCoord;
out vec4 frag_colour;

//"uniform vec3 Color"

void main()

{	
	//frag_colour = vec4(vec3(FragTextureCoord.x,FragTextureCoord.y,0),1.0f);

	frag_colour = vec4(texture2D(texture_diffuse, FragTextureCoord).rgb,1);
	//frag_colour = vec4(1,0,0,1);
}