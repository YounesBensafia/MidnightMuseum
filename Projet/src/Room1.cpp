#include "../include/Room1.hpp"
#include "../include/Application.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace glm;

Room1::Room1(Application& app) : app(app), rm(ResourceManager::getInstance()) {
    // Initialize all table spotlights as off
    for (int i = 0; i < 6; i++) {
        tableSpotlights[i] = false;
    }
}

Room1::~Room1() {
    // Cleanup prompt UI
    if (promptVAO != 0) {
        glDeleteVertexArrays(1, &promptVAO);
        glDeleteBuffers(1, &promptVBO);
    }
}

void Room1::init() {
    printf("\n=== Loading Room 1 ===\n");
    
    // Load models specific to Room 1
    carpetModel = rm.loadModel("model/carpet.obj", true);
    showcaseModel = rm.loadFBXModel("model/glass_showcase.glb");
    fossilsModel = rm.loadFBXModel("model/fossils.glb");
    effigyModel = rm.loadFBXModel("model/effigy.glb");
    ropeBarrierModel = rm.loadFBXModel("model/rope_barrier.glb");
    coffinModel = rm.loadFBXModel("model/egyptian_coffin.glb");
    mourningFemaleModel = rm.loadFBXModel("model/mourning_female.glb");
    tigerPaintingModel = rm.loadFBXModel("model/tiger_painting.glb");
    flashlightModel = rm.loadFBXModel("model/flashlight.glb");
    chandelierModel = rm.loadFBXModel("model/chandlier.glb");
    skirtingBoardModel = rm.loadFBXModel("model/skirtingboard.glb");
    skullModel = rm.loadFBXModel("model/skull.glb");
    spotlightModel = rm.loadFBXModel("model/spotlight.glb");
    
    initPromptUI();
}

void Room1::initPromptUI() {
    // Create letter "E" geometry using rectangles
    // E is made of 3 horizontal bars (top, middle, bottom) and 1 vertical bar (left)
    std::vector<float> promptVertices;
    
    float width = 0.04f;  // Width of letter
    float height = 0.06f; // Height of letter
    float thickness = 0.01f; // Bar thickness
    
    // Helper lambda to add a rectangle
    auto addRect = [&](float x, float y, float w, float h) {
        // Triangle 1
        promptVertices.insert(promptVertices.end(), {x, y + h, 0.0f});
        promptVertices.insert(promptVertices.end(), {x, y, 0.0f});
        promptVertices.insert(promptVertices.end(), {x + w, y, 0.0f});
        // Triangle 2
        promptVertices.insert(promptVertices.end(), {x, y + h, 0.0f});
        promptVertices.insert(promptVertices.end(), {x + w, y, 0.0f});
        promptVertices.insert(promptVertices.end(), {x + w, y + h, 0.0f});
    };
    
    // Vertical bar (left side of E)
    addRect(-width/2, -height/2, thickness, height);
    
    // Top horizontal bar
    addRect(-width/2, height/2 - thickness, width, thickness);
    
    // Middle horizontal bar
    addRect(-width/2, -thickness/2, width * 0.8f, thickness);
    
    // Bottom horizontal bar
    addRect(-width/2, -height/2, width, thickness);
    
    
    glGenVertexArrays(1, &promptVAO);
    glGenBuffers(1, &promptVBO);
    
    glBindVertexArray(promptVAO);
    glBindBuffer(GL_ARRAY_BUFFER, promptVBO);
    glBufferData(GL_ARRAY_BUFFER, promptVertices.size() * sizeof(float), promptVertices.data(), GL_STATIC_DRAW);
    
    // Position attribute (only need position, no UVs)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Room1::update(float dt, GLFWwindow* window) {
    // Get camera position from application
    vec3 cameraPos = app.getCamera().position;
    
    // Check if player is near the effigy
    float distanceToEffigy = length(vec2(cameraPos.x - effigyPosition.x, cameraPos.z - effigyPosition.z));
    playerNearEffigy = (distanceToEffigy < interactionDistance);
    
    // Check if player is near the coffin
    float distanceToCoffin = length(vec2(cameraPos.x - coffinPosition.x, cameraPos.z - coffinPosition.z));
    playerNearCoffin = (distanceToCoffin < interactionDistance);
    
    // Check if player is near the mourning statue
    float distanceToMourning = length(vec2(cameraPos.x - mourningPosition.x, cameraPos.z - mourningPosition.z));
    playerNearMourning = (distanceToMourning < interactionDistance);
    
    // Toggle effigy animation with E key when near
    if (playerNearEffigy && !playerNearCoffin && !playerNearMourning) {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            if (!eKeyPressed) {
                effigyAnimating = !effigyAnimating;
                printf("Effigy animation: %s\n", effigyAnimating ? "ON" : "OFF");
                // Turn on light for left front table (index 0) when effigy is activated
                if (effigyAnimating) {
                    tableSpotlights[0] = true;
                    printf("Table spotlight 0 activated!\n");
                }
                eKeyPressed = true;
            }
        } else {
            eKeyPressed = false;
        }
    }
    
    // Toggle coffin animation with E key when near
    if (playerNearCoffin && !playerNearEffigy && !playerNearMourning) {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            if (!eKeyPressed) {
                coffinAnimating = !coffinAnimating;
                printf("Coffin animation: %s\n", coffinAnimating ? "ON" : "OFF");
                // Turn on light for left back table (index 2) when coffin is activated
                if (coffinAnimating) {
                    tableSpotlights[2] = true;
                    printf("Table spotlight 2 activated!\n");
                }
                eKeyPressed = true;
            }
        } else {
            eKeyPressed = false;
        }
    }
    
    // Toggle mourning statue light with E key when near
    if (playerNearMourning && !playerNearEffigy && !playerNearCoffin) {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            if (!eKeyPressed) {
                mourningActivated = !mourningActivated;
                printf("Mourning statue: %s\n", mourningActivated ? "ACTIVATED" : "OFF");
                // Turn on light for left middle table (index 1) when statue is activated
                if (mourningActivated) {
                    tableSpotlights[1] = true;
                    printf("Table spotlight 1 activated!\n");
                }
                eKeyPressed = true;
            }
        } else {
            eKeyPressed = false;
        }
    }
    
    // Update floating animation for effigy
    if (effigyAnimating) {
        effigyAnimTime += dt;
        // Smooth sine wave for floating effect
        effigyFloatOffset = sin(effigyAnimTime * 2.0f) * 0.3f; // Floats up/down 0.3 units
    } else {
        // Gradually return to original position
        effigyFloatOffset *= 0.9f;
        if (abs(effigyFloatOffset) < 0.01f) {
            effigyFloatOffset = 0.0f;
            effigyAnimTime = 0.0f;
        }
    }
    
    // Update floating animation for coffin
    if (coffinAnimating) {
        coffinAnimTime += dt;
        // Smooth sine wave for floating effect
        coffinFloatOffset = sin(coffinAnimTime * 2.0f) * 0.3f; // Floats up/down 0.3 units
    } else {
        // Gradually return to original position
        coffinFloatOffset *= 0.9f;
        if (abs(coffinFloatOffset) < 0.01f) {
            coffinFloatOffset = 0.0f;
            coffinAnimTime = 0.0f;
        }
    }
}

