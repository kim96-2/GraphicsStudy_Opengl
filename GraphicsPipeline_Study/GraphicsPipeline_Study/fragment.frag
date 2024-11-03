#version 460

in DATA
{
	vec3 fCol;
	vec3 fNormal;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_in;

out vec4 oCol;

void main(){
	
	//normalize 
	vec3 normal = normalize(data_in.fNormal);
	vec3 lightDir = normalize(data_in.lightDirectionWS);
	vec3 vewDir = normalize(data_in.viewDirectionWS);

	float ndotl = dot(normal,lightDir);
	float halfNdotL = ndotl * 0.5 + 0.5;

	ndotl = ndotl < 0 ? 0 : ndotl;

	//Blinn Phong으로 Specular 계산
	vec3 H = normalize(lightDir + vewDir);
	float NdotH = dot(normal,H);
	NdotH = NdotH < 0 ? 0 : NdotH;

	float spec = pow(NdotH,10);

	vec3 color = data_in.fCol * halfNdotL + spec;

	oCol =  vec4(color,1);
}