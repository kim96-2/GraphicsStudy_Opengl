#version 460

// 삼각형 단위로 입력받는다(quads, triangles, isolines 중 하나)
// subdivision mode는 equal_spacing이다 (fractional_even_spacing 또는 fractional_odd_spacing 가능)
// 입력 순서는 시계 방향 (cw 또는 ccw. ccw가 기본값)
layout(triangles, fractional_even_spacing, ccw) in;

//TCS에서 보낸 입력들
in DATA
{
	vec3 fCol;
	vec3 fNormal;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_in[];

//TES에서 내보낼 출력들
out DATA
{
	vec3 fCol;
	vec3 fNormal;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_out;

uniform mat4x4 MODELVIEW_MATRIX;
uniform mat4x4 PROJECTION_MATRIX;

void main(){
	gl_Position =
				( gl_TessCoord.x * gl_in[0].gl_Position 
				+ gl_TessCoord.y * gl_in[1].gl_Position
				+ gl_TessCoord.z * gl_in[2].gl_Position);

	data_out.fCol = gl_TessCoord.x * data_in[0].fCol 
				+ gl_TessCoord.y * data_in[1].fCol
				+ gl_TessCoord.z * data_in[2].fCol;

	data_out.fCol = vec3(1,1,1);

	data_out.fNormal = gl_TessCoord.x * data_in[0].fNormal
				+ gl_TessCoord.y * data_in[1].fNormal
				+ gl_TessCoord.z * data_in[2].fNormal;

	data_out.viewDirectionWS = gl_TessCoord.x * data_in[0].viewDirectionWS 
				+ gl_TessCoord.y * data_in[1].viewDirectionWS
				+ gl_TessCoord.z * data_in[2].viewDirectionWS;

	data_out.lightDirectionWS = gl_TessCoord.x * data_in[0].lightDirectionWS
				+ gl_TessCoord.y * data_in[1].lightDirectionWS
				+ gl_TessCoord.z * data_in[2].lightDirectionWS;

}