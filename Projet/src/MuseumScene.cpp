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
    // Cleanup VAOs/VBOs if needed, or let RM handle it
}

void MuseumScene::init() {
    printf("\n=== Loading Museum Scene ===\n");
    
    // Load Shaders
    shaderProgram = rm.loadShader("mainShader", "shader/shader/SimpleVertexShader.vertexshader",
                                               "shader/shader/SimpleFragmentShader.fragmentshader");

    // Load Textures
    rm.loadTexture("floor", "textures/tiles/tiles_0099_color_1k.jpg");
    rm.loadTexture("wall", "textures/wall/wood_0013_color_1k.jpg");
    rm.loadTexture("bones", "model/bones.png");
    rm.loadTexture("flashlight", "model/Linterna_COLOR.jpg");
    
    // Load Models
    carpetModel = rm.loadModel("model/carpet.obj", true);
    showcaseModel = rm.loadFBXModel("model/glass_showcase.glb");
    fossilsModel = rm.loadFBXModel("model/fossils.glb");
    effigyModel = rm.loadFBXModel("model/effigy.glb");
    buddhaModel = rm.loadFBXModel("model/buddha_triad.glb");
    flashlightModel = rm.loadModelWithMaterial("model/Linterna.obj");
    
    initLights();
}

void MuseumScene::initLights() {
    // Corner lights setup
    cornerLights = {
        vec3(-12.0f, 11.5f, -12.0f),
        vec3(12.0f, 11.5f, -12.0f),
        vec3(-5.0f, 11.5f, -20.0f),
        vec3(5.0f, 11.5f, -20.0f),
        vec3(-10.0f, 11.5f, -31.0f),
        vec3(10.0f, 11.5f, -31.0f)
    };
}

void MuseumScene::update(float dt) {
    if (glfwGetKey(app.getWindow(), GLFW_KEY_F) == GLFW_PRESS) {
        if (!fKeyPressed) {
            flashlightOn = !flashlightOn;
            fKeyPressed = true;
        }
    } else {
        fKeyPressed = false;
    }

    checkCollisions();
}

// ... COLLISION LOGIC COPIED FROM MAIN.CPP ...
bool MuseumScene::checkFossilCollision(vec3 newPos) {
    vec3 fossilPos = vec3(0.0f, 0.0f, 0.0f);
    float fossilRadius = 2.5f;
    float distance = length(vec2(newPos.x - fossilPos.x, newPos.z - fossilPos.z));
    return distance < fossilRadius;
}

bool MuseumScene::checkWallCollision(vec3 newPos) {
    float wallMargin = 0.5f;
    if (newPos.z > 14.0f - wallMargin) return true;
    if (newPos.x > 14.0f - wallMargin) return true;
    if (newPos.x < -14.0f + wallMargin) return true;
    if (newPos.z < -14.0f + wallMargin && newPos.z > -15.0f) {
        if (newPos.x < -4.0f || newPos.x > 4.0f) return true;
    }
    if (newPos.z < -15.0f && newPos.z > -22.0f) {
        if (newPos.x > 12.0f - wallMargin) return true;
        if (newPos.x < -12.0f + wallMargin) return true;
    }
    if (newPos.z < -22.0f) {
        if (newPos.z < -37.0f) return true;
        if (newPos.x > 12.0f - wallMargin) return true;
        if (newPos.x < -12.0f + wallMargin) return true;
        if (newPos.z > -23.0f - wallMargin && newPos.z < -22.0f) {
            if (newPos.x < -3.0f || newPos.x > 3.0f) return true;
        }
    }
    return false;
}

bool MuseumScene::checkTableCollision(vec3 newPos) {
    vec3 tablePositions[] = {
        vec3(-10.0f, 1.0f, -10.0f), vec3(-10.0f, 1.0f, 0.0f), vec3(-10.0f, 1.0f, 10.0f),
        vec3(10.0f, 1.0f, -10.0f), vec3(10.0f, 1.0f, 0.0f), vec3(10.0f, 1.0f, 10.0f)
    };
    for (int i = 0; i < 6; i++) {
        if (length(vec2(newPos.x - tablePositions[i].x, newPos.z - tablePositions[i].z)) < 2.0f) return true;
    }
    return false;
}

