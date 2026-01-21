#include "../include/Hallway.hpp"
#include "../include/Application.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace glm;

Hallway::Hallway(Application& app) : app(app), rm(ResourceManager::getInstance()) {
}

Hallway::~Hallway() {
    // Cleanup handled by ResourceManager
}

void Hallway::init() {
    printf("\n=== Loading Hallway ===\n");
    
    // Load carpet model for walls/floor/ceiling
    carpetModel = rm.loadModel("model/carpet.obj", true);
    displayCabinetModel = rm.loadFBXModel("model/cabinet2.glb");
}

void Hallway::update(float dt, GLFWwindow* window) {
    // Hallway-specific interactions go here
    // Example: Flickering lights, sound triggers, etc.
}

void Hallway::render(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    renderFloorAndCeiling(view, projection, shaderProgram);
    renderWalls(view, projection, shaderProgram);
    renderDisplayCabinets(view, projection, shaderProgram);
}

void Hallway::renderFloorAndCeiling(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    
    if (carpetModel.vertexCount > 0) {
        glBindVertexArray(carpetModel.VAO);
        
        // FLOOR - Hallway (not needed as main floor covers it)
        // But keeping for clarity in case you want separate texture
        
        // CEILING - Hallway
        mat4 HallCeiling = mat4(1.0f);
        HallCeiling = translate(HallCeiling, vec3(0.0f, 12.0f, -19.0f));
        HallCeiling = rotate(HallCeiling, radians(180.0f), vec3(1, 0, 0));
        HallCeiling = scale(HallCeiling, vec3(3.0f, 1.0f, 2.5f));
        mat4 HallCeilingMVP = projection * view * HallCeiling;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &HallCeilingMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallCeiling[0][0]);
        glBindTexture(GL_TEXTURE_2D, rm.getTexture("wall"));
        glUniform1i(UseTextureID, 1);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    }
}

void Hallway::renderWalls(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    
    if (carpetModel.vertexCount > 0) {
        glBindVertexArray(carpetModel.VAO);
        glBindTexture(GL_TEXTURE_2D, rm.getTexture("wall"));
        glUniform1i(UseTextureID, 1);
        
        float wallThickness = 0.3f;
        
        // === HALLWAY WALLS (x = ±12 from z = -15 to z = -22) ===
        // Hallway East Wall (with thickness)
        mat4 HallEast = mat4(1.0f);
        HallEast = translate(HallEast, vec3(6.0f, 4.0f, -18.5f));
        HallEast = rotate(HallEast, radians(90.0f), vec3(1, 0, 0));
        HallEast = rotate(HallEast, radians(90.0f), vec3(0, 0, 1));
        HallEast = scale(HallEast, vec3(2.2f, 1.0f, 4.0f));
        mat4 HallEastMVP = projection * view * HallEast;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &HallEastMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallEast[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        mat4 HallEastBack = mat4(1.0f);
        HallEastBack = translate(HallEastBack, vec3(6.0f + wallThickness, 4.0f, -18.5f));
        HallEastBack = rotate(HallEastBack, radians(90.0f), vec3(1, 0, 0));
        HallEastBack = rotate(HallEastBack, radians(-90.0f), vec3(0, 0, 1));
        HallEastBack = scale(HallEastBack, vec3(2.2f, 1.0f, 4.0f));
        mat4 HallEastBackMVP = projection * view * HallEastBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &HallEastBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallEastBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // Hallway West Wall (with thickness)
        mat4 HallWest = mat4(1.0f);
        HallWest = translate(HallWest, vec3(-6.0f, 4.0f, -18.5f));
        HallWest = rotate(HallWest, radians(90.0f), vec3(1, 0, 0));
        HallWest = rotate(HallWest, radians(-90.0f), vec3(0, 0, 1));
        HallWest = scale(HallWest, vec3(2.2f, 1.0f, 4.0f));
        mat4 HallWestMVP = projection * view * HallWest;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &HallWestMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallWest[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        mat4 HallWestBack = mat4(1.0f);
        HallWestBack = translate(HallWestBack, vec3(-6.0f - wallThickness, 4.0f, -18.5f));
        HallWestBack = rotate(HallWestBack, radians(90.0f), vec3(1, 0, 0));
        HallWestBack = rotate(HallWestBack, radians(90.0f), vec3(0, 0, 1));
        HallWestBack = scale(HallWestBack, vec3(2.2f, 1.0f, 4.0f));
        mat4 HallWestBackMVP = projection * view * HallWestBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &HallWestBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallWestBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    }
}

bool Hallway::checkCollision(const vec3& newPos) {
    return checkWallCollision(newPos);
}

void Hallway::renderDisplayCabinets(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (displayCabinetModel.vertexCount > 0) {
        glBindVertexArray(displayCabinetModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        
        // Hallway runs from z=-15 to z=-22, place cabinets on each side
        vec3 cabinetPositions[] = {
            vec3(5.5f, 0.0f, -18.5f),   // East side (closer to wall)
            vec3(-5.5f, 0.0f, -18.5f)   // West side (closer to wall)
        };
        
        float cabinetRotations[] = {
            90.0f,   // East cabinet 
            -90.0f   // West cabinet
        };
        
        for (int i = 0; i < 2; i++) {
            mat4 CabinetModel = mat4(1.0f);
            CabinetModel = translate(CabinetModel, cabinetPositions[i]);
            CabinetModel = rotate(CabinetModel, radians(cabinetRotations[i]), vec3(0, 1, 0));  // Face into hallway
            CabinetModel = rotate(CabinetModel, radians(-90.0f), vec3(1, 0, 0));  // Stand upright
            CabinetModel = scale(CabinetModel, vec3(0.018f, 0.018f, 0.018f));  // Scaled down
            mat4 CabinetMVP = projection * view * CabinetModel;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &CabinetMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &CabinetModel[0][0]);
            
            if (displayCabinetModel.textureID > 0) {
                glBindTexture(GL_TEXTURE_2D, displayCabinetModel.textureID);
                glUniform1i(UseTextureID, 1);
            } else {
                glUniform1i(UseTextureID, 0);
            }
            glUniform1i(TextureID, 0);
            glDrawElements(GL_TRIANGLES, displayCabinetModel.indexCount, GL_UNSIGNED_INT, 0);
        }
    }
}

bool Hallway::checkWallCollision(const vec3& newPos) {
    // float wallMargin = 0.5f;
    
    // === HALLWAY SIDE WALLS (x = ±12 from z = -15 to z = -22) ===
    // if (newPos.z < -15.0f && newPos.z > -22.0f) {
    //     // East hallway wall at x = 12
    //     if (newPos.x > 12.0f - wallMargin) return true;
    //     
    //     // West hallway wall at x = -12
    //     if (newPos.x < -12.0f + wallMargin) return true;
    // }
    
    return false;
}
