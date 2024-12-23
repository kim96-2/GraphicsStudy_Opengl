#pragma once

#include "Shader.h"
#include "Model.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <string>
#include <vector>
using namespace std;


//360도 이미지를 사용하는 Sky mapping 클래스
class SphereMap {
public:

	Shader* sphereMapShader;

	SphereMap(const GLchar* vertexPath, const GLchar* fragmentPath, const char* skymapPath);

	void Draw();

private:
	Model *sphereModel;
};

SphereMap::SphereMap(const GLchar* vertexPath, const GLchar* fragmentPath, const char* skymapPath) {

	sphereMapShader = new Shader(vertexPath, fragmentPath);
	
	char path[] = "./resources/Sphere/Sphere.fbx";
	sphereModel = new Model(path);

	sphereModel->SetTextureDataFromFile(skymapPath, "texture_diffuse");

	//sphereModel->SetScale(2, 2, 2);
}

void SphereMap::Draw() {
	//Sky Map을 위한 뎁스 비교 비활성화 
	glDepthMask(GL_FALSE);

	glEnable(GL_CULL_FACE);

	//후면 컬링 
	glCullFace(GL_FRONT);

	sphereMapShader->Use();
	sphereMapShader->SetModelProjection();//MVP 연동

	sphereModel->Draw(sphereMapShader);

	//다시 뎁스 비교 활성화
	glDepthMask(GL_TRUE);

	//전면 컬링으로 초기화
	glCullFace(GL_BACK);
}

