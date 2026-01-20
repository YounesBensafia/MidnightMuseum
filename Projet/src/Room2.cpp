#include "../include/Room2.hpp"
#include "../include/Application.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace glm;

Room2::Room2(Application& app) : app(app), rm(ResourceManager::getInstance()) {
}

Room2::~Room2() {
    // Cleanup handled by ResourceManager
}

void Room2::init() {
    printf("\n=== Loading Room 2 ===\n");
    
    // Load models specific to Room 2
    carpetModel = rm.loadModel("model/carpet.obj", true);
    buddhaModel = rm.loadFBXModel("model/buddha_triad.glb");
}

void Room2::update(float dt, GLFWwindow* window) {
    // Room2-specific interactions go here
    // Example: Buddha glow effect, incense smoke animation, etc.
}

void Room2::render(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    renderFloorAndCeiling(view, projection, shaderProgram);
    renderWalls(view, projection, shaderProgram);
    renderBuddha(view, projection, shaderProgram);
}

void Room2::renderFloorAndCeiling(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    
    if (carpetModel.vertexCount > 0) {
        glBindVertexArray(carpetModel.VAO);
        
        // CEILING - Second Room
        mat4 Ceiling2 = mat4(1.0f);
        Ceiling2 = translate(Ceiling2, vec3(0.0f, 12.0f, -28.0f));
        Ceiling2 = rotate(Ceiling2, radians(180.0f), vec3(1, 0, 0));
        Ceiling2 = scale(Ceiling2, vec3(6.0f, 1.0f, 5.0f));
        mat4 Ceiling2MVP = projection * view * Ceiling2;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Ceiling2MVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Ceiling2[0][0]);
        glBindTexture(GL_TEXTURE_2D, rm.getTexture("wall"));
        glUniform1i(UseTextureID, 1);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    }
}

