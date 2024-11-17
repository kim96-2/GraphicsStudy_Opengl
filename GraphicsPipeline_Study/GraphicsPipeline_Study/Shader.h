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

	//������
	Shader();
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void AddShader(const GLuint shaderType, const std::string& shaderPath);

	//Program ��� �Լ�
	void Use();

	//Uniform ���� �Լ�
	void SetTexture(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetVector3f(const std::string& name, float x,float y,float z);
	void SetMatrix4fv(const std::string& name, GLfloat* matrix4);
	void SetMatrix4fv(const std::string& name, glm::mat4x4 matrix4);

	//Model,Projection Matrix ���� �Լ�
	void SetModelProjection();
};

void Shader::AddShader(GLuint shaderType, const std::string& shaderPath) {
	//���� ��θ� ���� shader ���� �о����
	std::string shaderCode;

	std::ifstream shaderFile;

	//ifstream ��ü�� ���ܸ� ���� �� �ְ� �Ѵٴµ�... ��Ȯ�Ѱ� �� �����ؾߵ�
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	//Shader ID ����
	GLuint shaderID;

	//���� Ȯ�ο� ����
	int success;
	char infoLog[512];

	try {
		//���� ���� �б�
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		//���� stream string������ ��ȯ
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR :: ���̴� ���� �б� ����" << std::endl;
	}

	const char* shader_char = shaderCode.c_str();

	//Shader ���� �� ������
	shaderID = glCreateShader(shaderType);//Vertex Shader  ����
	glShaderSource(shaderID, 1, &shader_char, NULL);// Vertex Shader ���� �߰�
	glCompileShader(shaderID);//������ ����

	//Shader ���� Ȯ��
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "ERROR::" << shaderPath << " ������ ����\n" << infoLog << std::endl;
	}

	//Shader ���α׷��� ����
	glAttachShader(ID, shaderID);

	glLinkProgram(ID);//TO DO : �갡 ��Ȯ�� ��� �ؾ��ϴ��� Ȯ�� �ʿ�

	//���� Ȯ��
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::" << shaderPath <<" Linking ����\n" << infoLog << std::endl;
	}

	//Program�� Shader link�� �Ϸ� �Ǿ����� Shader����
	glDeleteShader(shaderID);
}

Shader::Shader() {
	//Shader Program ����
	ID = glCreateProgram();
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	
	//Shader Program ����
	ID = glCreateProgram();

	AddShader(GL_VERTEX_SHADER, vertexPath);
	AddShader(GL_FRAGMENT_SHADER, fragmentPath);

	/*
	//���� ��θ� ���� shader ���� �о����
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//ifstream ��ü�� ���ܸ� ���� �� �ְ� �Ѵٴµ�... ��Ȯ�Ѱ� �� �����ؾߵ�
	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		//���� ���� �б�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		//���� stream string������ ��ȯ
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR :: ���̴� ���� �б� ����" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//Shader ID ����
	GLuint vertex, fragment;
	
	//���� Ȯ�ο� ����
	int success;
	char infoLog[512];

	//Vertex Shader �κ�
	vertex = glCreateShader(GL_VERTEX_SHADER);//Vertex Shader  ����
	glShaderSource(vertex, 1, &vShaderCode, NULL);// Vertex Shader ���� �߰�
	glCompileShader(vertex);//������ ����

	//Vertex Shader ���� Ȯ��
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::Vertex Shader ������ ����\n" << infoLog << std::endl;
	}

	//Fragment Shader �κ�
	fragment = glCreateShader(GL_FRAGMENT_SHADER);//Fragment Shader  ����
	glShaderSource(fragment, 1, &fShaderCode, NULL);// Fragment Shader ���� �߰�
	glCompileShader(fragment);//������ ����

	//Fragment Shader ���� Ȯ��
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::Fragment Shader ������ ����\n" << infoLog << std::endl;
	}

	//Shader�� ���α׷��� ����
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);

	glLinkProgram(ID);//TO DO : �갡 ��Ȯ�� ��� �ؾ��ϴ��� Ȯ�� �ʿ�

	//���� Ȯ��
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::Shader program Linking ����\n" << infoLog << std::endl;
	}

	//Program�� Shader link�� �Ϸ� �Ǿ����� Shader����
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
	
	//�� �� ��� �����ͼ� uniform���� �ѱ��
	GLfloat model[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, model);

	SetMatrix4fv("MODELVIEW_MATRIX", model);

	//for(int i=0;i<16;i++) std::cout << model[i] << " " ;
	//std::cout << "\n";

	//�������� �� ��� �����ͼ� uniform���� �ѱ��
	GLfloat projection[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	SetMatrix4fv("PROJECTION_MATRIX", projection);
}