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
#include "FrameBuffer.h"

#include "SphereMap.h"

//ȭ�� ũ�� ����
#define WIDTH 800
#define HEIGHT 600

//Shader ����
Shader* shaderProgram;
Shader* normalShaderProgram;

//�׸� �� ����
Model* model;

//Sky Map ����
SphereMap* sphereMap;
const char sphereMapPath[] = "./resources/Skymap/ny_spheremap.tga";
const char diffuseMapPath[] = "./resources/Skymap/ny_diffusemap.tga";

//Frame Buffer ����
FrameBuffer* frameBuffer;

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
	//shaderProgram = new Shader("ModelVertex.vert","ModelFragment.frag");
	shaderProgram = new Shader();
	//shaderProgram->AddShader(GL_VERTEX_SHADER, "ModelVertex.vert");					//Vertex Shader �߰�
	//shaderProgram->AddShader(GL_GEOMETRY_SHADER, "ModelGeometry.geom");				//Geometry Shader �߰�
	//shaderProgram->AddShader(GL_FRAGMENT_SHADER, "ModelFragment.frag");				//Fragment Shader �߰�

	shaderProgram->AddShader(GL_VERTEX_SHADER, "ModelVertex_SML.vert");					//Vertex Shader �߰�
	shaderProgram->AddShader(GL_FRAGMENT_SHADER, "ModelFragment_SML.frag");				//Fragment Shader �߰�

	shaderProgram->Use();//Shader Program ���	
}

void InitModel() {
	stbi_set_flip_vertically_on_load(false);

	shaderProgram->Use();//Shader Program ���	
	
	
	char modelPath[] = "./resources/statue/CB_Discobolus_LOD0.FBX";
	model = new Model(modelPath);

	model->SetScale(0.001, 0.001, 0.001);//�� ������ ���̱�

	model->SetTextureDataFromFile("./resources/statue/manstatue.png","texture_diffuse");
	model->SetTextureDataFromFile("./resources/statue/manstatue_N.png", "texture_normal");
	
	
	/*
	char modelPath[] = "./resources/RobotKyle/KyleRobot.fbx";
	model = new Model(modelPath);

	model->SetScale(0.01, 0.01, 0.01);//�� ������ ���̱�

	model->SetTextureDataFromFile("./resources/RobotKyle/KyleRobot_BaseMap.png", "texture_diffuse");
	model->SetTextureDataFromFile("./resources/RobotKyle/KyleRobot_Normal.png", "texture_normal");
	*/
	
	//SphereMap Texture �����ϱ�
	model->SetTextureDataFromFile(sphereMapPath, "texture_sphereMap");
	model->SetTextureDataFromFile(diffuseMapPath, "texture_diffuseMap");
}

void InitSphereMap() {
	//sphereMap = new SphereMap("Vertex.vert", "Fragment.frag",
	sphereMap = new SphereMap("SphereMapVertex.vert", "SphereMapFragment.frag",sphereMapPath);
}

void InitFrameBuffer() {
	frameBuffer = new FrameBuffer("FrameBufferVertex.vert", "FrameBufferFragment.frag", WIDTH, HEIGHT);
}

void InitDisplay() {
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(110, (float)WIDTH / HEIGHT, 0.01, 1000);

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
	//FrameBuffer ���ֱ�(���� ��� �������� ȭ���� �ƴ� ���ο� frameBuffer�� ��)
	frameBuffer->SetFrameBuffer();


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sphereMap->Draw();

	shaderProgram->Use();//Shader Program ���
	shaderProgram->SetModelProjection();//MVP ����

	glLineWidth(1.5);
	DrawModel();
	/*
	for (int _x = -2;_x <= 2;_x++) {
		for (int _y = -5;_y <= 5;_y++) {
			for (int _z = -3;_z <= 3;_z++) {
				glPushMatrix();
				glTranslatef(_x * 100.0, _y * 150.0, _z * 50.0);
				DrawModel();
				glPopMatrix();
			}
			
		}
	}
	*/
	glLineWidth(1.0);

	frameBuffer->DrawFrameBuffer();

	glutSwapBuffers();
}

//�ð� �������� ���� ������Ʈ ���ư��� �ϴ� �Լ�
void TimerFunc(int value) {
	rotateValue += 1.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glRotatef(rotateValue, 0, 1, 0);
	gluLookAt(0, 1, -1.5, 0, 1, 0, 0, 1, 0);
	glRotatef(rotateValue, 0, 1, 0);

	frameBuffer->frameBufferProgram->SetFloat("timer", rotateValue);
	shaderProgram->SetFloat("timer", rotateValue);

	glutPostRedisplay();

	glutTimerFunc(30, TimerFunc, 1);
}

void MouseMove(GLint x, GLint y) {
	frameBuffer->frameBufferProgram->SetVector2f("mousePos", x, y);

	//cout << x << " " << y << endl;

	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	
	//Glut �ʱ�ȭ
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	//������ ũ�� �� ��ġ ����
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(10, 10);

	//������ �̸�
	glutCreateWindow("PipelineTest");

	glewInit();

	//InitShader();
	InitModelShader();

	//InitVertex();
	InitModel();

	InitSphereMap();

	InitFrameBuffer();

	InitDisplay();

	glutDisplayFunc(displayFunc);

	glutTimerFunc(30, TimerFunc, 1);//ȸ�� �ִϸ��̼��� ���� �ݹ� �Լ�

	glutMotionFunc(MouseMove); //���콺 �̵� üũ�� �ݹ� �Լ�

	glutMainLoop(); 

	// VAO�� VBO ���ҽ� ����
	//glDeleteVertexArrays(1, &vao);
	//glDeleteBuffers(1, &vbo);
}
