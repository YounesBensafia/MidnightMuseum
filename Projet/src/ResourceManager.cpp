#include "../include/ResourceManager.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../shader/shader/shader.hpp"

// Implementations of load functions from main.cpp, but as methods

GLuint ResourceManager::loadTexture(const std::string& name, const char* path) {
    if (textures.find(name) != textures.end()) {
        return textures[name];
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    
    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 1) format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        std::cout << "Texture loaded: " << path << std::endl;
        stbi_image_free(data);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
    }
    
    textures[name] = textureID;
    return textureID;
}

GLuint ResourceManager::getTexture(const std::string& name) {
    return textures[name];
}

GLuint ResourceManager::loadShader(const std::string& name, const char* vertexPath, const char* fragmentPath) {
     GLuint programID = LoadShaders(vertexPath, fragmentPath);
     shaders[name] = programID;
     return programID;
}

GLuint ResourceManager::getShader(const std::string& name) {
    return shaders[name];
}

// ... Copy loadModel, loadModelWithMaterial, loadFBXModel logic here ...
// Since these are loose functions in main.cpp, we need to adapt them or link them properly.
// For now, I will include the implementations directly or assume they are available via headers.
// The headers objload.hpp/fbxload.hpp likely only DECLARE them if they were separate modules, 
// but in main.cpp they were implemented.
// Wait, looking at file list: objload.cpp and fbxload.cpp EXIST. 
// So I don't need to re-implement loadOBJ/loadFBX, I just need to implement the wrapper `loadModel` which constructs the VAO/VBO.

// Moving loadModel logic here
Model ResourceManager::loadModel(const char* filepath, bool regenerateUVs) {
    Model model;
    model.name = filepath;
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    
    // We assume loadOBJ is available from objload.hpp included
    bool res = loadOBJ(filepath, positions, uvs, normals);
    if (!res) {
        printf("Failed to load: %s\n", filepath);
        model.vertexCount = 0;
        return model;
    }
    
    printf("Loaded %s: %zu vertices\n", filepath, positions.size());
    
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.position = positions[i];
        
        if (regenerateUVs) {
            v.normal = glm::vec3(0, 1, 0);
            float tileSize = 1.0f;
            v.texture = glm::vec2(positions[i].x / tileSize, positions[i].z / tileSize);
        } else {
            v.normal = (i < normals.size()) ? normals[i] : glm::vec3(0, 1, 0);
            v.texture = (i < uvs.size()) ? uvs[i] : glm::vec2(0, 0);
        }
        vertices.push_back(v);
    }
    
    model.vertexCount = vertices.size();
    
    glGenVertexArrays(1, &model.VAO);
    glBindVertexArray(model.VAO);
    
    glGenBuffers(1, &model.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return model;
}

ModelWithMaterial ResourceManager::loadModelWithMaterial(const char* filepath) {
    ModelWithMaterial model;
    model.name = filepath;
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    
    bool res = loadOBJWithMaterials(filepath, positions, uvs, normals, model.materialNames, model.materials);
    if (!res) {
        printf("Failed to load: %s\n", filepath);
        model.vertexCount = 0;
        return model;
    }
    
    printf("Loaded %s: %zu vertices\n", filepath, positions.size());
    
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.position = positions[i];
        v.normal = normals[i];
        v.texture = uvs[i];
        vertices.push_back(v);
    }
    
    model.vertexCount = vertices.size();
    glGenVertexArrays(1, &model.VAO);
    glBindVertexArray(model.VAO);
    glGenBuffers(1, &model.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return model;
}

FBXModel ResourceManager::loadFBXModel(const char* filepath) {
    // This assumes ::loadFBXModel is globally available from #include "../model/fbxload.hpp"
    // However, the global function name might conflict with this method name.
    // We should call the global one.
    return ::loadFBXModel(filepath);
}