void MuseumScene::checkCollisions() {
    Camera& camera = app.getCamera();
    // We assume the camera has already moved in App::processInput. 
    // We need to check if the NEW position is valid.
    // However, App handles movement. Ideally App should ask Scene "isValidPosition?".
    // But since we are hacking this into existing structure, we can just check current pos
    // and push back if invalid. Using a simpler approach here:
    // If we collide, we need to know where we came from. 
    // But we don't track "previous frame" position easily unless we store it here.
    static vec3 lastValidPos = camera.position;
    
    if (checkFossilCollision(camera.position) || checkWallCollision(camera.position) || checkTableCollision(camera.position)) {
        camera.position = lastValidPos;
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
    
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint ViewPosID = glGetUniformLocation(shaderProgram, "viewPos");
    GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    GLuint LightClrID = glGetUniformLocation(shaderProgram, "lightColor");
    
    glUniform3f(LightClrID, 1.0f, 1.0f, 1.0f);
    glUniform3fv(ViewPosID, 1, &camera.position[0]);
    
    for (size_t i = 0; i < cornerLights.size(); i++) {
        std::string lightPosName = "lightPos[" + std::to_string(i) + "]";
        glUniform3fv(glGetUniformLocation(shaderProgram, lightPosName.c_str()), 1, &cornerLights[i][0]);
    }
    glUniform1i(glGetUniformLocation(shaderProgram, "numLights"), (int)cornerLights.size());
    
    // Flashlight uniforms
    glUniform3fv(glGetUniformLocation(shaderProgram, "spotLightPos"), 1, &camera.position[0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "spotLightDir"), 1, &camera.front[0]);
    glUniform1i(glGetUniformLocation(shaderProgram, "spotLightOn"), flashlightOn);

    // Render Room (Walls/Floor)
    // The wall rendering logic is huge (dozens of draw calls). 
    // To save specific tokens and time, I will simplify it by rendering just the floor 
    // or assume we copy the massive block from main.cpp.
    // For this example, I'll render the floor and one wall to show structure.
    
    // FLOOR
    mat4 FloorModel = scale(mat4(1.0f), vec3(7.0f, 1.0f, 7.0f));
    mat4 FloorMVP = Projection * View * FloorModel;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &FloorMVP[0][0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &FloorModel[0][0]);
    glBindTexture(GL_TEXTURE_2D, rm.getTexture("floor"));
    glUniform1i(UseTextureID, 1);
    glBindVertexArray(carpetModel.VAO); // Assuming all planes use "carpetModel" geometry (quad)
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);

    // CEILING
    mat4 CeilingModel = translate(mat4(1.0f), vec3(0.0f, 12.0f, 0.0f));
    CeilingModel = scale(CeilingModel, vec3(7.0f, 1.0f, 7.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * CeilingModel)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &CeilingModel[0][0]);
    glBindTexture(GL_TEXTURE_2D, rm.getTexture("wall")); // Wood ceiling
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);

    // WALLS - South Wall
    mat4 WallSouth = translate(mat4(1.0f), vec3(0.0f, 4.0f, 14.0f));
    WallSouth = rotate(WallSouth, radians(-90.0f), vec3(1, 0, 0));
    WallSouth = scale(WallSouth, vec3(7.0f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * WallSouth)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallSouth[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // East Wall
    mat4 WallEast = translate(mat4(1.0f), vec3(14.0f, 4.0f, 0.0f));
    WallEast = rotate(WallEast, radians(90.0f), vec3(1, 0, 0));
    WallEast = rotate(WallEast, radians(90.0f), vec3(0, 0, 1));
    WallEast = scale(WallEast, vec3(7.0f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * WallEast)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallEast[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // West Wall
    mat4 WallWest = translate(mat4(1.0f), vec3(-14.0f, 4.0f, 0.0f));
    WallWest = rotate(WallWest, radians(90.0f), vec3(1, 0, 0));
    WallWest = rotate(WallWest, radians(-90.0f), vec3(0, 0, 1));
    WallWest = scale(WallWest, vec3(7.0f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * WallWest)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallWest[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // North Wall (with doorway - left side)
    mat4 WallNorthLeft = translate(mat4(1.0f), vec3(-8.0f, 4.0f, -14.0f));
    WallNorthLeft = rotate(WallNorthLeft, radians(90.0f), vec3(1, 0, 0));
    WallNorthLeft = scale(WallNorthLeft, vec3(3.0f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * WallNorthLeft)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthLeft[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // North Wall - right side
    mat4 WallNorthRight = translate(mat4(1.0f), vec3(8.0f, 4.0f, -14.0f));
    WallNorthRight = rotate(WallNorthRight, radians(90.0f), vec3(1, 0, 0));
    WallNorthRight = scale(WallNorthRight, vec3(3.0f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * WallNorthRight)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthRight[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // North Wall - top of doorway
    mat4 WallNorthTop = translate(mat4(1.0f), vec3(0.0f, 6.5f, -14.0f));
    WallNorthTop = rotate(WallNorthTop, radians(90.0f), vec3(1, 0, 0));
    WallNorthTop = scale(WallNorthTop, vec3(2.0f, 1.0f, 1.5f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * WallNorthTop)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthTop[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // === HALLWAY WALLS ===
    // Hallway Floor
    mat4 HallFloor = translate(mat4(1.0f), vec3(0.0f, 0.0f, -18.5f));
    HallFloor = scale(HallFloor, vec3(3.0f, 1.0f, 2.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * HallFloor)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallFloor[0][0]);
    glBindTexture(GL_TEXTURE_2D, rm.getTexture("floor"));
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // Hallway Ceiling
    mat4 HallCeil = translate(mat4(1.0f), vec3(0.0f, 12.0f, -18.5f));
    HallCeil = scale(HallCeil, vec3(3.0f, 1.0f, 2.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * HallCeil)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallCeil[0][0]);
    glBindTexture(GL_TEXTURE_2D, rm.getTexture("wall"));
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // Hallway East Wall
    mat4 HallEast = translate(mat4(1.0f), vec3(6.0f, 4.0f, -18.5f));
    HallEast = rotate(HallEast, radians(90.0f), vec3(1, 0, 0));
    HallEast = rotate(HallEast, radians(90.0f), vec3(0, 0, 1));
    HallEast = scale(HallEast, vec3(2.2f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * HallEast)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallEast[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // Hallway West Wall
    mat4 HallWest = translate(mat4(1.0f), vec3(-6.0f, 4.0f, -18.5f));
    HallWest = rotate(HallWest, radians(90.0f), vec3(1, 0, 0));
    HallWest = rotate(HallWest, radians(-90.0f), vec3(0, 0, 1));
    HallWest = scale(HallWest, vec3(2.2f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * HallWest)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallWest[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // === SECOND ROOM WALLS ===
    // Second Room Floor
    mat4 Room2Floor = translate(mat4(1.0f), vec3(0.0f, 0.0f, -30.5f));
    Room2Floor = scale(Room2Floor, vec3(6.0f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * Room2Floor)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2Floor[0][0]);
    glBindTexture(GL_TEXTURE_2D, rm.getTexture("floor"));
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // Second Room Ceiling
    mat4 Room2Ceil = translate(mat4(1.0f), vec3(0.0f, 12.0f, -30.5f));
    Room2Ceil = scale(Room2Ceil, vec3(6.0f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * Room2Ceil)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2Ceil[0][0]);
    glBindTexture(GL_TEXTURE_2D, rm.getTexture("wall"));
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // Second Room North Wall (back)
    mat4 Room2North = translate(mat4(1.0f), vec3(0.0f, 4.0f, -38.0f));
    Room2North = rotate(Room2North, radians(90.0f), vec3(1, 0, 0));
    Room2North = scale(Room2North, vec3(6.0f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * Room2North)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2North[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // Second Room East Wall
    mat4 Room2East = translate(mat4(1.0f), vec3(12.0f, 4.0f, -28.0f));
    Room2East = rotate(Room2East, radians(90.0f), vec3(1, 0, 0));
    Room2East = rotate(Room2East, radians(90.0f), vec3(0, 0, 1));
    Room2East = scale(Room2East, vec3(5.0f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * Room2East)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2East[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // Second Room West Wall
    mat4 Room2West = translate(mat4(1.0f), vec3(-12.0f, 4.0f, -28.0f));
    Room2West = rotate(Room2West, radians(90.0f), vec3(1, 0, 0));
    Room2West = rotate(Room2West, radians(-90.0f), vec3(0, 0, 1));
    Room2West = scale(Room2West, vec3(5.0f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * Room2West)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2West[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // Second Room South Wall - left side of doorway
    mat4 Room2SouthLeft = translate(mat4(1.0f), vec3(-7.0f, 4.0f, -23.0f));
    Room2SouthLeft = rotate(Room2SouthLeft, radians(-90.0f), vec3(1, 0, 0));
    Room2SouthLeft = scale(Room2SouthLeft, vec3(2.5f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * Room2SouthLeft)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthLeft[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // Second Room South Wall - right side
    mat4 Room2SouthRight = translate(mat4(1.0f), vec3(7.0f, 4.0f, -23.0f));
    Room2SouthRight = rotate(Room2SouthRight, radians(-90.0f), vec3(1, 0, 0));
    Room2SouthRight = scale(Room2SouthRight, vec3(2.5f, 1.0f, 4.0f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * Room2SouthRight)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthRight[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // Second Room South Wall - top of doorway
    mat4 Room2SouthTop = translate(mat4(1.0f), vec3(0.0f, 6.5f, -23.0f));
    Room2SouthTop = rotate(Room2SouthTop, radians(-90.0f), vec3(1, 0, 0));
    Room2SouthTop = scale(Room2SouthTop, vec3(2.0f, 1.0f, 1.5f));
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * Room2SouthTop)[0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthTop[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    
    // Render Glass Showcases (6 tables)
    if (showcaseModel.vertexCount > 0) {
        glBindVertexArray(showcaseModel.VAO);
        vec3 showcasePositions[] = {
            vec3(-10.0f, 1.0f, -10.0f), vec3(-10.0f, 1.0f, 0.0f), vec3(-10.0f, 1.0f, 10.0f),
            vec3(10.0f, 1.0f, -10.0f), vec3(10.0f, 1.0f, 0.0f), vec3(10.0f, 1.0f, 10.0f)
        };
        
        for (int t = 0; t < 6; t++) {
            mat4 M = translate(mat4(1.0f), showcasePositions[t]);
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * M)[0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &M[0][0]);
            
            glActiveTexture(GL_TEXTURE0);
            if (showcaseModel.textureID > 0) {
                glBindTexture(GL_TEXTURE_2D, showcaseModel.textureID);
                glUniform1i(UseTextureID, 1);
            } else {
                glUniform1i(UseTextureID, 0);
            }
            glDrawElements(GL_TRIANGLES, showcaseModel.indexCount, GL_UNSIGNED_INT, 0);
        }
    }
    
    // Render Effigy on showcase
    if (effigyModel.vertexCount > 0) {
        glBindVertexArray(effigyModel.VAO);
        mat4 M = translate(mat4(1.0f), vec3(-10.5f, 4.0f, -10.0f));
        M = rotate(M, radians(120.0f), vec3(0, 1, 0));
        M = rotate(M, radians(-90.0f), vec3(1, 0, 0));
        M = rotate(M, radians(-90.0f), vec3(0, 1, 0));
        M = scale(M, vec3(1.2f));
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * M)[0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &M[0][0]);
        
        glActiveTexture(GL_TEXTURE0);
        if(effigyModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, effigyModel.textureID);
            glUniform1i(UseTextureID, 1);
        }
        glDrawElements(GL_TRIANGLES, effigyModel.indexCount, GL_UNSIGNED_INT, 0);
    }
    
    // Render Buddha
    if (buddhaModel.vertexCount > 0) {
        glBindVertexArray(buddhaModel.VAO);
        mat4 M = translate(mat4(1.0f), vec3(-11.5f, 6.0f, -30.5f));
        M = rotate(M, radians(100.0f), vec3(0, 0, 1));
        M = rotate(M, radians(57.0f), vec3(1, 0, 0));
        M = scale(M, vec3(3.0f));
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * M)[0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &M[0][0]);
        
        glUniform1i(UseTextureID, 0);
        if (buddhaModel.textureID > 0) {
            glUniform3fv(glGetUniformLocation(shaderProgram, "materialColor"), 1, &buddhaModel.baseColor[0]);
        } else {
            glUniform3f(glGetUniformLocation(shaderProgram, "materialColor"), 0.7f, 0.6f, 0.5f);
        }
        glDrawElements(GL_TRIANGLES, buddhaModel.indexCount, GL_UNSIGNED_INT, 0);
    }
    
    // Render Flashlight (if on)
    if (flashlightOn && flashlightModel.vertexCount > 0) {
        glBindVertexArray(flashlightModel.VAO);
        glDepthFunc(GL_ALWAYS);
        
        mat4 M = translate(mat4(1.0f), vec3(0.35f, -0.4f, -0.8f));
        M = rotate(M, radians(-90.0f), vec3(1, 0, 0));
        M = rotate(M, radians(-15.0f), vec3(1, 0, 0));
        M = rotate(M, radians(10.0f), vec3(0, 1, 0));
        M = scale(M, vec3(0.1f));
        
        mat4 FlashlightMVP = Projection * M;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &FlashlightMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &M[0][0]);
        
        glUniform1i(UseTextureID, 0);
        GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");
        
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
        
        glDepthFunc(GL_LESS);
    }
    
    // === RENDER FOSSILS MODEL ===
    if (fossilsModel.vertexCount > 0) {
        glBindVertexArray(fossilsModel.VAO);
        mat4 M = translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
        M = rotate(M, radians(-90.0f), vec3(1, 0, 0));
        M = rotate(M, radians(45.0f), vec3(0, 0, 1));
        M = scale(M, vec3(0.005f));
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &value_ptr(Projection * View * M)[0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &M[0][0]);
        
        glActiveTexture(GL_TEXTURE0);
        if(fossilsModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, fossilsModel.textureID);
            glUniform1i(UseTextureID, 1);
        } else {
            glBindTexture(GL_TEXTURE_2D, rm.getTexture("bones"));
            glUniform1i(UseTextureID, 1);
        }
        glUniform1i(TextureID, 0);
        
        glDrawElements(GL_TRIANGLES, fossilsModel.indexCount, GL_UNSIGNED_INT, 0);
    }
    
    glBindVertexArray(0);
}
