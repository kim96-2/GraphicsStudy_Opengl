#version 460

layout(triangles) in;
layout(triangle_strip,max_vertices = 3) out;

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
	vec3 vector0 = (gl_in[0].gl_Position - gl_in[1].gl_Position).xyz;
	vec3 vector1 = (gl_in[1].gl_Position - gl_in[2].gl_Position).xyz;
	vec4 surfaceNormal = vec4(normalize(cross(vector0,vector1)),0.0) * 0.2;

	vec4 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0 ;

	gl_Position = gl_in[0].gl_Position + surfaceNormal - (gl_in[0].gl_Position - center) * 0.4;
	data_out.fCol = data_in[0].fCol;
	data_out.fNormal = data_in[0].fNormal;
	data_out.viewDirectionWS = data_in[0].viewDirectionWS;
	data_out.lightDirectionWS = data_in[0].lightDirectionWS;

	EmitVertex();

	gl_Position = gl_in[1].gl_Position + surfaceNormal - (gl_in[1].gl_Position - center) * 0.4;
	data_out.fCol = data_in[1].fCol;
	data_out.fNormal = data_in[1].fNormal;
	data_out.viewDirectionWS = data_in[1].viewDirectionWS;
	data_out.lightDirectionWS = data_in[1].lightDirectionWS;

	EmitVertex();

	gl_Position = gl_in[2].gl_Position + surfaceNormal - (gl_in[2].gl_Position - center) * 0.4;
	data_out.fCol = data_in[2].fCol;
	data_out.fNormal = data_in[2].fNormal;
	data_out.viewDirectionWS = data_in[2].viewDirectionWS;
	data_out.lightDirectionWS = data_in[2].lightDirectionWS;

	EmitVertex();

	EndPrimitive();
}