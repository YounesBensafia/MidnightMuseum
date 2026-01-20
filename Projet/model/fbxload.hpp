#ifndef FBXLOAD_HPP
#define FBXLOAD_HPP

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>

struct FBXVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct FBXModel {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    size_t vertexCount;
    size_t indexCount;
    GLuint textureID;  // Embedded texture from GLB
    glm::vec3 baseColor; // Base color extracted from texture or material
    std::string name;
};

// Function declarations only
FBXModel loadFBXModel(const char* filepath);
FBXModel loadFBXModelClamped(const char* filepath);

#endif
