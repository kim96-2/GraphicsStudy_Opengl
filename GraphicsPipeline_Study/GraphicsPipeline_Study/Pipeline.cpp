#include<gl/glew.h>
#include<GL/glut.h> 

#include <assimp/Importer.hpp>    
#include <assimp/scene.h>             
#include <assimp/postprocess.h>     
#include <assimp/cimport.h>

#include<stdlib.h>
#include <filesystem>
#include <iostream>

#include "Shader.h"
#include "Model.h"

//ȭ�� ũ�� ����
#define WIDTH 800
#define HIGHT 800

//Shader ����
Shader* shaderProgram;
Shader* normalShaderProgram;

//�׸� �� ����
Model* model;

float rotateValue;

#pragma region  VERTEX INFO
struct Color {
	float r;
	float g;
	float b;
};

// �ﰢ�� ������
const Color pink_color = { 1.0f, 0.765f, 0.765f };
const Color green_color = { 0.545f, 1.0f, 0.345f };
#pragma endregion

#pragma region Vertex Making Test
/*
void InitVertex() {
	//VBO �����
	//glCreateBuffers(1, &vbo);
	//glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &vbo);//�� glCreateBuffers�ϰ��� �������� ����̴µ� �� ã�ƺ��� �Ѵ�

	//GL_ARRAY_BUFFER�� ���ε� �� ������ �־��ֱ�
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//VAO �����
	glGenVertexArrays(1, &vao);//VAO ����
	glBindVertexArray(vao); // VAO ���ε�

	//0��° ��Ʈ����Ʈ�� �����ǰ� �Ҵ� �� Ȱ��ȭ
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	//std::cout << offsetof(Vertex, position) << std::endl;

	//1��° ��Ʈ����Ʈ�� �÷��� �Ҵ� �� Ȱ��ȭ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	//std::cout << 3 * sizeof(GL_FLOAT) << std::endl;

	//state �ʱ�ȭ(�� �����ؾ��� �͵� �ִ� ��)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
*/
#pragma endregion

void InitShader() {
	//���̴� ���α׷� ���� �� �ʱ�ȭ
	//shaderProgram = new Shader("vertex.vert", "fragment.frag");
	shaderProgram = new Shader();

	shaderProgram->AddShader(GL_VERTEX_SHADER, "vertex.vert");					//Vertex Shader �߰�
	//Tessellation Shader �߰�
	//shaderProgram->AddShader(GL_TESS_CONTROL_SHADER, "TessCS.glsl");
	//shaderProgram->AddShader(GL_TESS_EVALUATION_SHADER, "TessES.glsl");
	//shaderProgram->AddShader(GL_GEOMETRY_SHADER, "geomaty.geom");				//Geometry Shader �߰�
	shaderProgram->AddShader(GL_FRAGMENT_SHADER, "fragment.frag");				//Fragment Shader �߰�

	shaderProgram->Use();//Shader Program ���

	//�븻 ǥ�ÿ� Shader
	normalShaderProgram = new Shader();

	normalShaderProgram->AddShader(GL_VERTEX_SHADER, "vertex.vert");			//Vertex Shader �߰�
	normalShaderProgram->AddShader(GL_GEOMETRY_SHADER, "normalGeomatry.geom");	//Geomatry Shader �߰�
	normalShaderProgram->AddShader(GL_FRAGMENT_SHADER, "normalFragment.frag");	//Fragment Shader �߰�


}

void InitModelShader() {
	shaderProgram = new Shader("ModelVertex.vert","ModelFragment.frag");

}

void InitModel() {
	stbi_set_flip_vertically_on_load(false);

	shaderProgram->Use();//Shader Program ���

	char modelPath[] = "./resources/statue/CB_Discobolus_LOD0.FBX";
	model = new Model(modelPath);

	model->SetTextureDataFromFile("./resources/statue/manstatue.png","texture_diffuse");
	model->SetTextureDataFromFile("./resources/statue/manstatue_N.png", "texture_normal");
	//model->SetTextureDataFromFile(".\resources\statue\manstatue_S.png", "texture_specular");
	
	/*
	unsigned int diffuse = TextureFromFile("./resources/statue/manstatue.png");
	shaderProgram->SetTexture("texture_diffuse1", 0);

	glActiveTexture(GL_TEXTURE0);
	
	glBindTexture(GL_TEXTURE_2D, diffuse);

	unsigned int normal = TextureFromFile("./resources/statue/manstatue_N.png");
	shaderProgram->SetTexture("texture_normal1", 1);

	glActiveTexture(GL_TEXTURE1);
	
	glBindTexture(GL_TEXTURE_2D, normal);
	*/
}

void InitDisplay() {
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (float)WIDTH / HIGHT, 0.01, 10000);

	//���� ���� Shader�� ���� �� ����
	shaderProgram->SetVector3f("lightDirection", -1.0f, 1.0f, 0.5f);
}

void DrawTeaPot() {
	glColor3f(1.0f, .5f, .5f);
	glutSolidTeapot(0.5);

	normalShaderProgram->Use();
	normalShaderProgram->SetModelProjection();

	glutSolidTeapot(0.5);
}

void DrawModel() {
	//glActiveTexture(GL_TEXTURE0);
	//glActiveTexture(GL_TEXTURE1);
	model->Draw(shaderProgram);
}

void displayFunc() {
	glClearColor(0.0f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderProgram->Use();//Shader Program ���
	shaderProgram->SetModelProjection();//MVP ����
	
	DrawModel();

	glutSwapBuffers();
}

//�ð� �������� ���� ������Ʈ ���ư��� �ϴ� �Լ�
void TimerFunc(int value) {
	rotateValue += 1.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 1000, -3000, 0, 1000, 0, 0, 1, 0);
	glRotatef(rotateValue, 0, 1, 0);

	glutPostRedisplay();

	glutTimerFunc(30, TimerFunc, 1);
}

int main(int argc, char* argv[]) {
	
	//Glut �ʱ�ȭ
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	//������ ũ�� �� ��ġ ����
	glutInitWindowSize(WIDTH, HIGHT);
	glutInitWindowPosition(10, 10);

	//������ �̸�
	glutCreateWindow("PipelineTest");

	glewInit();

	//InitShader();
	InitModelShader();

	//InitVertex();
	InitModel();

	InitDisplay();

	glutDisplayFunc(displayFunc);

	glutTimerFunc(30, TimerFunc, 1);//ȸ�� �ִϸ��̼��� ���� �ݹ� �Լ�

	glutMainLoop(); 

	// VAO�� VBO ���ҽ� ����
	//glDeleteVertexArrays(1, &vao);
	//glDeleteBuffers(1, &vbo);
}
