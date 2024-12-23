//SML(Sphere Map Lighting) 용 프레그먼트 쉐이더
#version 460

#define PI 3.1415926535897932384626433832795

in DATA
{
	vec3 normal;
	vec2 uv;
	mat3 TBN;

	vec3 viewDirectionWS;
	vec3 lightDirectionWS;
} data_in;

uniform mat4x4 MODELVIEW_MATRIX;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

uniform sampler2D texture_sphereMap;
uniform sampler2D texture_diffuseMap;

out vec4 oCol;

//박상일 교수님이 제공하신 Fresnel 계산식
float calculateFresnel(float fresnelPower,float NdotV){
	float F = 0;
	float ratio = F + (1.0 - F)*pow(1.0 - NdotV,fresnelPower);
	
	return fresnelPower > 10 ? 0 : ratio;
}

//커스텀 Phong 함수
vec3 calculatePhong(vec3 diffuseColor,float NdotL,float NdotH){
	float ambient = 0.3;

	float celShade = smoothstep(0.5,0.52,NdotL);
	celShade = mix(ambient,1.0,celShade);

	float specular = pow(NdotH,50);

	return diffuseColor * celShade + specular;

}

//방향을 가지고 SphereMap 에서 대응하는 UV값 계산하는 함수
vec2  calculateSphereMapUV(vec3 dir){
	dir = normalize(dir);
	
	float theta = atan(dir.x,dir.z);
	float phi = acos(-dir.y);

	float u = theta / (2 * PI) + 0.5;
	float v = phi / (1 * PI) + 0.0;

	return vec2(u,v);
}

void main(){
	
	vec4 diffuseColor = texture(texture_diffuse1,data_in.uv);

	//normalize 
	vec3 normal = texture(texture_normal1,data_in.uv).rgb;
	normal = normal * 2.0 - 1.0;
	normal = normalize(data_in.TBN * normal);

	vec3 lightDir = normalize(data_in.lightDirectionWS);
	vec3 viewDir = normalize(data_in.viewDirectionWS);
	vec3 H = normalize(lightDir + viewDir);

	float NdotL = max(dot(lightDir,normal),0);
	float halfNdotL = NdotL * 0.5 + 0.5;
	float NdotV = max(dot(viewDir,normal),0);
	float NdotH = max(dot(H,normal),0);

	vec3 phongColor = calculatePhong(diffuseColor.rgb,halfNdotL,NdotH);

	float fresnelValue = calculateFresnel(0.1,NdotV);

	vec3 reflectDir = reflect(lightDir,normal);
	mat4x4 viewRotMat = mat4(mat3(MODELVIEW_MATRIX));//야메 방법 -> 카메라 회전 행렬을 ModelView 행렬에서 계산
	reflectDir = (viewRotMat * vec4(reflectDir,1)).xyz;

	vec3 reflectColor = texture(texture_sphereMap,calculateSphereMapUV(reflectDir)).rgb;

	vec3 finalColor = mix(phongColor,reflectColor,fresnelValue);

	oCol = vec4(finalColor,1);
}