#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <glad/glad.h>
#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include "../model/objload.hpp"
#include "../model/fbxload.hpp"

// Definitions
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;
};

struct Model {
    GLuint VAO;
    GLuint VBO;
    size_t vertexCount;
    std::string name;
};

struct ModelWithMaterial {
    GLuint VAO;
    GLuint VBO;
    size_t vertexCount;
    std::string name;
    std::vector<std::string> materialNames; // Per-vertex material names
    std::map<std::string, Material> materials; // Material library
};
struct FBXModel;

class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    // Textures
    GLuint loadTexture(const std::string& name, const char* path);
    GLuint getTexture(const std::string& name);

    // Shaders
    GLuint loadShader(const std::string& name, const char* vertexPath, const char* fragmentPath);
    GLuint getShader(const std::string& name);

    // Models
    // We'll use simple wrappers around existing load functions for now
    Model loadModel(const char* filepath, bool regenerateUVs = false);
    ModelWithMaterial loadModelWithMaterial(const char* filepath);
    FBXModel loadFBXModel(const char* filepath);

private:
    ResourceManager() {}
    std::map<std::string, GLuint> textures;
    std::map<std::string, GLuint> shaders;
};

#endif
