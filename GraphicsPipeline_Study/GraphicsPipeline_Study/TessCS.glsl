#version 460

layout(vertices=3) out; // 정점 3개의 출력

//Vertex Shader에서 받은 입력들
in DATA
{
	vec3 fCol;
	vec3 fNormal;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_in[];

out DATA
{
	vec3 fCol;
	vec3 fNormal;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_out[];

void main(){
	
	if(gl_InvocationID == 0){
		//내부 테셀레이션 정점
		gl_TessLevelInner[0] = 2;

		//외부 테셀레이션 정점
		gl_TessLevelOuter[0] = 2;
		gl_TessLevelOuter[1] = 2;
		gl_TessLevelOuter[2] = 2;
	}
	

	//gl Position 패스 쓰루
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

	//그 외 출력값들 패스 쓰루
	data_out[gl_InvocationID].fCol = data_in[gl_InvocationID].fCol;
	data_out[gl_InvocationID].fNormal = data_in[gl_InvocationID].fNormal;
	data_out[gl_InvocationID].viewDirectionWS = data_in[gl_InvocationID].viewDirectionWS;
	data_out[gl_InvocationID].lightDirectionWS = data_in[gl_InvocationID].lightDirectionWS;

}