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
    
    // Get active spotlight positions for lighting calculations
    std::vector<glm::vec3> getActiveSpotlightPositions() const;
    
private:
    Application& app;
    ResourceManager& rm;
    float doorOpenAngle = 0.0f;   // Current angle
    bool isDoorOpening = false;   // Toggle state
    float doorTargetAngle = 90.0f; // Maximum open position
    bool playerNearDoor = false;



    // --- ADD THESE NEW VARIABLES ---
    bool fKeyPressed = false;    // Tracks if F is being held
    bool flashlightOn = false;   // Tracks if flashlight is actually ON
    
    // --- ADD THIS FUNCTION PROTOTYPE ---
    void playSFX(const std::string& alias);
    // Models specific to Room 1
    Model carpetModel;
    FBXModel showcaseModel;
    FBXModel fossilsModel;
    FBXModel effigyModel;
    FBXModel ropeBarrierModel;
    FBXModel coffinModel;
    FBXModel mourningFemaleModel;
    FBXModel tigerPaintingModel;
    FBXModel painting2Model;
    FBXModel painting3Model;
    FBXModel flashlightModel;
    FBXModel chandelierModel;
    FBXModel skirtingBoardModel;
    FBXModel skullModel;
    FBXModel lampModel;
    FBXModel statueModel;
    FBXModel headModel;
    FBXModel wallLampModel;
    FBXModel doorFrameModel;
    FBXModel doorModel;

    // Spotlight model for tables
    FBXModel spotlightModel;
    bool tableSpotlights[6]; // Track which table lights are on
    
    // Helper methods
    void renderWalls(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderFloorAndCeiling(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderShowcases(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderFossils(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderNewArtifacts(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderRopeBarriers(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderMourningFemale(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderTigerPainting(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderAdditionalPaintings(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderFlashlight(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderChandelier(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderSkirtingBoards(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderSkull(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderEKeyPrompt(const glm::mat4& projection);
    void renderTableSpotlights(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderLamps(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderWallLamps(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderDoor(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
        
    bool checkDoorCollision(const glm::vec3& newPos);   
    bool checkFossilCollision(const glm::vec3& newPos);
    bool checkTableCollision(const glm::vec3& newPos);
    bool checkWallCollision(const glm::vec3& newPos);
    bool checkRopeBarrierCollision(const glm::vec3& newPos);
    bool checkSkullCollision(const glm::vec3& newPos);
    bool checkMourningStatueCollision(const glm::vec3& newPos);
    
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
    
    // Mourning statue state
    bool mourningActivated = false;
    glm::vec3 mourningPosition = glm::vec3(-17.0f, 1.0f, 10.0f);
    bool playerNearMourning = false;
    
    // E key prompt UI
    GLuint promptVAO = 0;
    GLuint promptVBO = 0;
    void initPromptUI();
};
