#include <iostream>
#include "core/Application.hpp"
#include "scene/MuseumScene.hpp"

// This project is split into modules for collaboration:
// 1. core/     - Engine core (Application, ResourceManager, shader, audio)
// 2. model/    - Model loaders (obj, fbx)
// 3. scene/    - Scene/room logic (Room1, Room2, Hallway, MuseumScene)

int main()
{
    // Initialize Application (Core Engine)
    Application app("Museum 3D - Collaborative Project", 1280, 720);
    
    if (!app.init()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return -1;
    }
    
    // Create and Initialize Scene (Gameplay & Content)
    MuseumScene scene(app);
    scene.init();
    
    // Run Main Loop
    // We pass lambda functions for Update and Render steps
    app.run(
        [&](float dt) { 
            scene.update(dt); 
        }, 
        [&]() { 
            scene.render(); 
        }
    );

    return 0;
}
