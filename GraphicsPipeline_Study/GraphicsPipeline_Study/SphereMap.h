#pragma once

#include "Shader.h"
#include "Model.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <string>
#include <vector>
using namespace std;


//360�� �̹����� ����ϴ� Sky mapping Ŭ����
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
	//Sky Map�� ���� ���� �� ��Ȱ��ȭ 
	glDepthMask(GL_FALSE);

	glEnable(GL_CULL_FACE);

	//�ĸ� �ø� 
	glCullFace(GL_FRONT);

	sphereMapShader->Use();
	sphereMapShader->SetModelProjection();//MVP ����

	sphereModel->Draw(sphereMapShader);

	//�ٽ� ���� �� Ȱ��ȭ
	glDepthMask(GL_TRUE);

	//���� �ø����� �ʱ�ȭ
	glCullFace(GL_BACK);
}

