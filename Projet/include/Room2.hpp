#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../include/ResourceManager.hpp"

class Application;
struct GLFWwindow;

class Room2 {
public:
    Room2(Application& app);
    ~Room2();
    
    void init();
    void update(float dt, GLFWwindow* window);
    void render(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    bool checkCollision(const glm::vec3& newPos);
    
private:
    Application& app;
    ResourceManager& rm;
    
    Model carpetModel;
    FBXModel buddhaModel;
    
    // Helper methods
    void renderWalls(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderFloorAndCeiling(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderBuddha(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    
    bool checkWallCollision(const glm::vec3& newPos);
};