void Room2::renderWalls(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    
    if (carpetModel.vertexCount > 0) {
        glBindVertexArray(carpetModel.VAO);
        glBindTexture(GL_TEXTURE_2D, rm.getTexture("wall"));
        glUniform1i(UseTextureID, 1);
        
        float wallThickness = 0.3f;
        
        // === SECOND ROOM WALLS ===
        // Second Room South Wall - WITH DOORWAY FROM HALLWAY
        // South wall - Left side of doorway (front face)
        mat4 Room2SouthLeft = mat4(1.0f);
        Room2SouthLeft = translate(Room2SouthLeft, vec3(-7.0f, 4.0f, -23.0f));
        Room2SouthLeft = rotate(Room2SouthLeft, radians(-90.0f), vec3(1, 0, 0));
        Room2SouthLeft = scale(Room2SouthLeft, vec3(2.5f, 1.0f, 4.0f));
        mat4 Room2SouthLeftMVP = projection * view * Room2SouthLeft;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2SouthLeftMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthLeft[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // South wall - Left side (back face)
        mat4 Room2SouthLeftBack = mat4(1.0f);
        Room2SouthLeftBack = translate(Room2SouthLeftBack, vec3(-7.0f, 4.0f, -23.0f + wallThickness));
        Room2SouthLeftBack = rotate(Room2SouthLeftBack, radians(90.0f), vec3(1, 0, 0));
        Room2SouthLeftBack = scale(Room2SouthLeftBack, vec3(2.5f, 1.0f, 4.0f));
        mat4 Room2SouthLeftBackMVP = projection * view * Room2SouthLeftBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2SouthLeftBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthLeftBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // South wall - Right side of doorway (front face)
        mat4 Room2SouthRight = mat4(1.0f);
        Room2SouthRight = translate(Room2SouthRight, vec3(7.0f, 4.0f, -23.0f));
        Room2SouthRight = rotate(Room2SouthRight, radians(-90.0f), vec3(1, 0, 0));
        Room2SouthRight = scale(Room2SouthRight, vec3(2.5f, 1.0f, 4.0f));
        mat4 Room2SouthRightMVP = projection * view * Room2SouthRight;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2SouthRightMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthRight[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // South wall - Right side (back face)
        mat4 Room2SouthRightBack = mat4(1.0f);
        Room2SouthRightBack = translate(Room2SouthRightBack, vec3(7.0f, 4.0f, -23.0f + wallThickness));
        Room2SouthRightBack = rotate(Room2SouthRightBack, radians(90.0f), vec3(1, 0, 0));
        Room2SouthRightBack = scale(Room2SouthRightBack, vec3(2.5f, 1.0f, 4.0f));
        mat4 Room2SouthRightBackMVP = projection * view * Room2SouthRightBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2SouthRightBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthRightBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // South wall - Top of doorway (front face)
        mat4 Room2SouthTop = mat4(1.0f);
        Room2SouthTop = translate(Room2SouthTop, vec3(0.0f, 6.5f, -23.0f));
        Room2SouthTop = rotate(Room2SouthTop, radians(-90.0f), vec3(1, 0, 0));
        Room2SouthTop = scale(Room2SouthTop, vec3(2.0f, 1.0f, 1.5f));
        mat4 Room2SouthTopMVP = projection * view * Room2SouthTop;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2SouthTopMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthTop[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // South wall - Top (back face)
        mat4 Room2SouthTopBack = mat4(1.0f);
        Room2SouthTopBack = translate(Room2SouthTopBack, vec3(0.0f, 6.5f, -23.0f + wallThickness));
        Room2SouthTopBack = rotate(Room2SouthTopBack, radians(90.0f), vec3(1, 0, 0));
        Room2SouthTopBack = scale(Room2SouthTopBack, vec3(2.0f, 1.0f, 1.5f));
        mat4 Room2SouthTopBackMVP = projection * view * Room2SouthTopBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2SouthTopBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthTopBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // Second Room North Wall (far end) with thickness
        mat4 Room2North = mat4(1.0f);
        Room2North = translate(Room2North, vec3(0.0f, 4.0f, -38.0f));
        Room2North = rotate(Room2North, radians(90.0f), vec3(1, 0, 0));
        Room2North = scale(Room2North, vec3(6.0f, 1.0f, 4.0f));
        mat4 Room2NorthMVP = projection * view * Room2North;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2NorthMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2North[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        mat4 Room2NorthBack = mat4(1.0f);
        Room2NorthBack = translate(Room2NorthBack, vec3(0.0f, 4.0f, -38.0f - wallThickness));
        Room2NorthBack = rotate(Room2NorthBack, radians(-90.0f), vec3(1, 0, 0));
        Room2NorthBack = scale(Room2NorthBack, vec3(6.0f, 1.0f, 4.0f));
        mat4 Room2NorthBackMVP = projection * view * Room2NorthBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2NorthBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2NorthBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // Second Room East Wall (with thickness)
        mat4 Room2East = mat4(1.0f);
        Room2East = translate(Room2East, vec3(12.0f, 4.0f, -28.0f));
        Room2East = rotate(Room2East, radians(90.0f), vec3(1, 0, 0));
        Room2East = rotate(Room2East, radians(90.0f), vec3(0, 0, 1));
        Room2East = scale(Room2East, vec3(5.0f, 1.0f, 4.0f));
        mat4 Room2EastMVP = projection * view * Room2East;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2EastMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2East[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        mat4 Room2EastBack = mat4(1.0f);
        Room2EastBack = translate(Room2EastBack, vec3(12.0f + wallThickness, 4.0f, -28.0f));
        Room2EastBack = rotate(Room2EastBack, radians(90.0f), vec3(1, 0, 0));
        Room2EastBack = rotate(Room2EastBack, radians(-90.0f), vec3(0, 0, 1));
        Room2EastBack = scale(Room2EastBack, vec3(5.0f, 1.0f, 4.0f));
        mat4 Room2EastBackMVP = projection * view * Room2EastBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2EastBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2EastBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        // Second Room West Wall (with thickness)
        mat4 Room2West = mat4(1.0f);
        Room2West = translate(Room2West, vec3(-12.0f, 4.0f, -28.0f));
        Room2West = rotate(Room2West, radians(90.0f), vec3(1, 0, 0));
        Room2West = rotate(Room2West, radians(-90.0f), vec3(0, 0, 1));
        Room2West = scale(Room2West, vec3(5.0f, 1.0f, 4.0f));
        mat4 Room2WestMVP = projection * view * Room2West;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2WestMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2West[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        
        mat4 Room2WestBack = mat4(1.0f);
        Room2WestBack = translate(Room2WestBack, vec3(-12.0f - wallThickness, 4.0f, -28.0f));
        Room2WestBack = rotate(Room2WestBack, radians(90.0f), vec3(1, 0, 0));
        Room2WestBack = rotate(Room2WestBack, radians(90.0f), vec3(0, 0, 1));
        Room2WestBack = scale(Room2WestBack, vec3(5.0f, 1.0f, 4.0f));
        mat4 Room2WestBackMVP = projection * view * Room2WestBack;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2WestBackMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2WestBack[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
    }
}

void Room2::renderBuddha(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (buddhaModel.vertexCount > 0) {
        glBindVertexArray(buddhaModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");
        
        mat4 BuddhaModel = mat4(1.0f);
        BuddhaModel = translate(BuddhaModel, vec3(-11.5f, 6.0f, -30.5f));
        BuddhaModel = rotate(BuddhaModel, radians(100.0f), vec3(0, 0, 1));
        BuddhaModel = rotate(BuddhaModel, radians(57.0f), vec3(1, 0, 0));
        BuddhaModel = scale(BuddhaModel, vec3(3.0f, 3.0f, 3.0f));
        mat4 BuddhaMVP = projection * view * BuddhaModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &BuddhaMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &BuddhaModel[0][0]);
        
        if (buddhaModel.textureID > 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, buddhaModel.textureID);
            glUniform1i(UseTextureID, 0);
            glUniform3fv(MaterialColorID, 1, &buddhaModel.baseColor[0]);
        } else {
            glUniform1i(UseTextureID, 0);
            glUniform3f(MaterialColorID, 0.7f, 0.6f, 0.5f);
        }
        
        glDrawElements(GL_TRIANGLES, buddhaModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}

bool Room2::checkCollision(const vec3& newPos) {
    return checkWallCollision(newPos);
}

bool Room2::checkWallCollision(const vec3& newPos) {
    float wallMargin = 0.5f;
    
    // === SECOND ROOM WALLS ===
    if (newPos.z < -22.0f) {
        // Second room north wall at z = -33 (back wall with buddha)
        if (newPos.z < -37.0f) return true;
        
        // Second room east wall at x = 12
        if (newPos.x > 12.0f - wallMargin) return true;
        
        // Second room west wall at x = -12
        if (newPos.x < -12.0f + wallMargin) return true;
        
        // Second room south wall with doorway at z = -23 (opening from x = -3 to x = 3)
        if (newPos.z > -23.0f - wallMargin && newPos.z < -22.0f) {
            // If outside doorway opening, block
            if (newPos.x < -3.0f || newPos.x > 3.0f) {
                return true;
            }
        }
    }
    
    return false;
}
