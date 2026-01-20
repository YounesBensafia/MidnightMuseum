#include "../include/Room1.hpp"
#include "../include/Application.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace glm;

Room1::Room1(Application& app) : app(app), rm(ResourceManager::getInstance()) {
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
    
    // Toggle effigy animation with E key when near
    if (playerNearEffigy && !playerNearCoffin) {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            if (!eKeyPressed) {
                effigyAnimating = !effigyAnimating;
                printf("Effigy animation: %s\n", effigyAnimating ? "ON" : "OFF");
                eKeyPressed = true;
            }
        } else {
            eKeyPressed = false;
        }
    }
    
    // Toggle coffin animation with E key when near
    if (playerNearCoffin && !playerNearEffigy) {
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            if (!eKeyPressed) {
                coffinAnimating = !coffinAnimating;
                printf("Coffin animation: %s\n", coffinAnimating ? "ON" : "OFF");
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

void Room1::render(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");
    
    renderFloorAndCeiling(view, projection, shaderProgram);
    renderWalls(view, projection, shaderProgram);
    renderShowcases(view, projection, shaderProgram);
    renderFossils(view, projection, shaderProgram);
    renderRopeBarriers(view, projection, shaderProgram);
    renderCoffin(view, projection, shaderProgram);
    renderEffigy(view, projection, shaderProgram);
    renderEKeyPrompt(projection);
}

void Room1::renderFloorAndCeiling(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    
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
            vec3(-17.0f, 1.0f, -7.0f),   // Left front
            vec3(-17.0f, 1.0f, 10.0f),   // Left middle
            vec3(-17.0f, 1.0f, 27.0f),   // Left back
            vec3(17.0f, 1.0f, -7.0f),    // Right front
            vec3(17.0f, 1.0f, 10.0f),    // Right middle
            vec3(17.0f, 1.0f, 27.0f)     // Right back
        };
        
        for (int i = 0; i < 6; i++) {
            mat4 Model = mat4(1.0f);
            Model = translate(Model, showcasePositions[i]);
            Model = scale(Model, vec3(1.0f, 1.0f, 1.0f));
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
            vec3(4.0f, 1.0f, -1.0f),   // East side
            vec3(-3.0f, 1.0f, -1.0f),  // West side
            vec3(0.0f, 1.0f, 4.0f),   // South side
            vec3(0.0f, 1.0f, -6.0f)   // North side
        };
        
        float barrierRotations[] = {
            90.0f,    // East
            270.0f,   // West
            180.0f,   // South
            0.0f      // North
        };
        
        for (int i = 0; i < 4; i++) {
            mat4 BarrierModel = mat4(1.0f);
            BarrierModel = translate(BarrierModel, barrierPositions[i]);
            BarrierModel = rotate(BarrierModel, radians(barrierRotations[i]), vec3(0, 1, 0));
            BarrierModel = rotate(BarrierModel, radians(-90.0f), vec3(1, 0, 0));  // Stand upright, flipped
            BarrierModel = scale(BarrierModel, vec3(0.03f, 0.03f, 0.03f));
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
        
        // Place coffin on top of left middle table at (-17.0f, 1.0f, 10.0f)
        mat4 CoffinModel = mat4(1.0f);
        CoffinModel = translate(CoffinModel, vec3(-17.0f, 4.0f + coffinFloatOffset, 10.0f));  // Apply floating animation
    
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

void Room1::renderEffigy(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (effigyModel.vertexCount > 0) {
        glBindVertexArray(effigyModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        mat4 EffigyModel = mat4(1.0f);
        // Apply floating animation offset
        EffigyModel = translate(EffigyModel, vec3(-17.5f, 4.0f + effigyFloatOffset, -7.0f));
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
    if ((!playerNearEffigy && !playerNearCoffin) || promptVAO == 0) return;
    
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
    return checkFossilCollision(newPos) || checkWallCollision(newPos) || checkTableCollision(newPos);
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
        float distance = length(vec2(newPos.x - tablePositions[i].x, newPos.z - tablePositions[i].z));
        if (distance < tableRadius) {
            return true;
        }
    }
    return false;
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
