#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../include/ResourceManager.hpp"

class Application;
struct GLFWwindow;

class Room1 {
public:
    Room1(Application& app);
    ~Room1();
    
    void init();
    void update(float dt, GLFWwindow* window);
    void render(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    bool checkCollision(const glm::vec3& newPos);
    
private:
    Application& app;
    ResourceManager& rm;
    
    // Models specific to Room 1
    Model carpetModel;
    FBXModel showcaseModel;
    FBXModel fossilsModel;
    FBXModel effigyModel;
    
    // Helper methods
    void renderWalls(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderFloorAndCeiling(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderShowcases(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderFossils(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderEffigy(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    
    bool checkFossilCollision(const glm::vec3& newPos);
    bool checkTableCollision(const glm::vec3& newPos);
    bool checkWallCollision(const glm::vec3& newPos);
};
