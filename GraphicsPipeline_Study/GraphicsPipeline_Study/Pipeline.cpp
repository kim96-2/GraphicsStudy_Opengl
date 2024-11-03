#include<gl/glew.h>
#include<GL/glut.h>

#include<stdlib.h>

#include "Shader.h"

//화면 크기 고정
#define WIDTH 500
#define HIGHT 500

//VAO VBO 선언
GLuint vbo;
GLuint vao;

//Shader 선언
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

// 삼각형 데이터
const Color pink_color = { 1.0f, 0.765f, 0.765f };
const Color green_color = { 0.545f, 1.0f, 0.345f };
#pragma endregion

const Vertex vertices[6] = {
	// 핑크색 삼각형
	Vertex{Vec3d{0.5f, -0.5f , 0.f}, pink_color},
	Vertex{Vec3d{-0.5f, -0.5f, 0.f}, green_color},
	Vertex{Vec3d{0.0f, 0.5f, 0.f}, pink_color},

	// 연두색 삼각형
	Vertex{Vec3d{0.8f, 0.1f, 0.f}, green_color},
	Vertex{Vec3d{0.8f, -0.1f, 0.f}, pink_color},
	Vertex{Vec3d{0.6f, -0.3f, 0.f}, green_color},
};

void InitVertex() {
	//VBO 만들기
	//glCreateBuffers(1, &vbo);
	//glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &vbo);//위 glCreateBuffers하고의 차이점은 없어보이는데 더 찾아봐야 한다

	//GL_ARRAY_BUFFER로 바인딩 후 데이터 넣어주기
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//VAO 만들기
	glGenVertexArrays(1, &vao);//VAO 생성
	glBindVertexArray(vao); // VAO 바인딩

	//0번째 어트리뷰트에 포지션값 할당 및 활성화
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	//std::cout << offsetof(Vertex, position) << std::endl;

	//1번째 어트리뷰트에 컬러값 할당 및 활성화
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	//std::cout << 3 * sizeof(GL_FLOAT) << std::endl;

	//state 초기화(더 공부해야할 것들 있는 듯)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void InitShader() {
	//쉐이더 프로그램 생성 및 초기화
	//shaderProgram = new Shader("vertex.vert", "fragment.frag");
	shaderProgram = new Shader();

	shaderProgram->AddShader(GL_VERTEX_SHADER, "vertex.vert");

	//Tessellation Shader 추가
	//shaderProgram->AddShader(GL_TESS_CONTROL_SHADER, "TessCS.glsl");
	//shaderProgram->AddShader(GL_TESS_EVALUATION_SHADER, "TessES.glsl");

	//Geomatry Shader 추가
	shaderProgram->AddShader(GL_GEOMETRY_SHADER, "geomaty.geom");

	shaderProgram->AddShader(GL_FRAGMENT_SHADER, "fragment.frag");

	shaderProgram->Use();//Shader Program 사용

	GLint out;
	glGetIntegerv(GL_PATCH_VERTICES, &out);
	std::cout << out << std::endl;
}

void InitDisplay() {
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (float)WIDTH / HIGHT, 0.01, 15);

	//광원 방향 Shader에 지정 및 전달
	shaderProgram->SetVector3f("lightDirection", -1.0f, 1.0f, 0.5f);
}

void displayFunc() {
	glClearColor(0.0f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//생성했던 vao 바인딩(활성화)
	//glBindVertexArray(vao);

	//버텍스 그리기
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	shaderProgram->SetModelProjection();

	glColor3f(1.0f, .5f, .5f);

	
	glutSolidTeapot(0.5);
	//glutWireTeapot(0.5);
	

	glutSwapBuffers();
}

//시간 지나갈떄 마다 오브젝트 돌아가게 하는 함수
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
	
	//Glut 초기화
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	//윈도우 크기 및 위치 설정
	glutInitWindowSize(WIDTH, HIGHT);
	glutInitWindowPosition(10, 10);

	//윈도우 이름
	glutCreateWindow("PipelineTest");

	glewInit();

	InitShader();

	InitVertex();

	InitDisplay();

	glutDisplayFunc(displayFunc);

	glutTimerFunc(30, TimerFunc, 1);//회전 애니메이션을 위한 콜백 함수

	glutMainLoop(); 

	// VAO와 VBO 리소스 해제
	//glDeleteVertexArrays(1, &vao);
	//glDeleteBuffers(1, &vbo);
}
