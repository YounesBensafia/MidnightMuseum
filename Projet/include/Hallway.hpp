#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "../include/ResourceManager.hpp"

class Application;
struct GLFWwindow;

class Hallway {
public:
    Hallway(Application& app);
    ~Hallway();
    
    void init();
    void update(float dt, GLFWwindow* window);
    void render(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    bool checkCollision(const glm::vec3& newPos);
    
    bool isInspecting() const { return inspectionMode; }
    void updateInspectionCamera(glm::mat4& view, glm::mat4& projection);
    void renderInspectionObject(GLuint shaderProgram);
    
    // Spotlight system
    std::vector<glm::vec3> getActiveSpotlightPositions() const;
    
private:
    Application& app;
    ResourceManager& rm;
    
    Model carpetModel;
    FBXModel displayCabinetModel;
    FBXModel smallFossilModel;
    FBXModel smallFossil2Model;
    FBXModel spotlightModel;
    
    // Spotlight activation (one for each cabinet)
    bool cabinetSpotlights[2] = {false, false};  // 0 = east cabinet, 1 = west cabinet
    
    // Inspection system
    bool inspectionMode;
    int inspectedCabinetIndex;  // 0 = east, 1 = west
    float inspectionRotationY;
    float inspectionRotationX;
    bool wasIKeyPressed;
    bool wasMousePressed;
    double lastMouseX, lastMouseY;
    glm::vec3 savedCameraPos;
    float savedCameraYaw;
    float savedCameraPitch;
    bool playerNearCabinet;
    
    // I key prompt UI
    GLuint promptVAO;
    GLuint promptVBO;
    void initPromptUI();
    void renderIKeyPrompt(const glm::mat4& projection);
    
    // Helper methods
    void renderWalls(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderFloorAndCeiling(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderDisplayCabinets(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderSmallFossil(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderCabinetSpotlights(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    
    bool checkWallCollision(const glm::vec3& newPos);
    bool checkCabinetCollision(const glm::vec3& newPos);
    bool isNearCabinet(const glm::vec3& playerPos, int& cabinetIndex);
    void enterInspectionMode(int cabinetIndex);
    void exitInspectionMode();
};
