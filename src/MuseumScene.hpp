#ifndef MUSEUM_SCENE_HPP
#define MUSEUM_SCENE_HPP

#include "Application.hpp"
#include "ResourceManager.hpp"
#include "Room1.hpp"
#include "Hallway.hpp"
#include "Room2.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <memory>

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

    // Room components
    std::unique_ptr<Room1> room1;
    std::unique_ptr<Hallway> hallway;
    std::unique_ptr<Room2> room2;

    // Shared models
    Model carpetModel;  // Floor that spans all rooms
    ModelWithMaterial flashlightModel;

    // Lighting
    GLuint shaderProgram;
    std::vector<glm::vec3> cornerLights;

    // Game State
    bool flashlightOn;
    bool fKeyPressed;

    // Internal methods
    void checkCollisions();
    void renderFlashlightModel(const glm::mat4& view, const glm::mat4& projection);
    
    // Initializers
    void initLights();
};

#endif
