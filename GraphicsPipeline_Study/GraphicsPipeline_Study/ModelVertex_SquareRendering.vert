#version 460

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 biTangent;

uniform mat4x4 MODELVIEW_MATRIX;
uniform mat4x4 PROJECTION_MATRIX;

uniform vec3 lightDirection;

#define PI 3.1415926535897932384626433832795

out DATA
{
	vec3 normal;
	vec2 uv;
	mat3 TBN;//��ָ��� ����ϱ� ���� TBN

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_out;

void main(){
	//��� �並 �߾��� ���� ���� �� �� ���� �������ϴ� ��� �׽�Ʈ

	//������ǥ ���
	vec3 positionWS = (MODELVIEW_MATRIX * vec4(pos,1.0)).xyz;

	float dis = length(positionWS);
	vec3 dir = positionWS / dis;

	float x_a = atan(dir.x,-dir.z);
	//float x_a = acos(-dir.z / sqrt(dir.x * dir.x + dir.z * dir.z));
	//x_a = dir.x > 0 ? x_a : -x_a;

	float y_a = acos(-dir.y);
	//float y_a = acos(-dir.z / sqrt(dir.y * dir.y + dir.z * dir.z));
	//y_a = dir.y > 0 ? y_a : -y_a;

	gl_Position = vec4(x_a /PI, y_a /PI,dis / 1000, 1);

	

	//�ٶ󺸴� ����(View Direction) ���
	data_out.viewDirectionWS = vec3(0,0,0) - positionWS;
	
	//���� ������ ���� ��ǥ�� ��ٰ� ����
	data_out.lightDirectionWS = lightDirection;

	vec3 T = normalize(vec3(MODELVIEW_MATRIX * vec4(tangent,0.0)));
	vec3 B = normalize(vec3(MODELVIEW_MATRIX * vec4(biTangent,0.0)));
	vec3 N = normalize(vec3(MODELVIEW_MATRIX * vec4(normal,0.0)));

	data_out.normal = N;
	data_out.TBN = mat3(T,B,N);

	data_out.uv = uv;
}