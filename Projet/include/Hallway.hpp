#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../include/ResourceManager.hpp"

class Application;

class Hallway {
public:
    Hallway(Application& app);
    ~Hallway();
    
    void init();
    void render(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    bool checkCollision(const glm::vec3& newPos);
    
private:
    Application& app;
    ResourceManager& rm;
    
    Model carpetModel;
    
    // Helper methods
    void renderWalls(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderFloorAndCeiling(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    
    bool checkWallCollision(const glm::vec3& newPos);
};
