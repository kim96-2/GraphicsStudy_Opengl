#version 460

layout(triangles) in;
layout(line_strip,max_vertices = 2) out;

uniform mat4x4 PROJECTION_MATRIX;//노말을 PROJECTION 노말로 변경하기 위해 가져옴

out DATA
{
	vec3 fCol;
	vec3 fNormal;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_out;

in DATA
{
	vec3 fCol;
	vec3 fNormal;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_in[];

void main(){
	
	vec4 viewNormal = PROJECTION_MATRIX * vec4(data_in[0].fNormal,1.0);
	viewNormal = normalize(viewNormal);

	gl_Position = gl_in[0].gl_Position;

	data_out.fCol = data_in[0].fCol;
	data_out.fNormal = data_in[0].fNormal;
	data_out.viewDirectionWS = data_in[0].viewDirectionWS;
	data_out.lightDirectionWS = data_in[0].lightDirectionWS;

	EmitVertex();

	gl_Position = gl_in[0].gl_Position + viewNormal * 0.2;

	data_out.fCol = data_in[0].fCol;
	data_out.fNormal = data_in[0].fNormal;
	data_out.viewDirectionWS = data_in[0].viewDirectionWS;
	data_out.lightDirectionWS = data_in[0].lightDirectionWS;

	EmitVertex();

	EndPrimitive();
}