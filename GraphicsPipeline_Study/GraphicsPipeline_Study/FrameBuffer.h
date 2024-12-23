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
    //FrameBuffer 그려줄 Shader 선언
    frameBufferProgram = new Shader(vertexPath, fragmentPath);

    //FrameBuffer 그려줄 화면 VAO,VBO 선언
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

    //fbo 생성
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //frame buffer의 color buffer용 Texture 생성
    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);

    //glTexImage 생성할 때 마지막 매개변수를 NULL로 하여 위치 할당만 하게 해줌
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //frame buffer에 color buffer를 붙여줌
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    //rbo 생성(Depth 와 Stencil Buffer를 위해 생성)
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    //Stencil과 Depth rbo저장소 생성
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

    //frame buffer에 위 rbo 붙여줌
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //fbo 생성에 문제 없는지 최종 확인
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;

    //fbo 바인딩 끊어주기
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//FrameBuffer 변경해주는 함수
void FrameBuffer::SetFrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FrameBuffer::DrawFrameBuffer() {
    //화면에 그려주기 위해 현재 frame buffer 바인딩 해제
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    //frame buffer를 화면에 그릴때는 Depth test 비활성화
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