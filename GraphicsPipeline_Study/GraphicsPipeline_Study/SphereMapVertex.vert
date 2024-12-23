#version 460

layout(location = 0) in vec3 pos;

uniform mat4x4 MODELVIEW_MATRIX;
uniform mat4x4 PROJECTION_MATRIX;


out DATA
{
	vec3 position;//Projection Space Position
} data_out;

void main(){
	
	float scaleAmount = 5;

	mat4x4 scale;
	scale[0] = vec4(scaleAmount,0,0,0);
	scale[1] = vec4(0,scaleAmount,0,0);
	scale[2] = vec4(0,0,scaleAmount,0);
	scale[3] = vec4(0,0,0,1);

	float rotAmount = 3.14;

	mat4x4 rot;
	rot[0] = vec4(cos(rotAmount),0,-sin(rotAmount),0);
	rot[1] = vec4(0,1,0,0);
	rot[2] = vec4(sin(rotAmount),0,cos(rotAmount),0);
	rot[3] = vec4(0,0,0,1);

	mat4x4 move;
	move[0] = vec4(1,0,0,0);
	move[1] = vec4(0,1,0,500);
	move[2] = vec4(0,0,1,100);
	move[3] = vec4(0,0,0,1);

	//모델뷰 메트릭스에서 이동 부분 제거
	mat4x4 modelview = mat4(mat3(MODELVIEW_MATRIX));

    vec4 posPS =  PROJECTION_MATRIX * modelview * scale * vec4(pos,1.0);
	posPS.z = 0;

	gl_Position = posPS;

	data_out.position = pos;


	
}