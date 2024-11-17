#pragma once

#include<GL/glew.h>
#include<GL/glut.h>
#include<glm.hpp>
#include<gtc/type_ptr.hpp>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader {
public:
	//Program ID
	GLint ID;

	//생성자
	Shader();
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void AddShader(const GLuint shaderType, const std::string& shaderPath);

	//Program 사용 함수
	void Use();

	//Uniform 설정 함수
	void SetTexture(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetVector3f(const std::string& name, float x,float y,float z);
	void SetMatrix4fv(const std::string& name, GLfloat* matrix4);
	void SetMatrix4fv(const std::string& name, glm::mat4x4 matrix4);

	//Model,Projection Matrix 설정 함수
	void SetModelProjection();
};

void Shader::AddShader(GLuint shaderType, const std::string& shaderPath) {
	//파일 경로를 통해 shader 파일 읽어오기
	std::string shaderCode;

	std::ifstream shaderFile;

	//ifstream 객체가 예외를 던질 수 있게 한다는데... 정확한건 더 공부해야됨
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	//Shader ID 변수
	GLuint shaderID;

	//에러 확인용 변수
	int success;
	char infoLog[512];

	try {
		//실제 파일 읽기
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		//최종 stream string값으로 변환
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR :: 쉐이더 파일 읽기 실패" << std::endl;
	}

	const char* shader_char = shaderCode.c_str();

	//Shader 생성 및 컴파일
	shaderID = glCreateShader(shaderType);//Vertex Shader  생성
	glShaderSource(shaderID, 1, &shader_char, NULL);// Vertex Shader 내용 추가
	glCompileShader(shaderID);//컴파일 진행

	//Shader 오류 확인
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "ERROR::" << shaderPath << " 컴파일 오류\n" << infoLog << std::endl;
	}

	//Shader 프로그램에 적용
	glAttachShader(ID, shaderID);

	glLinkProgram(ID);//TO DO : 얘가 정확히 어떤걸 해야하는지 확인 필요

	//에러 확인
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::" << shaderPath <<" Linking 오류\n" << infoLog << std::endl;
	}

	//Program에 Shader link가 완료 되었으면 Shader삭제
	glDeleteShader(shaderID);
}

Shader::Shader() {
	//Shader Program 생성
	ID = glCreateProgram();
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	
	//Shader Program 생성
	ID = glCreateProgram();

	AddShader(GL_VERTEX_SHADER, vertexPath);
	AddShader(GL_FRAGMENT_SHADER, fragmentPath);

	/*
	//파일 경로를 통해 shader 파일 읽어오기
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//ifstream 객체가 예외를 던질 수 있게 한다는데... 정확한건 더 공부해야됨
	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		//실제 파일 읽기
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		//최종 stream string값으로 변환
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR :: 쉐이더 파일 읽기 실패" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//Shader ID 변수
	GLuint vertex, fragment;
	
	//에러 확인용 변수
	int success;
	char infoLog[512];

	//Vertex Shader 부분
	vertex = glCreateShader(GL_VERTEX_SHADER);//Vertex Shader  생성
	glShaderSource(vertex, 1, &vShaderCode, NULL);// Vertex Shader 내용 추가
	glCompileShader(vertex);//컴파일 진행

	//Vertex Shader 오류 확인
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::Vertex Shader 컴파일 오류\n" << infoLog << std::endl;
	}

	//Fragment Shader 부분
	fragment = glCreateShader(GL_FRAGMENT_SHADER);//Fragment Shader  생성
	glShaderSource(fragment, 1, &fShaderCode, NULL);// Fragment Shader 내용 추가
	glCompileShader(fragment);//컴파일 진행

	//Fragment Shader 오류 확인
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::Fragment Shader 컴파일 오류\n" << infoLog << std::endl;
	}

	//Shader들 프로그램에 적용
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);

	glLinkProgram(ID);//TO DO : 얘가 정확히 어떤걸 해야하는지 확인 필요

	//에러 확인
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::Shader program Linking 오류\n" << infoLog << std::endl;
	}

	//Program에 Shader link가 완료 되었으면 Shader삭제
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	*/
}

void Shader::Use() {
	glUseProgram(ID);
}

void Shader::SetTexture(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVector3f(const std::string& name, float x, float y, float z) {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::SetMatrix4fv(const std::string &name, GLfloat * matrix4) {

	glm::mat4x4 glmMat;

	for (int i = 0;i < 4;i++) {
		for (int j = 0; j < 4;j++) {
			glmMat[i][j] = matrix4[i * 4 + j];
		}

	}
	
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(glmMat));
}

void Shader::SetMatrix4fv(const std::string& name, glm::mat4x4 matrix4) {
	//glGetUniformLocation(ID, "MODELVIEW_MATRIX");

	glUniformMatrix4fv(glGetUniformLocation(ID, "MODELVIEW_MATRIX"), 1, GL_FALSE, glm::value_ptr(matrix4));
}

void Shader::SetModelProjection() {
	
	//모델 뷰 행렬 가져와서 uniform으로 넘기기
	GLfloat model[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, model);

	SetMatrix4fv("MODELVIEW_MATRIX", model);

	//for(int i=0;i<16;i++) std::cout << model[i] << " " ;
	//std::cout << "\n";

	//프로젝션 뷰 행렬 가져와서 uniform으로 넘기기
	GLfloat projection[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	SetMatrix4fv("PROJECTION_MATRIX", projection);
}