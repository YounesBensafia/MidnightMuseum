#ifndef MUSEUM_SCENE_HPP
#define MUSEUM_SCENE_HPP

#include "Application.hpp"
#include "ResourceManager.hpp"
#include <vector>
#include <glm/glm.hpp>

class MuseumScene {
public:
    MuseumScene(Application& app);
    ~MuseumScene();

    void init();
    void update(float dt);
    void render();

private:
    Application& app;
    ResourceManager& rm;

    // Game Objects / Models
    Model carpetModel;
    FBXModel showcaseModel;
    FBXModel fossilsModel;
    FBXModel effigyModel;
    FBXModel buddhaModel;
    ModelWithMaterial flashlightModel;

    // Lighting
    GLuint shaderProgram;
    std::vector<glm::vec3> cornerLights;

    // Game State
    bool flashlightOn;
    bool fKeyPressed;

    // Internal methods
    void checkCollisions();
    bool checkFossilCollision(glm::vec3 newPos);
    bool checkWallCollision(glm::vec3 newPos);
    bool checkTableCollision(glm::vec3 newPos);
    
    // Initializers
    void initLights();
};

#endif
