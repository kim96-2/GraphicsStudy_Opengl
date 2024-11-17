#version 460

in DATA
{
	vec3 normal;
	vec2 uv;
	mat3 TBN;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

out vec4 oCol;

void main(){
	
	vec4 diffuseColor = texture(texture_diffuse1,data_in.uv);

	//normalize 
	vec3 normal = texture(texture_normal1,data_in.uv).rgb;
	normal = normal * 2.0 - 1.0;
	normal = normalize(data_in.TBN * normal);


	vec3 lightDir = normalize(data_in.lightDirectionWS);
	vec3 vewDir = normalize(data_in.viewDirectionWS);

	float ndotl = dot(normal,lightDir);
	float halfNdotL = ndotl * 0.5 + 0.5;

	float celShade = smoothstep(0.5,0.52,halfNdotL);//�´� ���� ������ �����Ϸ��� ������� �Ǵ��Ѵ�. �����δ� ���� ����
	celShade = 0.4 * (1 - celShade) + 1 * celShade;

	ndotl = ndotl < 0 ? 0 : ndotl;

	//Blinn Phong���� Specular ���
	vec3 H = normalize(lightDir + vewDir);
	float NdotH = dot(normal,H);
	NdotH = NdotH < 0 ? 0 : NdotH;

	float spec = pow(NdotH,100);

	vec3 color = diffuseColor.rgb * celShade + spec;

	oCol =  vec4(1,1,1,1);
	oCol =  vec4(color,1);
}