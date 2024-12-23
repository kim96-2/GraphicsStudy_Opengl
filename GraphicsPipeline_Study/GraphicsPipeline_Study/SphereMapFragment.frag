#version 460

#define PI 3.1415926535897932384626433832795

uniform sampler2D skyMapTexture;

in DATA
{
	vec3 position;//Projection Space Position
} data_in;

out vec4 oCol;

void main(){
	
	
	vec3 pos = normalize(data_in.position);

	float theta = atan(pos.x,pos.z);
	float phi = acos(pos.y);

	float u = theta / (2 * PI) + 0.5;
	float v = phi / (1 * PI) + 0.0;
	//float v = pos.y /1.2;

	vec4 col = texture(skyMapTexture, vec2(u,v));

	oCol = vec4(col.xyz,1.0);

	//oCol = vec4(1,1,1,1);
}