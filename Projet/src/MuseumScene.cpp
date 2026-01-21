#include "../include/MuseumScene.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

MuseumScene::MuseumScene(Application& app) 
    : app(app), rm(ResourceManager::getInstance()), flashlightOn(false), fKeyPressed(false) {
}

MuseumScene::~MuseumScene() {
    // Cleanup handled by ResourceManager and unique_ptr
}

void MuseumScene::init() {
    printf("\n=== Loading Museum Scene ===\n");
    
    // Load Shaders
    shaderProgram = rm.loadShader("mainShader", "shader/shader/SimpleVertexShader.vertexshader",
                                               "shader/shader/SimpleFragmentShader.fragmentshader");

    // Load shared textures
    rm.loadTexture("floor", "textures/tiles/tiles_0099_color_1k.jpg");
    rm.loadTexture("wall", "textures/wall/wood_0013_color_1k.jpg");
    rm.loadTexture("bones", "model/bones.png");
    rm.loadTexture("flashlight", "model/Linterna_COLOR.jpg");
    
    // Load shared floor model
    carpetModel = rm.loadModel("model/carpet.obj", true);
    
    // Load flashlight model
    flashlightModel = rm.loadModelWithMaterial("model/Linterna.obj");
    
    // Initialize room components
    room1 = std::make_unique<Room1>(app);
    room1->init();
    
    hallway = std::make_unique<Hallway>(app);
    hallway->init();
    
    room2 = std::make_unique<Room2>(app);
    room2->init();
    
    initLights();
}

void MuseumScene::initLights() {
    // Corner lights setup - 2 per room positioned like lamps
    cornerLights = {
        vec3(-12.0f, 11.5f, -12.0f),  // Main room - back left corner
        vec3(12.0f, 11.5f, -12.0f),   // Main room - back right corner
        vec3(-5.0f, 11.5f, -20.0f),   // Hallway - left side
        vec3(5.0f, 11.5f, -20.0f),    // Hallway - right side
        vec3(-10.0f, 11.5f, -31.0f),  // Second room - back left corner
        vec3(10.0f, 11.5f, -31.0f)    // Second room - back right corner
    };
}

void MuseumScene::update(float dt) {
    // Toggle flashlight with F key
    if (glfwGetKey(app.getWindow(), GLFW_KEY_F) == GLFW_PRESS) {
        if (!fKeyPressed) {
            flashlightOn = !flashlightOn;
            fKeyPressed = true;
        }
    } else {
        fKeyPressed = false;
    }

    // Update each room (allows room-specific interactions)
    room1->update(dt, app.getWindow());
    hallway->update(dt, app.getWindow());
    room2->update(dt, app.getWindow());

    // Only check collisions if not in inspection mode
    if (!hallway->isInspecting()) {
        checkCollisions();
    }
}

void MuseumScene::checkCollisions() {
    Camera& camera = app.getCamera();
    static vec3 lastValidPos = camera.position;
    
    bool collision = false;
    
    // Check collisions with each room
    if (room1->checkCollision(camera.position)) {
        collision = true;
    }
    if (hallway->checkCollision(camera.position)) {
        collision = true;
    }
    if (room2->checkCollision(camera.position)) {
        collision = true;
    }
    
    if (collision) {
        // Only reset X and Z, keep the Y height
        camera.position.x = lastValidPos.x;
        camera.position.z = lastValidPos.z;
    } else {
        lastValidPos = camera.position;
    }
}