void Room1::render(const mat4& view, const mat4& projection, GLuint shaderProgram, bool flashlightOn) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");
    
    renderFloorAndCeiling(view, projection, shaderProgram);
    renderWalls(view, projection, shaderProgram);
    renderSkirtingBoards(view, projection, shaderProgram);
    renderShowcases(view, projection, shaderProgram);
    renderFossils(view, projection, shaderProgram);
    renderSkull(view, projection, shaderProgram);
    renderRopeBarriers(view, projection, shaderProgram);
    renderCoffin(view, projection, shaderProgram);
    renderMourningFemale(view, projection, shaderProgram);
    renderTigerPainting(view, projection, shaderProgram);
    if (flashlightOn) {
        renderFlashlight(view, projection, shaderProgram);
    }
    renderChandelier(view, projection, shaderProgram);
    renderTableSpotlights(view, projection, shaderProgram);
    renderEffigy(view, projection, shaderProgram);
    renderEKeyPrompt(projection);
}

void Room1::renderFloorAndCeiling(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");
    
    if (carpetModel.vertexCount > 0) {
        glBindVertexArray(carpetModel.VAO);
        
        // FLOOR - Removed to prevent z-fighting with main floor in MuseumScene
        // The main floor in MuseumScene.cpp covers the entire museum
        
        // CEILING - Main room (Room extends from x=-24 to 24, z=-14 to 34)
        mat4 CeilingModel = mat4(1.0f);
        CeilingModel = translate(CeilingModel, vec3(0.0f, 24.0f, 10.0f));  // Center at z=10 (midpoint of -14 and 34)
        CeilingModel = rotate(CeilingModel, radians(180.0f), vec3(1, 0, 0));
        CeilingModel = scale(CeilingModel, vec3(12.0f, 1.0f, 12.0f));
        mat4 CeilingMVP = projection * view * CeilingModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &CeilingMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &CeilingModel[0][0]);
        glBindTexture(GL_TEXTURE_2D, rm.getTexture("wall"));
        glUniform1i(UseTextureID, 1);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    }
}

