#version 460

layout(triangles) in;
layout(line_strip,max_vertices = 3) out;

uniform float timer;

in DATA
{
	vec3 normal;
	vec2 uv;
	mat3 TBN;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_in[];

out DATA
{
	vec3 normal;
	vec2 uv;
	mat3 TBN;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_out;

void main(){

	vec4 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0 ;
	float amount = (1 + sin(timer * 0.1))/2.0 ;


	for(int i=0;i<3;i++){
		//gl_Position = gl_in[i].gl_Position - (gl_in[i].gl_Position - center) * amount* 0.8;
		gl_Position = gl_in[i].gl_Position;

		gl_PointSize = gl_in[i].gl_PointSize * 3;

		data_out.normal = data_in[i].normal;
		data_out.uv = data_in[i].uv;
		data_out.TBN = data_in[i].TBN;

		data_out.viewDirectionWS = data_in[i].viewDirectionWS;
		data_out.lightDirectionWS = data_in[i].lightDirectionWS;

		EmitVertex();
	}
	

	EndPrimitive();

}