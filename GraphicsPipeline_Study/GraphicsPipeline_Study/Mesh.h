#pragma once

#include "Shader.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <string>
#include <vector>
using namespace std;

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
private:
    /*  ���� ������  */
    unsigned int VAO, VBO, EBO;

    /*  �Լ�         */
    void SetupMesh();

    void AddTextures(Texture texture);

public:
    /*  Mesh ������  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    
    /*  �Լ�         */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader shader);

};

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    SetupMesh();
}

void Mesh::SetupMesh() {
    //VBO, VAO, EBO ����
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Bind VAO
    glBindVertexArray(VAO);
    
    //Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    //Bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    //�ʱ�ȭ
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //std::cout << "Vertex Count : " << indices.size() << std::endl;

    //for (int i = 0;i < 10;i++) {
    //    std::cout << "Vertex Ex : " << vertices[i].Position.x << " " << vertices[i].Position.y << " " << vertices[i].Position.z << " " << std::endl;
    //}
}

//�ӽ÷� �ܺο��� Texture ������ ���� �� �ְ� ���� �Լ�
void Mesh::AddTextures(Texture texture) {
    this->textures.push_back(texture);
}

void Mesh::Draw(Shader shader) {
    //�ؽ��� �̸��� ���⼭ ��Ģ ����
    //texture_diffuse1, texture_diffuse2, texture_diffuse3, ...
    //texture_specular1, texture_specular2, texture_specular3, ...

    
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string


        //std::cout << "Set Texture : " << name << number << endl;
        // now set the sampler to the correct texture unit(sampler�� int�� ���ε� ������ �����ε�)
        shader.SetTexture((name + number).c_str(), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    
    //Shader Program���� MVP��� �޾ƿ� �ҷ�����
    shader.SetModelProjection();

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    //�ʱ�ȭ
    glActiveTexture(GL_TEXTURE0);
}