void Room1::renderWalls(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    
    if (carpetModel.vertexCount > 0) {
        glBindVertexArray(carpetModel.VAO);
        glBindTexture(GL_TEXTURE_2D, rm.getTexture("wall"));
        glUniform1i(UseTextureID, 1);
        
        float wallThickness = 0.3f;
        
        // WALL - SOUTH (front)
        mat4 WallSouth = mat4(1.0f);
        WallSouth = translate(WallSouth, vec3(0.0f, 12.0f, 34.0f));
        WallSouth = rotate(WallSouth, radians(-90.0f), vec3(1, 0, 0));
        WallSouth = scale(WallSouth, vec3(12.0f, 1.0f, 12.0f));
        mat4 WallSouthMVP = projection * view * WallSouth;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallSouthMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallSouth[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // WALL - EAST (right)
        mat4 WallEast = mat4(1.0f);
        WallEast = translate(WallEast, vec3(24.0f, 12.0f, 10.0f));
        WallEast = rotate(WallEast, radians(90.0f), vec3(1, 0, 0));
        WallEast = rotate(WallEast, radians(90.0f), vec3(0, 0, 1));
        WallEast = scale(WallEast, vec3(12.0f, 1.0f, 12.0f));
        mat4 WallEastMVP = projection * view * WallEast;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallEastMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallEast[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // WALL - WEST (left)
        mat4 WallWest = mat4(1.0f);
        WallWest = translate(WallWest, vec3(-24.0f, 12.0f, 10.0f));
        WallWest = rotate(WallWest, radians(90.0f), vec3(1, 0, 0));
        WallWest = rotate(WallWest, radians(-90.0f), vec3(0, 0, 1));
        WallWest = scale(WallWest, vec3(12.0f, 1.0f, 12.0f));
        mat4 WallWestMVP = projection * view * WallWest;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallWestMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallWest[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // WALL - NORTH (back) - WITH DOORWAY TO HALLWAY
        // North wall - Left side of doorway (front face)
        mat4 WallNorthLeft = mat4(1.0f);
        WallNorthLeft = translate(WallNorthLeft, vec3(-14.0f, 12.0f, -14.0f));
        WallNorthLeft = rotate(WallNorthLeft, radians(90.0f), vec3(1, 0, 0));
        WallNorthLeft = scale(WallNorthLeft, vec3(5.0f, 1.0f, 12.0f));
        mat4 WallNorthLeftMVP = projection * view * WallNorthLeft;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthLeftMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthLeft[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // North wall - Left side (back face for thickness)
        mat4 WallNorthLeftBack = mat4(1.0f);
        WallNorthLeftBack = translate(WallNorthLeftBack, vec3(-14.0f, 12.0f, -14.0f - wallThickness));
        WallNorthLeftBack = rotate(WallNorthLeftBack, radians(-90.0f), vec3(1, 0, 0));
        WallNorthLeftBack = scale(WallNorthLeftBack, vec3(5.0f, 1.0f, 12.0f));
        mat4 WallNorthLeftBackMVP = projection * view * WallNorthLeftBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthLeftBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthLeftBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // North wall - Right side of doorway (front face)
        mat4 WallNorthRight = mat4(1.0f);
        WallNorthRight = translate(WallNorthRight, vec3(14.0f, 12.0f, -14.0f));
        WallNorthRight = rotate(WallNorthRight, radians(90.0f), vec3(1, 0, 0));
        WallNorthRight = scale(WallNorthRight, vec3(5.0f, 1.0f, 12.0f));
        mat4 WallNorthRightMVP = projection * view * WallNorthRight;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthRightMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthRight[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // North wall - Right side (back face)
        mat4 WallNorthRightBack = mat4(1.0f);
        WallNorthRightBack = translate(WallNorthRightBack, vec3(14.0f, 12.0f, -14.0f - wallThickness));
        WallNorthRightBack = rotate(WallNorthRightBack, radians(-90.0f), vec3(1, 0, 0));
        WallNorthRightBack = scale(WallNorthRightBack, vec3(5.0f, 1.0f, 12.0f));
        mat4 WallNorthRightBackMVP = projection * view * WallNorthRightBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthRightBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthRightBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // North wall - Top of doorway (front face)
        mat4 WallNorthTop = mat4(1.0f);
        WallNorthTop = translate(WallNorthTop, vec3(0.0f, 16.0f, -14.0f));
        WallNorthTop = rotate(WallNorthTop, radians(90.0f), vec3(1, 0, 0));
        WallNorthTop = scale(WallNorthTop, vec3(4.0f, 1.0f, 4.0f));
        mat4 WallNorthTopMVP = projection * view * WallNorthTop;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthTopMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthTop[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // North wall - Top (back face)
        mat4 WallNorthTopBack = mat4(1.0f);
        WallNorthTopBack = translate(WallNorthTopBack, vec3(0.0f, 16.0f, -14.0f - wallThickness));
        WallNorthTopBack = rotate(WallNorthTopBack, radians(-90.0f), vec3(1, 0, 0));
        WallNorthTopBack = scale(WallNorthTopBack, vec3(4.0f, 1.0f, 4.0f));
        mat4 WallNorthTopBackMVP = projection * view * WallNorthTopBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthTopBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthTopBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    }
}

void Room1::renderShowcases(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (showcaseModel.vertexCount > 0) {
        glBindVertexArray(showcaseModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        // Showcase positions: 3 on left side, 3 on right side
        vec3 showcasePositions[] = {
            vec3(-17.0f, 4.0f, -7.0f),   // Left front
            vec3(-17.0f, 4.0f, 10.0f),   // Left middle
            vec3(-17.0f, 4.0f, 27.0f),   // Left back
            vec3(17.0f, 4.0f, -7.0f),    // Right front
            vec3(17.0f, 4.0f, 10.0f),    // Right middle
            vec3(17.0f, 4.0f, 27.0f)     // Right back
        };
        
        for (int i = 0; i < 6; i++) {
            // Skip left middle showcase (index 1) - mourning female is there without table
            // Skip right front, middle and back showcases (indices 3, 4, 5) - clear space for painting
            if (i == 1 || i == 3 || i == 4 || i == 5) continue;
            
            mat4 Model = mat4(1.0f);
            Model = translate(Model, showcasePositions[i]);
            Model = scale(Model, vec3(1.0f, 1.0f, 1.0f));
            Model = rotate(Model, radians(90.0f), vec3(0, 1, 0));
            mat4 MVP = projection * view * Model;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);
            
            glActiveTexture(GL_TEXTURE0);
            if (showcaseModel.textureID > 0) {
                glBindTexture(GL_TEXTURE_2D, showcaseModel.textureID);
                glUniform1i(TextureID, 0);
                glUniform1i(UseTextureID, 1);
            } else {
                glUniform1i(UseTextureID, 0);
            }
            
            glDrawElements(GL_TRIANGLES, showcaseModel.indexCount, GL_UNSIGNED_INT, 0);
        }
    }
}

void Room1::renderFossils(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (fossilsModel.vertexCount > 0) {
        glBindVertexArray(fossilsModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        mat4 FossilModel = mat4(1.0f);
        FossilModel = translate(FossilModel, vec3(0.0f, 0.0f, 0.0f));
        FossilModel = rotate(FossilModel, radians(-90.0f), vec3(1, 0, 0));
        FossilModel = rotate(FossilModel, radians(45.0f), vec3(0, 0, 1));
        FossilModel = scale(FossilModel, vec3(0.015f, 0.015f, 0.015f));
        mat4 FossilMVP = projection * view * FossilModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &FossilMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &FossilModel[0][0]);
        glActiveTexture(GL_TEXTURE0);
        if (fossilsModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, fossilsModel.textureID);
        } else {
            glBindTexture(GL_TEXTURE_2D, rm.getTexture("bones"));
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, fossilsModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Room1::renderRopeBarriers(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (ropeBarrierModel.vertexCount > 0) {
        glBindVertexArray(ropeBarrierModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        // Position 4 rope barriers around the central fossils display
        // Fossils are at (0, 0, 0), place barriers in a square around them
        vec3 barrierPositions[] = {
            vec3(4.0f, 1.6f, -1.0f),   // East side
            vec3(-4.0f, 1.6f, -1.0f),  // West side
            vec3(0.0f, 1.6f, 4.0f),   // South side
            vec3(0.0f, 1.6f, -6.0f),   // North side
            // 4 barriers around skull - same X/Y, shifted in Z
            vec3(7.0f, 1.6f, 15.0f),   // East side (shifted -5 in Z)
            vec3(-7.0f, 1.6f, 15.0f),  // West side (shifted -5 in Z)
            vec3(0.0f, 1.6f, 20.0f),   // South side (shifted -5 in Z)
            vec3(0.0f, 1.6f, 9.0f)   // North side (shifted -5 in Z)
        };
        
        float barrierRotations[] = {
            90.0f,    // East
            270.0f,   // West
            180.0f,   // South
            0.0f,     // North
            90.0f,    // East of skull
            270.0f,   // West of skull
            180.0f,   // South of skull
            0.0f      // North of skull
        };
        
        for (int i = 0; i < 8; i++) {
            mat4 BarrierModel = mat4(1.0f);
            BarrierModel = translate(BarrierModel, barrierPositions[i]);
            BarrierModel = rotate(BarrierModel, radians(barrierRotations[i]), vec3(0, 1, 0));
            BarrierModel = rotate(BarrierModel, radians(-90.0f), vec3(1, 0, 0));  // Stand upright, flipped
            BarrierModel = scale(BarrierModel, vec3(0.05f, 0.05f, 0.05f));
            mat4 BarrierMVP = projection * view * BarrierModel;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &BarrierMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &BarrierModel[0][0]);
            
            if (ropeBarrierModel.textureID > 0) {
                glBindTexture(GL_TEXTURE_2D, ropeBarrierModel.textureID);
            }
            glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 1);
            glDrawElements(GL_TRIANGLES, ropeBarrierModel.indexCount, GL_UNSIGNED_INT, 0);
        }
    }
}

void Room1::renderCoffin(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (coffinModel.vertexCount > 0) {
        glBindVertexArray(coffinModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        // Place coffin on top of left back table (last showcase)
        mat4 CoffinModel = mat4(1.0f);
        CoffinModel = translate(CoffinModel, vec3(-17.0f, 2.5f + coffinFloatOffset, 26.5f));  // Apply floating animation
    
        CoffinModel = rotate(CoffinModel, radians(270.0f), vec3(0, 0, 1));  // Flip vertically
        CoffinModel = rotate(CoffinModel, radians(-10.0f), vec3(0, 1, 0));
        CoffinModel = rotate(CoffinModel, radians(-50.0f), vec3(1, 0, 0));
        CoffinModel = rotate(CoffinModel, radians(-20.0f), vec3(0, 1, 0));
        CoffinModel = rotate(CoffinModel, radians(-20.0f), vec3(0, 0, 1));
        CoffinModel = rotate(CoffinModel, radians(20.0f), vec3(0, 1, 0));

        CoffinModel = scale(CoffinModel, vec3(0.5f, 0.5f, 0.5f));  // Much bigger
        mat4 CoffinMVP = projection * view * CoffinModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &CoffinMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &CoffinModel[0][0]);
        
        if (coffinModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, coffinModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, coffinModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Room1::renderMourningFemale(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (mourningFemaleModel.vertexCount > 0) {
        glBindVertexArray(mourningFemaleModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        // Place mourning female on left middle table where coffin was
        mat4 FemaleModel = mat4(1.0f);
        FemaleModel = translate(FemaleModel, vec3(-17.0f, 1.0f, 10.0f));
        FemaleModel = rotate(FemaleModel, radians(90.0f), vec3(0, 1, 0));  // Face the fossils
        FemaleModel = rotate(FemaleModel, radians(-120.0f), vec3(1, 0, 0));  // Rotate to sit properly

        FemaleModel = scale(FemaleModel, vec3(5.0f, 5.0f, 5.0f));  // Much bigger
        mat4 FemaleMVP = projection * view * FemaleModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &FemaleMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &FemaleModel[0][0]);
        
        if (mourningFemaleModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, mourningFemaleModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, mourningFemaleModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Room1::renderEffigy(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (effigyModel.vertexCount > 0) {
        glBindVertexArray(effigyModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        mat4 EffigyModel = mat4(1.0f);
        // Apply floating animation offset
        EffigyModel = translate(EffigyModel, vec3(-17.5f, 2.5f + effigyFloatOffset, -7.0f));
        EffigyModel = rotate(EffigyModel, radians(120.0f), vec3(0, 1, 0));
        EffigyModel = rotate(EffigyModel, radians(-90.0f), vec3(1, 0, 0));
        EffigyModel = rotate(EffigyModel, radians(-90.0f), vec3(0, 1, 0));
        EffigyModel = scale(EffigyModel, vec3(1.2f, 1.2f, 1.2f));
        mat4 EffigyMVP = projection * view * EffigyModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &EffigyMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &EffigyModel[0][0]);
        glActiveTexture(GL_TEXTURE0);
        if (effigyModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, effigyModel.textureID);
        } else {
            glBindTexture(GL_TEXTURE_2D, rm.getTexture("bones"));
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, effigyModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Room1::renderEKeyPrompt(const mat4& projection) {
    if ((!playerNearEffigy && !playerNearCoffin && !playerNearMourning) || promptVAO == 0) return;
    
    // Get shader program from ResourceManager
    GLuint shaderProgram = rm.getShader("mainShader");
    glUseProgram(shaderProgram);
    
    // Disable depth test for 2D overlay
    GLint oldDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
    glDepthFunc(GL_ALWAYS);
    
    glBindVertexArray(promptVAO);
    
    // Create identity matrix for 2D rendering (no 3D transformations)
    mat4 model = mat4(1.0f);
    // Position at bottom-center of screen in NDC space
    model = translate(model, vec3(0.0f, -0.8f, 0.0f));
    model = scale(model, vec3(0.8f, 0.8f, 1.0f)); // Small square
    
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");
    
    // Use orthographic projection for 2D
    mat4 orthoProj = mat4(1.0f);
    mat4 mvp = orthoProj * model;
    
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &model[0][0]);
    glUniform1i(UseTextureID, 0); // Don't use texture, use solid color
    
    // Solid white color for the E letter
    glUniform3f(MaterialColorID, 1.0f, 1.0f, 1.0f);
    
    // Draw all triangles (4 rectangles = 24 vertices)
    glDrawArrays(GL_TRIANGLES, 0, 24);
    
    glBindVertexArray(0);
    glDepthFunc(oldDepthFunc);
}

bool Room1::checkCollision(const vec3& newPos) {
    return checkFossilCollision(newPos) || checkWallCollision(newPos) || checkTableCollision(newPos) ||
           checkRopeBarrierCollision(newPos) || checkSkullCollision(newPos) || checkMourningStatueCollision(newPos);
}

bool Room1::checkFossilCollision(const vec3& newPos) {
    vec3 fossilPos = vec3(0.0f, 0.0f, 0.0f);
    float fossilRadius = 2.5f;
    float distance = length(vec2(newPos.x - fossilPos.x, newPos.z - fossilPos.z));
    return distance < fossilRadius;
}

bool Room1::checkTableCollision(const vec3& newPos) {
    vec3 tablePositions[] = {
        vec3(-17.0f, 1.0f, -7.0f),   // Left front
        vec3(-17.0f, 1.0f, 10.0f),   // Left middle
        vec3(-17.0f, 1.0f, 27.0f),   // Left back
        vec3(17.0f, 1.0f, -7.0f),    // Right front
        vec3(17.0f, 1.0f, 10.0f),    // Right middle
        vec3(17.0f, 1.0f, 27.0f)     // Right back
    };
    
    float tableRadius = 2.0f;
    for (int i = 0; i < 6; i++) {
        // Skip removed tables: left middle (1), right front/middle/back (3, 4, 5)
        if (i == 1 || i == 3 || i == 4 || i == 5) continue;
        
        float distance = length(vec2(newPos.x - tablePositions[i].x, newPos.z - tablePositions[i].z));
        if (distance < tableRadius) {
            return true;
        }
    }
    return false;
}

bool Room1::checkRopeBarrierCollision(const vec3& newPos) {
    // 8 rope barriers total: 4 around fossils + 4 around skull
    vec3 barrierPositions[] = {
        vec3(4.0f, 1.6f, -1.0f),   // East side of fossils
        vec3(-4.0f, 1.6f, -1.0f),  // West side of fossils
        vec3(0.0f, 1.6f, 4.0f),    // South side of fossils
        vec3(0.0f, 1.6f, -6.0f),   // North side of fossils
        vec3(7.0f, 1.6f, 15.0f),   // East side of skull
        vec3(-7.0f, 1.6f, 15.0f),  // West side of skull
        vec3(0.0f, 1.6f, 20.0f),   // South side of skull
        vec3(0.0f, 1.6f, 9.0f)     // North side of skull
    };
    
    float barrierRotations[] = {
        90.0f,    // East
        270.0f,   // West
        180.0f,   // South
        0.0f,     // North
        90.0f,    // East of skull
        270.0f,   // West of skull
        180.0f,   // South of skull
        0.0f      // North of skull
    };
    
    // Check collision with each barrier (treated as rectangular area)
    for (int i = 0; i < 8; i++) {
        vec3 pos = barrierPositions[i];
        float rot = barrierRotations[i];
        
        // Rope barriers are oriented along their rotation
        // Length ~2.0, width ~0.3 for collision
        float halfLength = 1.5f;
        float halfWidth = 0.3f;
        
        // Convert rotation to radians and get direction
        float rotRad = radians(rot);
        float cosRot = cos(rotRad);
        float sinRot = sin(rotRad);
        
        // Transform player position to barrier's local space
        float dx = newPos.x - pos.x;
        float dz = newPos.z - pos.z;
        float localX = dx * cosRot + dz * sinRot;
        float localZ = -dx * sinRot + dz * cosRot;
        
        // Check if within barrier bounds
        if (abs(localX) < halfLength && abs(localZ) < halfWidth) {
            return true;
        }
    }
    
    return false;
}

bool Room1::checkSkullCollision(const vec3& newPos) {
    // Skull is at (-2.0f, -10.0f, 17.0f) with scale 10.0 (actual XZ collision center)
    vec3 skullPos = vec3(-2.0f, 0.0f, 17.0f);  // Use ground level Y for 2D collision
    float skullRadius = 3.5f;  // Larger radius for scaled skull (10x scale)
    float distance = length(vec2(newPos.x - skullPos.x, newPos.z - skullPos.z));
    return distance < skullRadius;
}

bool Room1::checkMourningStatueCollision(const vec3& newPos) {
    // Mourning statue at (-17.0f, 1.0f, 10.0f)
    vec3 mourningPos = vec3(-17.0f, 1.0f, 10.0f);
    float mourningRadius = 1.5f;  // Collision radius around statue
    float distance = length(vec2(newPos.x - mourningPos.x, newPos.z - mourningPos.z));
    return distance < mourningRadius;
}

bool Room1::checkWallCollision(const vec3& newPos) {
    float wallMargin = 0.5f;
    
    // Room 1 extends from x=-24 to x=24, and from z=-14 to z=34
    // South wall at z = 34
    if (newPos.z > 34.0f - wallMargin && newPos.z < 35.0f) return true;
    
    // East wall at x = 24
    if (newPos.x > 24.0f - wallMargin && newPos.z > -14.0f && newPos.z < 34.0f) return true;
    
    // West wall at x = -24
    if (newPos.x < -24.0f + wallMargin && newPos.z > -14.0f && newPos.z < 34.0f) return true;
    
    // North wall at z = -14 with doorway opening from x = -4 to x = 4
    if (newPos.z < -14.0f + wallMargin && newPos.z > -15.0f) {
        // If outside doorway opening, block
        if (newPos.x < -4.0f || newPos.x > 4.0f) {
            return true;
        }
    }
    
    return false;
}

void Room1::renderTigerPainting(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (tigerPaintingModel.vertexCount > 0) {
        glBindVertexArray(tigerPaintingModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        // Place tiger painting on east wall (opposite side)
        // East wall is at x=24, centered at z=18.5
        mat4 PaintingModel = mat4(1.0f);
        PaintingModel = translate(PaintingModel, vec3(21.8f, 11.0f, 11.0f));  // High on wall
        PaintingModel = rotate(PaintingModel, radians(90.0f), vec3(0, 1, 0));  // Face west
        PaintingModel = rotate(PaintingModel, radians(-65.0f), vec3(0, 0, 1));  // Face west
        PaintingModel = rotate(PaintingModel, radians(20.0f), vec3(0, 1, 0));  // Face west
        PaintingModel = rotate(PaintingModel, radians(2.0f), vec3(1, 0, 0));  // Face west

        PaintingModel = scale(PaintingModel, vec3(5.0f, 5.0f, 5.0f));  // BIG
        mat4 PaintingMVP = projection * view * PaintingModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &PaintingMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &PaintingModel[0][0]);
        
        if (tigerPaintingModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, tigerPaintingModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, tigerPaintingModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Room1::renderFlashlight(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (flashlightModel.vertexCount > 0) {
        glBindVertexArray(flashlightModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        // Get camera position and direction for flashlight
        vec3 cameraPos = app.getCamera().position;
        vec3 cameraFront = app.getCamera().front;
        vec3 cameraRight = app.getCamera().right;
        
        // Position flashlight slightly forward and to the right of camera
        mat4 FlashlightModel = mat4(1.0f);
        vec3 flashlightPos = cameraPos + cameraFront * 0.3f + cameraRight * 0.2f - vec3(0, 0.15f, 0);
        FlashlightModel = translate(FlashlightModel, flashlightPos);
        
        // Rotate to align with camera direction
        float yaw = atan2(cameraFront.x, cameraFront.z);
        FlashlightModel = rotate(FlashlightModel, yaw, vec3(0, 1, 0));
        FlashlightModel = rotate(FlashlightModel, radians(-90.0f), vec3(1, 0, 0));  // Point forward
        
        FlashlightModel = scale(FlashlightModel, vec3(0.06f, 0.06f, 0.06f));
        mat4 FlashlightMVP = projection * view * FlashlightModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &FlashlightMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &FlashlightModel[0][0]);
        
        if (flashlightModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, flashlightModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, flashlightModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Room1::renderChandelier(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (chandelierModel.vertexCount > 0) {
        glBindVertexArray(chandelierModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        // Place chandelier at center of ceiling (x=0, z=10 is room center, Y=23.5 just below ceiling)
        mat4 ChandelierModel = mat4(1.0f);
        ChandelierModel = translate(ChandelierModel, vec3(0.0f, 24.0f, 10.0f));
        ChandelierModel = rotate(ChandelierModel, radians(-90.0f), vec3(1, 0, 0));
        ChandelierModel = scale(ChandelierModel, vec3(10.0f, 10.0f, 10.0f));
        mat4 ChandelierMVP = projection * view * ChandelierModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &ChandelierMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &ChandelierModel[0][0]);
        
        if (chandelierModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, chandelierModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, chandelierModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Room1::renderSkirtingBoards(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (skirtingBoardModel.vertexCount > 0) {
        glBindVertexArray(skirtingBoardModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        // Room1 walls: South (z=34), East (x=24), West (x=-24), North (z=-14)
        // Skirting boards along the 4 walls at floor level
        
        // South wall skirting (z=34)
        mat4 SkirtSouth = mat4(1.0f);
        SkirtSouth = translate(SkirtSouth, vec3(0.0f, 0.0f, 33.5f));
        SkirtSouth = rotate(SkirtSouth, radians(-90.0f), vec3(1, 0, 0));  // Stand upright
        SkirtSouth = scale(SkirtSouth, vec3(50.0f, 1.0f, 0.5f));  // Span full width, reduced height
        mat4 SkirtSouthMVP = projection * view * SkirtSouth;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SkirtSouthMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SkirtSouth[0][0]);
        if (skirtingBoardModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, skirtingBoardModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, skirtingBoardModel.indexCount, GL_UNSIGNED_INT, 0);
        
        // East wall skirting (x=24)
        mat4 SkirtEast = mat4(1.0f);
        SkirtEast = translate(SkirtEast, vec3(23.5f, 0.0f, 10.0f));
        SkirtEast = rotate(SkirtEast, radians(90.0f), vec3(0, 1, 0));
        SkirtEast = rotate(SkirtEast, radians(-90.0f), vec3(1, 0, 0));  // Stand upright
        SkirtEast = scale(SkirtEast, vec3(50.0f, 1.0f, 0.5f));  // Span full depth, reduced height
        mat4 SkirtEastMVP = projection * view * SkirtEast;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SkirtEastMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SkirtEast[0][0]);
        if (skirtingBoardModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, skirtingBoardModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, skirtingBoardModel.indexCount, GL_UNSIGNED_INT, 0);
        
        // West wall skirting (x=-24)
        mat4 SkirtWest = mat4(1.0f);
        SkirtWest = translate(SkirtWest, vec3(-23.5f, 0.0f, 10.0f));
        SkirtWest = rotate(SkirtWest, radians(-90.0f), vec3(0, 1, 0));
        SkirtWest = rotate(SkirtWest, radians(-90.0f), vec3(1, 0, 0));  // Stand upright
        SkirtWest = scale(SkirtWest, vec3(50.0f, 1.0f, 0.5f));  // Span full depth, reduced height
        mat4 SkirtWestMVP = projection * view * SkirtWest;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SkirtWestMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SkirtWest[0][0]);
        if (skirtingBoardModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, skirtingBoardModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, skirtingBoardModel.indexCount, GL_UNSIGNED_INT, 0);
        
        // North wall skirting (z=-14) - with gap for doorway
        // Left side
        mat4 SkirtNorthLeft = mat4(1.0f);
        SkirtNorthLeft = translate(SkirtNorthLeft, vec3(-14.0f, 0.0f, -13.5f));
        SkirtNorthLeft = rotate(SkirtNorthLeft, radians(-180.0f), vec3(0, 1, 0));
        SkirtNorthLeft = rotate(SkirtNorthLeft, radians(-90.0f), vec3(1, 0, 0));  // Stand upright
        SkirtNorthLeft = scale(SkirtNorthLeft, vec3(20.0f, 1.0f, 0.5f));
        mat4 SkirtNorthLeftMVP = projection * view * SkirtNorthLeft;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SkirtNorthLeftMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SkirtNorthLeft[0][0]);
        if (skirtingBoardModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, skirtingBoardModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, skirtingBoardModel.indexCount, GL_UNSIGNED_INT, 0);
        
        // Right side
        mat4 SkirtNorthRight = mat4(1.0f);
        SkirtNorthRight = translate(SkirtNorthRight, vec3(14.0f, 0.0f, -13.5f));
        SkirtNorthRight = rotate(SkirtNorthRight, radians(-180.0f), vec3(0, 1, 0));
        SkirtNorthRight = rotate(SkirtNorthRight, radians(-90.0f), vec3(1, 0, 0));  // Stand upright
        SkirtNorthRight = scale(SkirtNorthRight, vec3(20.0f, 1.0f, 0.5f));
        mat4 SkirtNorthRightMVP = projection * view * SkirtNorthRight;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SkirtNorthRightMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SkirtNorthRight[0][0]);
        if (skirtingBoardModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, skirtingBoardModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, skirtingBoardModel.indexCount, GL_UNSIGNED_INT, 0);
        
        // ===== TOP SKIRTING BOARDS (mirrored at ceiling level Y=24) =====
        
        // South wall top skirting (z=34)
        mat4 SkirtSouthTop = mat4(1.0f);
        SkirtSouthTop = translate(SkirtSouthTop, vec3(0.0f, 24.0f, 33.5f));
        SkirtSouthTop = rotate(SkirtSouthTop, radians(180.0f), vec3(0, 1, 0));  // Flip to face inward
        SkirtSouthTop = rotate(SkirtSouthTop, radians(90.0f), vec3(1, 0, 0));  // Face downward
        SkirtSouthTop = scale(SkirtSouthTop, vec3(50.0f, 1.0f, 0.5f));
        mat4 SkirtSouthTopMVP = projection * view * SkirtSouthTop;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SkirtSouthTopMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SkirtSouthTop[0][0]);
        if (skirtingBoardModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, skirtingBoardModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, skirtingBoardModel.indexCount, GL_UNSIGNED_INT, 0);
        
        // East wall top skirting (x=24)
        mat4 SkirtEastTop = mat4(1.0f);
        SkirtEastTop = translate(SkirtEastTop, vec3(23.5f, 24.0f, 10.0f));
        SkirtEastTop = rotate(SkirtEastTop, radians(-90.0f), vec3(0, 1, 0));  // Flip to face inward
        SkirtEastTop = rotate(SkirtEastTop, radians(90.0f), vec3(1, 0, 0));  // Face downward
        SkirtEastTop = scale(SkirtEastTop, vec3(50.0f, 1.0f, 0.5f));
        mat4 SkirtEastTopMVP = projection * view * SkirtEastTop;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SkirtEastTopMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SkirtEastTop[0][0]);
        if (skirtingBoardModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, skirtingBoardModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, skirtingBoardModel.indexCount, GL_UNSIGNED_INT, 0);
        
        // West wall top skirting (x=-24)
        mat4 SkirtWestTop = mat4(1.0f);
        SkirtWestTop = translate(SkirtWestTop, vec3(-23.5f, 24.0f, 10.0f));
        SkirtWestTop = rotate(SkirtWestTop, radians(90.0f), vec3(0, 1, 0));  // Flip to face inward
        SkirtWestTop = rotate(SkirtWestTop, radians(90.0f), vec3(1, 0, 0));  // Face downward
        SkirtWestTop = scale(SkirtWestTop, vec3(50.0f, 1.0f, 0.5f));
        mat4 SkirtWestTopMVP = projection * view * SkirtWestTop;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SkirtWestTopMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SkirtWestTop[0][0]);
        if (skirtingBoardModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, skirtingBoardModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, skirtingBoardModel.indexCount, GL_UNSIGNED_INT, 0);
        
        // North wall top skirting (z=-14) - continuous across entire wall at ceiling
        mat4 SkirtNorthTop = mat4(1.0f);
        SkirtNorthTop = translate(SkirtNorthTop, vec3(0.0f, 24.0f, -13.5f));
        SkirtNorthTop = rotate(SkirtNorthTop, radians(0.0f), vec3(0, 1, 0));  // Face inward
        SkirtNorthTop = rotate(SkirtNorthTop, radians(90.0f), vec3(1, 0, 0));  // Face downward
        SkirtNorthTop = scale(SkirtNorthTop, vec3(50.0f, 1.0f, 0.5f));  // Full width
        mat4 SkirtNorthTopMVP = projection * view * SkirtNorthTop;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SkirtNorthTopMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SkirtNorthTop[0][0]);
        if (skirtingBoardModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, skirtingBoardModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, skirtingBoardModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Room1::renderSkull(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (skullModel.vertexCount > 0) {
        glBindVertexArray(skullModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        // Place skull on ground near spawn point, beside fossils (free space)
        // Camera spawns around (0, 2, 5), fossils at (0, 0, 0)
        mat4 SkullModel = mat4(1.0f);
        SkullModel = translate(SkullModel, vec3(-2.0f, -10.0f, 17.0f));  // Free space near entrance, lifted up
        SkullModel = rotate(SkullModel, radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));  // Stand upright on base
        SkullModel = scale(SkullModel, vec3(10.0f, 10.0f, 10.0f));
        mat4 SkullMVP = projection * view * SkullModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SkullMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SkullModel[0][0]);
        
        if (skullModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, skullModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, skullModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Room1::renderTableSpotlights(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (spotlightModel.vertexCount > 0) {
        glBindVertexArray(spotlightModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        // Same positions as showcases but higher up (hanging from ceiling at Y=20)
        vec3 spotlightPositions[] = {
            vec3(-17.0f, 20.0f, -7.0f),   // Left front
            vec3(-17.0f, 20.0f, 10.0f),   // Left middle
            vec3(-17.0f, 20.0f, 27.0f),   // Left back
            vec3(17.0f, 20.0f, -7.0f),    // Right front
            vec3(17.0f, 20.0f, 10.0f),    // Right middle
            vec3(17.0f, 20.0f, 27.0f)     // Right back
        };
        
        for (int i = 0; i < 6; i++) {
            // Only render if this spotlight is activated
            if (!tableSpotlights[i]) continue;
            
            mat4 SpotlightModel = mat4(1.0f);
            SpotlightModel = translate(SpotlightModel, spotlightPositions[i]);
            SpotlightModel = rotate(SpotlightModel, radians(180.0f), vec3(1, 0, 0));  // Point downward
            SpotlightModel = scale(SpotlightModel, vec3(3.0f, 3.0f, 3.0f));
            mat4 SpotlightMVP = projection * view * SpotlightModel;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SpotlightMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SpotlightModel[0][0]);
            
            if (spotlightModel.textureID > 0) {
                glBindTexture(GL_TEXTURE_2D, spotlightModel.textureID);
            }
            glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 1);
            glDrawElements(GL_TRIANGLES, spotlightModel.indexCount, GL_UNSIGNED_INT, 0);
        }
    }
}

std::vector<vec3> Room1::getActiveSpotlightPositions() const {
    std::vector<vec3> activePositions;
    
    // Same positions as in renderTableSpotlights - these become light sources
    vec3 spotlightPositions[] = {
        vec3(-17.0f, 18.0f, -7.0f),   // Left front (slightly lower for better lighting)
        vec3(-17.0f, 18.0f, 10.0f),   // Left middle
        vec3(-17.0f, 18.0f, 27.0f),   // Left back
        vec3(17.0f, 18.0f, -7.0f),    // Right front
        vec3(17.0f, 18.0f, 10.0f),    // Right middle
        vec3(17.0f, 18.0f, 27.0f)     // Right back
    };
    
    for (int i = 0; i < 6; i++) {
        if (tableSpotlights[i]) {
            activePositions.push_back(spotlightPositions[i]);
        }
    }
    
    return activePositions;
}
