#include <iostream>
#include "Application.hpp"
#include "MuseumScene.hpp"

// This project is split into 3 parts for collaboration:
// 1. Core Engine (Application.cpp/hpp): Main loop, Window, Input.
// 2. Asset Management (ResourceManager.cpp/hpp): Loading Textures, Models, Shaders.
// 3. Gameplay/Scene (MuseumScene.cpp/hpp): The specific museum logic and rendering.

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
