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
    void render(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram, bool flashlightOn = false);
    bool checkCollision(const glm::vec3& newPos);
    
private:
    Application& app;
    ResourceManager& rm;
    
    // Models specific to Room 1
    Model carpetModel;
    FBXModel showcaseModel;
    FBXModel fossilsModel;
    FBXModel effigyModel;
    FBXModel ropeBarrierModel;
    FBXModel coffinModel;
    FBXModel mourningFemaleModel;
    FBXModel tigerPaintingModel;
    FBXModel flashlightModel;
    FBXModel chandelierModel;
    
    // Helper methods
    void renderWalls(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderFloorAndCeiling(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderShowcases(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderFossils(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderEffigy(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderRopeBarriers(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderCoffin(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderMourningFemale(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderTigerPainting(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderFlashlight(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderChandelier(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderEKeyPrompt(const glm::mat4& projection);
    
    bool checkFossilCollision(const glm::vec3& newPos);
    bool checkTableCollision(const glm::vec3& newPos);
    bool checkWallCollision(const glm::vec3& newPos);
    
    // Animation state
    bool effigyAnimating = false;
    bool eKeyPressed = false;
    float effigyFloatOffset = 0.0f;
    float effigyAnimTime = 0.0f;
    glm::vec3 effigyPosition = glm::vec3(-17.5f, 4.0f, -7.0f);
    float interactionDistance = 3.0f;
    bool playerNearEffigy = false;
    
    // Coffin animation state
    bool coffinAnimating = false;
    float coffinFloatOffset = 0.0f;
    float coffinAnimTime = 0.0f;
    glm::vec3 coffinPosition = glm::vec3(-17.0f, 4.0f, 27.0f);
    bool playerNearCoffin = false;
    
    // E key prompt UI
    GLuint promptVAO = 0;
    GLuint promptVBO = 0;
    void initPromptUI();
};
