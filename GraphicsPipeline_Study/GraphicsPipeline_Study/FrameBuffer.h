#pragma once
#include "Shader.h"

#include<GL/glew.h>
#include<GL/glut.h>

#include<stdlib.h>
using namespace std;

float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

class FrameBuffer {
public:
    Shader* frameBufferProgram;

	FrameBuffer(const GLchar* vertexPath, const GLchar* fragmentPath, unsigned int width, unsigned int height);

	void SetFrameBuffer();

	void DrawFrameBuffer();

private:

    GLuint VAO, VBO;

    GLuint fbo,rbo;

    GLuint textureColorBuffer;
};

FrameBuffer::FrameBuffer(const GLchar* vertexPath, const GLchar* fragmentPath, unsigned int width, unsigned int height) {
    //FrameBuffer �׷��� Shader ����
    frameBufferProgram = new Shader(vertexPath, fragmentPath);

    //FrameBuffer �׷��� ȭ�� VAO,VBO ����
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Bind VAO
    glBindVertexArray(VAO);

    //Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    //Vertex Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    //Vertex UV
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    //fbo ����
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //frame buffer�� color buffer�� Texture ����
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

    //glTexImage ������ �� ������ �Ű������� NULL�� �Ͽ� ��ġ �Ҵ縸 �ϰ� ����
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //frame buffer�� color buffer�� �ٿ���
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    //rbo ����(Depth �� Stencil Buffer�� ���� ����)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    //Stencil�� Depth rbo����� ����
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    //frame buffer�� �� rbo �ٿ���
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //fbo ������ ���� ������ ���� Ȯ��
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;

    //fbo ���ε� �����ֱ�
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//FrameBuffer �������ִ� �Լ�
void FrameBuffer::SetFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FrameBuffer::DrawFrameBuffer() {
    //ȭ�鿡 �׷��ֱ� ���� ���� frame buffer ���ε� ����
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    //frame buffer�� ȭ�鿡 �׸����� Depth test ��Ȱ��ȭ
    glDisable(GL_DEPTH_TEST);

    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    frameBufferProgram->Use();

    frameBufferProgram->SetTexture("ColorBufferTexture", 0);
    glActiveTexture(GL_TEXTURE0);  
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}