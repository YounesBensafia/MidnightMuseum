#include "../include/Room1.hpp"
#include "../include/Application.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace glm;

Room1::Room1(Application& app) : app(app), rm(ResourceManager::getInstance()) {
}

Room1::~Room1() {
    // Cleanup handled by ResourceManager
}

void Room1::init() {
    printf("\n=== Loading Room 1 ===\n");
    
    // Load models specific to Room 1
    carpetModel = rm.loadModel("model/carpet.obj", true);
    showcaseModel = rm.loadFBXModel("model/glass_showcase.glb");
    fossilsModel = rm.loadFBXModel("model/fossils.glb");
    effigyModel = rm.loadFBXModel("model/effigy.glb");
}

void Room1::update(float dt, GLFWwindow* window) {
    // Room1-specific interactions go here
    // Example: Toggle room-specific lights, trigger animations, etc.
    
    // Example key handler (E key):
    // static bool eKeyPressed = false;
    // if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    //     if (!eKeyPressed) {
    //         // Do something specific to Room1
    //         printf("Room1: E key pressed!\n");
    //         eKeyPressed = true;
    //     }
    // } else {
    //     eKeyPressed = false;
    // }
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
    renderEffigy(view, projection, shaderProgram);
}

void Room1::renderFloorAndCeiling(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    
    if (carpetModel.vertexCount > 0) {
        glBindVertexArray(carpetModel.VAO);
        
        // FLOOR - Removed to prevent z-fighting with main floor in MuseumScene
        // The main floor in MuseumScene.cpp covers the entire museum
        
        // CEILING - Main room
        mat4 CeilingModel = mat4(1.0f);
        CeilingModel = translate(CeilingModel, vec3(0.0f, 12.0f, 0.0f));
        CeilingModel = rotate(CeilingModel, radians(180.0f), vec3(1, 0, 0));
        CeilingModel = scale(CeilingModel, vec3(7.0f, 1.0f, 7.0f));
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
        WallSouth = translate(WallSouth, vec3(0.0f, 4.0f, 14.0f));
        WallSouth = rotate(WallSouth, radians(-90.0f), vec3(1, 0, 0));
        WallSouth = scale(WallSouth, vec3(7.0f, 1.0f, 4.0f));
        mat4 WallSouthMVP = projection * view * WallSouth;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallSouthMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallSouth[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // WALL - EAST (right)
        mat4 WallEast = mat4(1.0f);
        WallEast = translate(WallEast, vec3(14.0f, 4.0f, 0.0f));
        WallEast = rotate(WallEast, radians(90.0f), vec3(1, 0, 0));
        WallEast = rotate(WallEast, radians(90.0f), vec3(0, 0, 1));
        WallEast = scale(WallEast, vec3(7.0f, 1.0f, 4.0f));
        mat4 WallEastMVP = projection * view * WallEast;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallEastMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallEast[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // WALL - WEST (left)
        mat4 WallWest = mat4(1.0f);
        WallWest = translate(WallWest, vec3(-14.0f, 4.0f, 0.0f));
        WallWest = rotate(WallWest, radians(90.0f), vec3(1, 0, 0));
        WallWest = rotate(WallWest, radians(-90.0f), vec3(0, 0, 1));
        WallWest = scale(WallWest, vec3(7.0f, 1.0f, 4.0f));
        mat4 WallWestMVP = projection * view * WallWest;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallWestMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallWest[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // WALL - NORTH (back) - WITH DOORWAY TO HALLWAY
        // North wall - Left side of doorway (front face)
        mat4 WallNorthLeft = mat4(1.0f);
        WallNorthLeft = translate(WallNorthLeft, vec3(-8.0f, 4.0f, -14.0f));
        WallNorthLeft = rotate(WallNorthLeft, radians(90.0f), vec3(1, 0, 0));
        WallNorthLeft = scale(WallNorthLeft, vec3(3.0f, 1.0f, 4.0f));
        mat4 WallNorthLeftMVP = projection * view * WallNorthLeft;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthLeftMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthLeft[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // North wall - Left side (back face for thickness)
        mat4 WallNorthLeftBack = mat4(1.0f);
        WallNorthLeftBack = translate(WallNorthLeftBack, vec3(-8.0f, 4.0f, -14.0f - wallThickness));
        WallNorthLeftBack = rotate(WallNorthLeftBack, radians(-90.0f), vec3(1, 0, 0));
        WallNorthLeftBack = scale(WallNorthLeftBack, vec3(3.0f, 1.0f, 4.0f));
        mat4 WallNorthLeftBackMVP = projection * view * WallNorthLeftBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthLeftBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthLeftBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // North wall - Right side of doorway (front face)
        mat4 WallNorthRight = mat4(1.0f);
        WallNorthRight = translate(WallNorthRight, vec3(8.0f, 4.0f, -14.0f));
        WallNorthRight = rotate(WallNorthRight, radians(90.0f), vec3(1, 0, 0));
        WallNorthRight = scale(WallNorthRight, vec3(3.0f, 1.0f, 4.0f));
        mat4 WallNorthRightMVP = projection * view * WallNorthRight;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthRightMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthRight[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // North wall - Right side (back face)
        mat4 WallNorthRightBack = mat4(1.0f);
        WallNorthRightBack = translate(WallNorthRightBack, vec3(8.0f, 4.0f, -14.0f - wallThickness));
        WallNorthRightBack = rotate(WallNorthRightBack, radians(-90.0f), vec3(1, 0, 0));
        WallNorthRightBack = scale(WallNorthRightBack, vec3(3.0f, 1.0f, 4.0f));
        mat4 WallNorthRightBackMVP = projection * view * WallNorthRightBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthRightBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthRightBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // North wall - Top of doorway (front face)
        mat4 WallNorthTop = mat4(1.0f);
        WallNorthTop = translate(WallNorthTop, vec3(0.0f, 6.5f, -14.0f));
        WallNorthTop = rotate(WallNorthTop, radians(90.0f), vec3(1, 0, 0));
        WallNorthTop = scale(WallNorthTop, vec3(2.0f, 1.0f, 1.5f));
        mat4 WallNorthTopMVP = projection * view * WallNorthTop;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthTopMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthTop[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // North wall - Top (back face)
        mat4 WallNorthTopBack = mat4(1.0f);
        WallNorthTopBack = translate(WallNorthTopBack, vec3(0.0f, 6.5f, -14.0f - wallThickness));
        WallNorthTopBack = rotate(WallNorthTopBack, radians(-90.0f), vec3(1, 0, 0));
        WallNorthTopBack = scale(WallNorthTopBack, vec3(2.0f, 1.0f, 1.5f));
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
            vec3(-10.0f, 1.0f, -10.0f),  // Left front
            vec3(-10.0f, 1.0f, 0.0f),    // Left middle
            vec3(-10.0f, 1.0f, 10.0f),   // Left back
            vec3(10.0f, 1.0f, -10.0f),   // Right front
            vec3(10.0f, 1.0f, 0.0f),     // Right middle
            vec3(10.0f, 1.0f, 10.0f)     // Right back
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
        FossilModel = scale(FossilModel, vec3(0.005f, 0.005f, 0.005f));
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

void Room1::renderEffigy(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (effigyModel.vertexCount > 0) {
        glBindVertexArray(effigyModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        mat4 EffigyModel = mat4(1.0f);
        EffigyModel = translate(EffigyModel, vec3(-10.5f, 4.0f, -10.0f));
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
        vec3(-10.0f, 1.0f, -10.0f),  // Left front
        vec3(-10.0f, 1.0f, 0.0f),    // Left middle
        vec3(-10.0f, 1.0f, 10.0f),   // Left back
        vec3(10.0f, 1.0f, -10.0f),   // Right front
        vec3(10.0f, 1.0f, 0.0f),     // Right middle
        vec3(10.0f, 1.0f, 10.0f)     // Right back
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
    
    // Only check walls for Room 1 (main room extends from -14 to 14 in both x and z)
    // South wall at z = 14
    if (newPos.z > 14.0f - wallMargin && newPos.z < 15.0f) return true;
    
    // East wall at x = 14
    if (newPos.x > 14.0f - wallMargin && newPos.z > -14.0f && newPos.z < 14.0f) return true;
    
    // West wall at x = -14
    if (newPos.x < -14.0f + wallMargin && newPos.z > -14.0f && newPos.z < 14.0f) return true;
    
    // North wall at z = -14 with doorway opening from x = -4 to x = 4
    if (newPos.z < -14.0f + wallMargin && newPos.z > -15.0f) {
        // If outside doorway opening, block
        if (newPos.x < -4.0f || newPos.x > 4.0f) {
            return true;
        }
    }
    
    return false;
}
