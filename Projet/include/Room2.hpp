#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include <string>
#include "../include/ResourceManager.hpp"

class Application;
struct GLFWwindow;

// Data structure for exhibit configuration
struct ExhibitData {
    std::string name;
    std::string modelPath;
    glm::vec3 position;
    glm::vec3 rotation;    // in degrees
    glm::vec3 scale;
    std::string description;
};

// Individual exhibit instance
struct Exhibit {
    FBXModel model;
    glm::mat4 transform;
    std::string name;
    std::string description;
};

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
    FBXModel TutaModel;
    FBXModel monsterModel;
    FBXModel sekiModel;
    FBXModel torsoModel;
    FBXModel peopleModel;
    FBXModel modelMinEgy;
    
    std::vector<Exhibit> exhibits;
    
    // Helper methods
    void renderWalls(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderFloorAndCeiling(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderBuddha(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderTuta(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderMonster(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderSeki(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderTorso(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderPeople(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderMinEgy(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    void renderExhibits(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram);
    
    // Data-driven configuration
    std::vector<ExhibitData> loadGalleryData();
    void initializeExhibits();
    
    bool checkWallCollision(const glm::vec3& newPos);
};