void MuseumScene::render() {
    // Clear screen
    glClearColor(0.15f, 0.15f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Camera& camera = app.getCamera();
    mat4 View = camera.getViewMatrix();
    mat4 Projection = perspective(radians(camera.zoom), 1280.0f / 720.0f, 0.1f, 100.0f);
    
    glUseProgram(shaderProgram);
    
    GLuint ViewPosID = glGetUniformLocation(shaderProgram, "viewPos");
    GLuint LightClrID = glGetUniformLocation(shaderProgram, "lightColor");
    
    glUniform3f(LightClrID, 1.0f, 1.0f, 1.0f);
    glUniform3fv(ViewPosID, 1, &camera.position[0]);
    
    // Send all light positions to shader
    for (size_t i = 0; i < cornerLights.size(); i++) {
        std::string lightPosName = "lightPos[" + std::to_string(i) + "]";
        glUniform3fv(glGetUniformLocation(shaderProgram, lightPosName.c_str()), 1, &cornerLights[i][0]);
    }
    glUniform1i(glGetUniformLocation(shaderProgram, "numLights"), (int)cornerLights.size());
    
    // Flashlight spotlight
    glUniform3fv(glGetUniformLocation(shaderProgram, "spotLightPos"), 1, &camera.position[0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "spotLightDir"), 1, &camera.front[0]);
    glUniform1i(glGetUniformLocation(shaderProgram, "spotLightOn"), flashlightOn);

    // Check if in inspection mode
    if (hallway->isInspecting()) {
        // In inspection mode: only render the inspected object with simple lighting
        hallway->updateInspectionCamera(View, Projection);
        hallway->renderInspectionObject(shaderProgram);
    } else {
        // Normal rendering mode
        // Render shared large floor that spans entire museum
        if (carpetModel.vertexCount > 0) {
            glBindVertexArray(carpetModel.VAO);
            
            GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
            GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
            GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
            GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
            
            mat4 FloorModel = mat4(1.0f);
            FloorModel = translate(FloorModel, vec3(0.0f, 0.0f, -5.0f)); // Shift center forward to cover Room1 back area
            FloorModel = scale(FloorModel, vec3(15.0f, 1.0f, 30.0f)); // Increased Z scale to cover entire Room1
            mat4 FloorMVP = Projection * View * FloorModel;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &FloorMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &FloorModel[0][0]);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, rm.getTexture("floor"));
            glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 1);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        }
        
        // Render each room
        room1->render(View, Projection, shaderProgram, flashlightOn);
        hallway->render(View, Projection, shaderProgram);
        room2->render(View, Projection, shaderProgram);
        
        // Render flashlight
        renderFlashlightModel(View, Projection);
    }
    
    glBindVertexArray(0);
}

void MuseumScene::renderFlashlightModel(const mat4& view, const mat4& projection) {
    if (flashlightModel.vertexCount > 0 && flashlightOn) {
        glBindVertexArray(flashlightModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");
        
        // Disable depth testing so flashlight renders on top
        glDepthFunc(GL_ALWAYS);
        
        // Create a view-space model for the flashlight (weapon view model)
        mat4 FlashlightModel = mat4(1.0f);
        FlashlightModel = translate(FlashlightModel, vec3(0.35f, -0.4f, -0.8f));
        FlashlightModel = rotate(FlashlightModel, radians(-90.0f), vec3(1, 0, 0));
        FlashlightModel = rotate(FlashlightModel, radians(-15.0f), vec3(1, 0, 0));
        FlashlightModel = rotate(FlashlightModel, radians(10.0f), vec3(0, 1, 0));
        FlashlightModel = scale(FlashlightModel, vec3(0.1f, 0.1f, 0.1f));
        
        mat4 FlashlightMVP = projection * FlashlightModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &FlashlightMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &FlashlightModel[0][0]);
        
        // Render with per-triangle materials
        glUniform1i(UseTextureID, 0);
        string currentMat = "";
        for (size_t i = 0; i < flashlightModel.vertexCount; i += 3) {
            if (i < flashlightModel.materialNames.size()) {
                string matName = flashlightModel.materialNames[i];
                if (matName != currentMat) {
                    currentMat = matName;
                    if (flashlightModel.materials.find(matName) != flashlightModel.materials.end()) {
                        Material& mat = flashlightModel.materials[matName];
                        glUniform3fv(MaterialColorID, 1, &mat.Kd[0]);
                    }
                }
            }
            glDrawArrays(GL_TRIANGLES, i, 3);
        }
        
        // Restore normal depth testing
        glDepthFunc(GL_LESS);
    }
}
