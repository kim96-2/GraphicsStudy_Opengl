#include<gl/glew.h>
#include<GL/glut.h>

#include<stdlib.h>

#include "Shader.h"

//ȭ�� ũ�� ����
#define WIDTH 500
#define HIGHT 500

//VAO VBO ����
GLuint vbo;
GLuint vao;

//Shader ����
Shader* shaderProgram;

float rotateValue;

#pragma region  VERTEX INFO
struct Vec3d {
	float x;
	float y;
	float z;
};

struct Color {
	float r;
	float g;
	float b;
};

struct Vertex {
	Vec3d position;
	Color color;
};

// �ﰢ�� ������
const Color pink_color = { 1.0f, 0.765f, 0.765f };
const Color green_color = { 0.545f, 1.0f, 0.345f };
#pragma endregion

const Vertex vertices[6] = {
	// ��ũ�� �ﰢ��
	Vertex{Vec3d{0.5f, -0.5f , 0.f}, pink_color},
	Vertex{Vec3d{-0.5f, -0.5f, 0.f}, green_color},
	Vertex{Vec3d{0.0f, 0.5f, 0.f}, pink_color},

	// ���λ� �ﰢ��
	Vertex{Vec3d{0.8f, 0.1f, 0.f}, green_color},
	Vertex{Vec3d{0.8f, -0.1f, 0.f}, pink_color},
	Vertex{Vec3d{0.6f, -0.3f, 0.f}, green_color},
};

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

void InitShader() {
	//���̴� ���α׷� ���� �� �ʱ�ȭ
	//shaderProgram = new Shader("vertex.vert", "fragment.frag");
	shaderProgram = new Shader();

	shaderProgram->AddShader(GL_VERTEX_SHADER, "vertex.vert");

	//Tessellation Shader �߰�
	//shaderProgram->AddShader(GL_TESS_CONTROL_SHADER, "TessCS.glsl");
	//shaderProgram->AddShader(GL_TESS_EVALUATION_SHADER, "TessES.glsl");

	//Geomatry Shader �߰�
	shaderProgram->AddShader(GL_GEOMETRY_SHADER, "geomaty.geom");

	shaderProgram->AddShader(GL_FRAGMENT_SHADER, "fragment.frag");

	shaderProgram->Use();//Shader Program ���

	GLint out;
	glGetIntegerv(GL_PATCH_VERTICES, &out);
	std::cout << out << std::endl;
}

void InitDisplay() {
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (float)WIDTH / HIGHT, 0.01, 15);

	//���� ���� Shader�� ���� �� ����
	shaderProgram->SetVector3f("lightDirection", -1.0f, 1.0f, 0.5f);
}

void displayFunc() {
	glClearColor(0.0f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�����ߴ� vao ���ε�(Ȱ��ȭ)
	//glBindVertexArray(vao);

	//���ؽ� �׸���
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	shaderProgram->SetModelProjection();

	glColor3f(1.0f, .5f, .5f);

	
	glutSolidTeapot(0.5);
	//glutWireTeapot(0.5);
	

	glutSwapBuffers();
}

//�ð� �������� ���� ������Ʈ ���ư��� �ϴ� �Լ�
void TimerFunc(int value) {
	rotateValue += 1.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, -2.5, 0, 0, 0, 0, 1, 0);
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

	InitShader();

	InitVertex();

	InitDisplay();

	glutDisplayFunc(displayFunc);

	glutTimerFunc(30, TimerFunc, 1);//ȸ�� �ִϸ��̼��� ���� �ݹ� �Լ�

	glutMainLoop(); 

	// VAO�� VBO ���ҽ� ����
	//glDeleteVertexArrays(1, &vao);
	//glDeleteBuffers(1, &vbo);
}
