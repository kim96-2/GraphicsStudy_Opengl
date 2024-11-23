#pragma once
#include "Shader.h"
#include "Mesh.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char* path, bool gamma = false);

class Model
{
public:
    /*  함수   */
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(Shader* shader);

    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

    void SetTextureDataFromFile(const char* path, string type);
private:
    vector<Texture> textures_loaded;//최적화를 위해 실제 불러온 Texture인지 확인하는 변수

    /*  Model 데이터  */
    vector<Mesh> meshes;
    string directory;


    /*  함수   */
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    
};

void Model::Draw(Shader* shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(*shader);
}

void Model::loadModel(string path) {
    Assimp::Importer import;

    //씬 읽어오기(aiProcess_Triangulate => 모든 매쉬 삼각형으로, aiProcess_FlipUVs => UV 뒤집어서 읽기)
    const aiScene * scene = import.ReadFile(path, 
        aiProcess_Triangulate | 
        aiProcess_FlipUVs |
        //aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_GenUVCoords
    );
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));//이것도 정확히 이해 안감

    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);

}

//assimp에서 재귀적으로 접근하는 노드(mesh 및 메테리얼 정보 저장)
void Model::processNode(aiNode* node, const aiScene* scene)
{
    //현재 노드의 모든 메쉬 가져오기
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.(중요 : 각 노드는 메쉬 index만 가지고 있다..?) 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // 각 메쉬의 정점 데이터들 처리
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
            
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    /*Texture위치가 이상하게 잡ㅎ힌게 있는 이유로 Texture는 외부에서 호출 및 잡아주기
    // 메테리얼 데이터 불러오기
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    */

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        //최적화 부분: 이미 불러온 텍스쳐라면 넘어가고 아니면 불러오기
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)//불러온 텍스쳐들 전부 확인
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)//텍스쳐를 불러온 적 없다면 불러오기
        {   
            Texture texture;
            texture.id = TextureFromFile(str.C_Str());
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}

unsigned int TextureFromFile(const char* path, bool gamma)
{
    //std::cout << "in" << std::endl;
    //텍스쳐 파일 위치가 모델 파일 위치와 동일해야됨
    string filename = string(path);

    //filename = directory + '/' + filename;

    //Texture 버퍼 생성
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    //이미지 불러오기
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)//한 색깔만의 이미지일 때
            format = GL_RED;
        else if (nrComponents == 3)//RGB 세가지 색상 이미지 일 때
            format = GL_RGB;
        else //nrComponents == 4//RGBA로 투명도 포함 이미지 일 때
            format = GL_RGBA;

        std::cout  << "texture size : " << width << " " << height << std::endl;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        //Sampler 설정
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else//이미지 찾지 못했을 시
    {
        std::cout << "Texture failed to load at path: " << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Model::SetTextureDataFromFile(const char* path,string type ) {
    string filename = string(path);

    //Texture 버퍼 생성
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    //이미지 불러오기
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)//한 색깔만의 이미지일 때
            format = GL_RED;
        else if (nrComponents == 3)//RGB 세가지 색상 이미지 일 때
            format = GL_RGB;
        else //nrComponents == 4//RGBA로 투명도 포함 이미지 일 때
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        //Sampler 설정
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else//이미지 찾지 못했을 시
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    Texture texture;
    texture.id = textureID;
    texture.path = path;
    texture.type = type;

    //원래 이러면 안되지만 하나의 매쉬에 모든 texture데이터를 전부 넣기 위해 이래 만듬
    meshes[0].textures.push_back(texture);

    //return texture;
}
