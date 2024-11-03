#version 460

layout(location = 0) in vec3 pos;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 col;

uniform mat4x4 MODELVIEW_MATRIX;
uniform mat4x4 PROJECTION_MATRIX;

uniform vec3 lightDirection;

out DATA
{
	vec3 fCol;
	vec3 fNormal;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_out;

void main(){
	gl_Position = PROJECTION_MATRIX * MODELVIEW_MATRIX * vec4(pos,1.0);
	//gl_Position = vec4(pos,1.0);//TES���� MVP��ȯ�� �ϱ����� ����ȯ ��ȯ

	data_out.fCol = col;

	//������ǥ ���
	vec3 positionWS = (MODELVIEW_MATRIX * vec4(pos,1.0)).xyz;

	//�ٶ󺸴� ����(View Direction) ���
	data_out.viewDirectionWS = vec3(0,0,0) - positionWS;
	
	//���� ������ ���� ��ǥ�� ��ٰ� ����
	data_out.lightDirectionWS = lightDirection;

	data_out.fNormal = (MODELVIEW_MATRIX * vec4(normal,0.0)).xyz;
}