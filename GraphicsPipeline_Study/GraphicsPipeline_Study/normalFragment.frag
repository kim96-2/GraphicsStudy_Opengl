#version 460

uniform mat4x4 PROJECTION_MATRIX;

in DATA
{
	vec3 fCol;
	vec3 fNormal;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_in;

out vec4 oCol;

void main(){
	
	oCol = PROJECTION_MATRIX * vec4(data_in.fNormal,1.0);
}