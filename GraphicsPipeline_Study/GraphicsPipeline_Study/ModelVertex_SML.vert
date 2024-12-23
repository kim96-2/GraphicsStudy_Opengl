//SML(Sphere Map Lighting) 용 버텍스 쉐이더
#version 460

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 biTangent;

uniform mat4x4 MODELVIEW_MATRIX;
uniform mat4x4 PROJECTION_MATRIX;

uniform vec3 lightDirection;

out DATA
{
	vec3 normal;
	vec2 uv;
	mat3 TBN;//노멀맵을 사용하기 위한 TBN

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_out;

void main(){
	gl_Position = PROJECTION_MATRIX * MODELVIEW_MATRIX * vec4(pos,1.0);
	//gl_Position = vec4(pos,1.0);//TES에서 MVP변환을 하기위한 무변환 반환

	//월드좌표 계산
	vec3 positionWS = (MODELVIEW_MATRIX * vec4(pos,1.0)).xyz;

	//바라보는 방향(View Direction) 계산
	data_out.viewDirectionWS = vec3(0,0,0) - positionWS;
	
	//빛의 방향은 월드 좌표로 줬다고 가정
	data_out.lightDirectionWS = lightDirection;

	vec3 T = normalize(vec3(MODELVIEW_MATRIX * vec4(tangent,0.0)));
	vec3 B = normalize(vec3(MODELVIEW_MATRIX * vec4(biTangent,0.0)));
	vec3 N = normalize(vec3(MODELVIEW_MATRIX * vec4(normal,0.0)));

	data_out.normal = N;
	data_out.TBN = mat3(T,B,N);

	data_out.uv = uv;
}