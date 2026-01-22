#include "../include/Room2.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>

using namespace glm;
// Render the Status statue (modelStatus)
void Room2::renderStatus(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram) {
    if (modelStatus.indexCount > 0) {
        glBindVertexArray(modelStatus.VAO);

        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");

        glm::mat4 modelStatusMatrix = glm::mat4(1.0f);
        // Example transform: adjust as needed for your scene
        modelStatusMatrix = glm::translate(modelStatusMatrix, glm::vec3(-6.0f, 0.0f, -37.0f));
        modelStatusMatrix = glm::rotate(modelStatusMatrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
        modelStatusMatrix = glm::rotate(modelStatusMatrix, glm::radians(0.0f), glm::vec3(0, 0, 1));
        modelStatusMatrix = glm::rotate(modelStatusMatrix, glm::radians(0.0f), glm::vec3(1, 0, 0));
        modelStatusMatrix = glm::scale(modelStatusMatrix, glm::vec3(6.0f, 6.0f, 6.0f));
        glm::mat4 StatusMVP = projection * view * modelStatusMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &StatusMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &modelStatusMatrix[0][0]);

        glActiveTexture(GL_TEXTURE0);
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        if (modelStatus.textureID > 0) {
            printf("[Status] Binding texture ID: %u\n", modelStatus.textureID);
            glBindTexture(GL_TEXTURE_2D, modelStatus.textureID);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 1);
            glUniform3fv(MaterialColorID, 1, &modelStatus.baseColor[0]);
        } else {
            printf("[Status] WARNING: No texture found, using default color.\n");
            glBindTexture(GL_TEXTURE_2D, 0);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 0);
            glUniform3f(MaterialColorID, 0.7f, 0.6f, 0.5f); // Default color
        }

        glDrawElements(GL_TRIANGLES, modelStatus.indexCount, GL_UNSIGNED_INT, 0);
    }
}
// Render the Min Egyptian statue (MinEgy)
void Room2::renderMinEgy(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram) {
    if (modelMinEgy.indexCount > 0) {
        glBindVertexArray(modelMinEgy.VAO);

        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");

        glm::mat4 MinEgyModelMatrix = glm::mat4(1.0f);
        // Example transform: adjust as needed for your scene
        MinEgyModelMatrix = glm::translate(MinEgyModelMatrix, glm::vec3(6.0f, 2.0f, -34.0f));
        MinEgyModelMatrix = glm::rotate(MinEgyModelMatrix, glm::radians(0.0f), glm::vec3(0, 1, 0));
        MinEgyModelMatrix = glm::rotate(MinEgyModelMatrix, glm::radians(0.0f), glm::vec3(0, 0, 1));
        MinEgyModelMatrix = glm::rotate(MinEgyModelMatrix, glm::radians(-90.0f), glm::vec3(1, 0, 0));
        MinEgyModelMatrix = glm::scale(MinEgyModelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
        glm::mat4 MinEgyMVP = projection * view * MinEgyModelMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MinEgyMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &MinEgyModelMatrix[0][0]);

        glActiveTexture(GL_TEXTURE0);
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        if (modelMinEgy.textureID > 0) {
            printf("[MinEgy] Binding texture ID: %u\n", modelMinEgy.textureID);
            glBindTexture(GL_TEXTURE_2D, modelMinEgy.textureID);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 1);
            glUniform3fv(MaterialColorID, 1, &modelMinEgy.baseColor[0]);
        } else {
            printf("[MinEgy] WARNING: No texture found, using default color.\n");
            glBindTexture(GL_TEXTURE_2D, 0);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 0);
            glUniform3f(MaterialColorID, 0.7f, 0.6f, 0.5f); // Default color
        }

        glDrawElements(GL_TRIANGLES, modelMinEgy.indexCount, GL_UNSIGNED_INT, 0);
    }
}
// Render the Pyramid (modelPyramid)
void Room2::renderPyramid(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram) {
    if (modelPyramid.indexCount > 0) {
        glBindVertexArray(modelPyramid.VAO);

        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");

        glm::mat4 PyramidModelMatrix = glm::mat4(1.0f);
        // Example transform: adjust as needed for your scene
        PyramidModelMatrix = glm::translate(PyramidModelMatrix, glm::vec3(0.0f, 0.0f, -30.0f));
        PyramidModelMatrix = glm::rotate(PyramidModelMatrix, glm::radians(-90.0f), glm::vec3(1, 0, 0));
        PyramidModelMatrix = glm::scale(PyramidModelMatrix, glm::vec3(4.0f, 4.0f, 4.0f));
        glm::mat4 PyramidMVP = projection * view * PyramidModelMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &PyramidMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &PyramidModelMatrix[0][0]);

        glActiveTexture(GL_TEXTURE0);
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
        if (modelPyramid.textureID > 0) {
            printf("[Pyramid] Binding texture ID: %u\n", modelPyramid.textureID);
            glBindTexture(GL_TEXTURE_2D, modelPyramid.textureID);
            glUniform1i(UseTextureID, 1);
            glUniform3fv(MaterialColorID, 1, &modelPyramid.baseColor[0]);
        } else {
            printf("[Pyramid] WARNING: No texture found, using default color.\n");
            glBindTexture(GL_TEXTURE_2D, 0);
            glUniform1i(UseTextureID, 0);
            // Sand/gold color
            glUniform3f(MaterialColorID, 0.9f, 0.8f, 0.5f);
        }

        glDrawElements(GL_TRIANGLES, modelPyramid.indexCount, GL_UNSIGNED_INT, 0);
    }
}
// Render the Sphinx (modelSphinx)
void Room2::renderSphinx(const glm::mat4& view, const glm::mat4& projection, GLuint shaderProgram) {
    if (modelSphinx.indexCount > 0) {
        glBindVertexArray(modelSphinx.VAO);

        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");

        glm::mat4 SphinxModelMatrix = glm::mat4(1.0f);
        SphinxModelMatrix = glm::translate(SphinxModelMatrix, glm::vec3(9.0f, 0.0f, -30.0f));
        SphinxModelMatrix = glm::rotate(SphinxModelMatrix, glm::radians(90.0f), glm::vec3(0, 1, 0));
        SphinxModelMatrix = glm::rotate(SphinxModelMatrix, glm::radians(180.0f), glm::vec3(0, 0, 1));
        SphinxModelMatrix = glm::rotate(SphinxModelMatrix, glm::radians(90.0f), glm::vec3(1, 0, 0));
        SphinxModelMatrix = glm::scale(SphinxModelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
        glm::mat4 SphinxMVP = projection * view * SphinxModelMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SphinxMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SphinxModelMatrix[0][0]);

        glActiveTexture(GL_TEXTURE0);
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
        if (modelSphinx.textureID > 0) {
            printf("[Sphinx] Binding texture ID: %u\n", modelSphinx.textureID);
            glBindTexture(GL_TEXTURE_2D, modelSphinx.textureID);
            glUniform1i(UseTextureID, 1);
            glUniform3fv(MaterialColorID, 1, &modelSphinx.baseColor[0]);
        } else {
            printf("[Sphinx] WARNING: No texture found, using default color.\n");
            glBindTexture(GL_TEXTURE_2D, 0);
            glUniform1i(UseTextureID, 0);
            glUniform3f(MaterialColorID, 0.8f, 0.7f, 0.5f); // Default limestone color
        }

        glDrawElements(GL_TRIANGLES, modelSphinx.indexCount, GL_UNSIGNED_INT, 0);
    }
}
// Ensure all required headers are included for OpenGL and GLM symbols
#include "../include/Room2.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <iostream>

using namespace glm;

void Room2::renderTorso(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (torsoModel.indexCount > 0) {
        printf("[Torso] VAO=%u, indexCount=%zu, textureID=%u\n", torsoModel.VAO, torsoModel.indexCount, torsoModel.textureID);
        glBindVertexArray(torsoModel.VAO);

        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");

        mat4 TorsoModelMatrix = mat4(1.0f);
        TorsoModelMatrix = translate(TorsoModelMatrix, vec3(4.0f, 0.0f, -35.5f));
        TorsoModelMatrix = rotate(TorsoModelMatrix, radians(0.0f), vec3(0, 1, 0));
        TorsoModelMatrix = rotate(TorsoModelMatrix, radians(0.0f), vec3(0, 0, 1));
        TorsoModelMatrix = rotate(TorsoModelMatrix, radians(-90.0f), vec3(1, 0, 0));
        TorsoModelMatrix = scale(TorsoModelMatrix, vec3(1.8f, 1.8f, 1.8f));
        mat4 TorsoMVP = projection * view * TorsoModelMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &TorsoMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &TorsoModelMatrix[0][0]);

        glActiveTexture(GL_TEXTURE0);
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        if (torsoModel.textureID > 0) {
            printf("[Torso] Binding texture ID: %u\n", torsoModel.textureID);
            glBindTexture(GL_TEXTURE_2D, torsoModel.textureID);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 1);
            glUniform3fv(MaterialColorID, 1, &torsoModel.baseColor[0]);
        } else {
            printf("[Torso] WARNING: No texture found, using default color.\n");
            glBindTexture(GL_TEXTURE_2D, 0);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 0);
            glUniform3f(MaterialColorID, 0.7f, 0.6f, 0.5f); // Default color
        }

        glDrawElements(GL_TRIANGLES, torsoModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}
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
    TutaModel = rm.loadFBXModel("model/the_bust_of_pharaoh_tutankhamun.glb");
        monsterModel = rm.loadFBXModel("model/monster__sekireinel__totem_kings_raid.glb");
        // Load the Amenhotep / Seki statue
        sekiModel = rm.loadFBXModel("model/seated_statue_of_amenhotep_iii.glb");
                torsoModel = rm.loadFBXModel("model/torso_de_tutmosis_iii.glb");
                modelStatus = rm.loadFBXModel("model/egyptian_miniature.glb");
                modelPyramid = rm.loadFBXModel("model/pyramid_figure_decoration.glb");
                modelSphinx = rm.loadFBXModel("model/sandstone_sphinx_statue.glb");
                

                // Debug: report loaded texture IDs (carpet is a Model without textureID)
                printf("Loaded models textures: buddha=%u, tuta=%u, monster=%u, seki=%u\n",
                    buddhaModel.textureID, TutaModel.textureID, monsterModel.textureID, sekiModel.textureID);
    
    // Initialize Pharaonic exhibits from data
    initializeExhibits();
}

void Room2::update(float dt, GLFWwindow* window) {
    // Room2-specific interactions go here
    // Example: Buddha glow effect, incense smoke animation, etc.
}

void Room2::render(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    // Ensure the fragment shader sampler uses texture unit 0
    GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
    if (TextureID != (GLuint)-1) {
        glUniform1i(TextureID, 0);
    }
    renderFloorAndCeiling(view, projection, shaderProgram);
    renderWalls(view, projection, shaderProgram);
    renderBuddha(view, projection, shaderProgram);
    renderTuta(view, projection, shaderProgram);
    renderMonster(view, projection, shaderProgram);
    renderTorso(view, projection, shaderProgram);
    renderStatus(view, projection, shaderProgram);
    renderSeki(view, projection, shaderProgram);
    renderExhibits(view, projection, shaderProgram);
    renderPyramid(view, projection, shaderProgram);
    renderSphinx(view, projection, shaderProgram);

}

void Room2::renderFloorAndCeiling(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
    
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
        // Ensure texture unit 0 is active and sampler is bound to it
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rm.getTexture("wall"));
        if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
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
        BuddhaModel = translate(BuddhaModel, vec3(-10.5f, 6.0f, -30.5f));
        // No rotation: keep Buddha upright (X=0, Y=0, Z=0)
        BuddhaModel = rotate(BuddhaModel, radians(0.0f), vec3(0, 1, 0));
        BuddhaModel = rotate(BuddhaModel, radians(90.0f), vec3(0, 0, 1));
        BuddhaModel = rotate(BuddhaModel, radians(60.0f), vec3(1, 0, 0));
        BuddhaModel = scale(BuddhaModel, vec3(3.0f, 3.0f, 3.0f));
        mat4 BuddhaMVP = projection * view * BuddhaModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &BuddhaMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &BuddhaModel[0][0]);
        
        glActiveTexture(GL_TEXTURE0);
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        if (buddhaModel.textureID > 0) {
            printf("[Buddha] Binding texture ID: %u\n", buddhaModel.textureID);
            glBindTexture(GL_TEXTURE_2D, buddhaModel.textureID);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 1);
            glUniform3fv(MaterialColorID, 1, &buddhaModel.baseColor[0]);
        } else {
            printf("[Buddha] WARNING: No texture found, using default color.\n");
            glBindTexture(GL_TEXTURE_2D, 0);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 0);
            glUniform3f(MaterialColorID, 0.7f, 0.6f, 0.5f);
        }
        
        glDrawElements(GL_TRIANGLES, buddhaModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Room2::renderTuta(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (TutaModel.indexCount > 0) {
        printf("[DEBUG] Rendering Tuta: VAO=%d, indexCount=%d, textureID=%d\n", TutaModel.VAO, TutaModel.indexCount, TutaModel.textureID);
        // Move Tuta to the center of the room and raise it for visibility
        glBindVertexArray(TutaModel.VAO);

        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");

        mat4 TutaModelMatrix = mat4(1.0f);
        // Center of the room, raised up for visibility
        TutaModelMatrix = translate(TutaModelMatrix, vec3(0.0f, 0.0f, -28.0f));
        TutaModelMatrix = rotate(TutaModelMatrix, radians(-100.0f), vec3(0, 0, 1));
        TutaModelMatrix = rotate(TutaModelMatrix, radians(57.0f), vec3(1, 0, 0));
        TutaModelMatrix = scale(TutaModelMatrix, vec3(8.0f, 8.0f, 8.0f));
        mat4 TutaMVP = projection * view * TutaModelMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &TutaMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &TutaModelMatrix[0][0]);

        glActiveTexture(GL_TEXTURE0);
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        if (TutaModel.textureID > 0) {
            printf("[Tuta] Binding texture ID: %u\n", TutaModel.textureID);
            glBindTexture(GL_TEXTURE_2D, TutaModel.textureID);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 1);
            glUniform3fv(MaterialColorID, 1, &TutaModel.baseColor[0]);
        } else {
            printf("[Tuta] WARNING: No texture found, using default color.\n");
            glBindTexture(GL_TEXTURE_2D, 0);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 0);
            glUniform3f(MaterialColorID, 0.8f, 0.7f, 0.5f);  // Golden color for Tutankhamun
        }

        printf("[DEBUG] Drawing Tuta at position (0, 5, -28) with scale 3.0\n");
        glDrawElements(GL_TRIANGLES, TutaModel.indexCount, GL_UNSIGNED_INT, 0);
    } else {
        printf("[ERROR] Tuta model not loaded: indexCount=%d\n", TutaModel.indexCount);
    }
}

void Room2::renderMonster(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (monsterModel.indexCount > 0) {
        glBindVertexArray(monsterModel.VAO);
        
        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");
        
        mat4 MonsterModelMatrix = mat4(1.0f);
        MonsterModelMatrix = translate(MonsterModelMatrix, vec3(0.0f, 0.0f, -35.5f));  // In the floor, further back
        MonsterModelMatrix = rotate(MonsterModelMatrix, radians(0.0f), vec3(0, 1, 0));  // Y - Turning
        MonsterModelMatrix = rotate(MonsterModelMatrix, radians(0.0f), vec3(0, 0, 1)); // Z - Spinning
        MonsterModelMatrix = rotate(MonsterModelMatrix, radians(-90.0f), vec3(1, 0, 0));  // X - Standing upright
        MonsterModelMatrix = scale(MonsterModelMatrix, vec3(0.04f, 0.04f, 0.04f));  // Slightly smaller
        mat4 MonsterMVP = projection * view * MonsterModelMatrix;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MonsterMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &MonsterModelMatrix[0][0]);
        
        glActiveTexture(GL_TEXTURE0);
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        if (monsterModel.textureID > 0) {
            printf("[Monster] Binding texture ID: %u\n", monsterModel.textureID);
            glBindTexture(GL_TEXTURE_2D, monsterModel.textureID);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 1);
            glUniform3fv(MaterialColorID, 1, &monsterModel.baseColor[0]);
        } else {
            printf("[Monster] WARNING: No texture found, using default color.\n");
            glBindTexture(GL_TEXTURE_2D, 0);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 0);
            glUniform3f(MaterialColorID, 0.5f, 0.3f, 0.2f);  // Dark mystical color
        }
        
        glDrawElements(GL_TRIANGLES, monsterModel.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Room2::renderSeki(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    if (sekiModel.indexCount > 0) {
        glBindVertexArray(sekiModel.VAO);

        GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
        GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
        GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
        GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");

        mat4 SekiModelMatrix = mat4(1.0f);
        SekiModelMatrix = translate(SekiModelMatrix, vec3(-3.5f, 0.0f, -33.5f)); // Position: left-back, slightly in floor
        SekiModelMatrix = rotate(SekiModelMatrix, radians(0.0f), vec3(0, 1, 0));
        SekiModelMatrix = rotate(SekiModelMatrix, radians(0.0f), vec3(0, 0, 1));
        SekiModelMatrix = rotate(SekiModelMatrix, radians(0.0f), vec3(1, 0, 0));
        SekiModelMatrix = scale(SekiModelMatrix, vec3(0.1f, 0.1f, 0.1f));
        mat4 SekiMVP = projection * view * SekiModelMatrix;

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &SekiMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &SekiModelMatrix[0][0]);

        glActiveTexture(GL_TEXTURE0);
        GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
        if (sekiModel.textureID > 0) {
            printf("[Seki] Binding texture ID: %u\n", sekiModel.textureID);
            glBindTexture(GL_TEXTURE_2D, sekiModel.textureID);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 1);
            glUniform3fv(MaterialColorID, 1, &sekiModel.baseColor[0]);
        } else {
            printf("[Seki] WARNING: No texture found, using default color.\n");
            glBindTexture(GL_TEXTURE_2D, 0);
            if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 0);
            glUniform3f(MaterialColorID, 0.7f, 0.6f, 0.5f);
        }

        glDrawElements(GL_TRIANGLES, sekiModel.indexCount, GL_UNSIGNED_INT, 0);
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
        // === PYRAMID COLLISION ===
        // Pyramid is at (0, 0, -30), scaled by 4.0, so its base is roughly 4x4 units (assuming model base is 1x1)
        // Adjust these bounds as needed for your model's real size
        float pyramidX = 0.0f;
        float pyramidY = 0.0f;
        float pyramidZ = -30.0f;
        float pyramidBase = 4.0f * 1.0f; // 4.0 is the scale
        float pyramidHalf = pyramidBase * 0.5f + wallMargin;
        float pyramidHeight = 4.0f + wallMargin; // 4.0 is the scale

        if (newPos.x > (pyramidX - pyramidHalf) && newPos.x < (pyramidX + pyramidHalf) &&
            newPos.z > (pyramidZ - pyramidHalf) && newPos.z < (pyramidZ + pyramidHalf) &&
            newPos.y > pyramidY && newPos.y < (pyramidY + pyramidHeight)) {
            // Inside the pyramid's bounding box
            return true;
        }
    }
    
    return false;
}

// ===== PHARAONIC GALLERY DATA SYSTEM =====

std::vector<ExhibitData> Room2::loadGalleryData() {
    std::vector<ExhibitData> data;
    
    // NOTE: Tutankhamun Bust is rendered directly via renderTuta() - not in exhibits vector
    
    // === ARTIFACT 2: Rosetta Stone Replica ===
    // Center back wall - translation key (most famous artifact)
    data.push_back({
        "Rosetta Stone Replica",
        "model/rosetta_stone.glb",
        vec3(0.0f, 4.0f, -36.0f),        // Position: Center, near back wall
        vec3(0.0f, 0.0f, 0.0f),          // Rotation: Facing forward
        vec3(2.0f, 2.5f, 0.8f),          // Scale: Tall stele (114cm original)
        "Granodiorite stele with decree in hieroglyphic, Demotic, and Ancient Greek"
    });
    
    // === ARTIFACT 3: Sphinx Bust ===
    // Right side, symmetric to canopic jars - guardian statue
    data.push_back({
        "Sphinx Bust",
        "model/sphinx_bust.glb",
        vec3(9.0f, 3.0f, -30.0f),        // Position: Right side, mid-depth
        vec3(0.0f, -45.0f, 0.0f),        // Rotation: Angled toward center
        vec3(1.8f, 1.8f, 1.8f),          // Scale: Imposing presence
        "Limestone sphinx head representing royal power and divine wisdom"
    });
    
    // === ARTIFACT 4: Anubis Statue ===
    // Left front area - god of mummification welcoming visitors
    data.push_back({
        "Anubis Statue",
        "model/anubis_statue.glb",
        vec3(-6.0f, 2.0f, -25.0f),       // Position: Near entrance, left
        vec3(0.0f, 90.0f, 0.0f),         // Rotation: Facing entrance
        vec3(1.5f, 1.5f, 1.5f),          // Scale: Standing jackal-headed god
        "Black basalt statue of Anubis, jackal-headed god of embalming and the afterlife"
    });
    
    // === ARTIFACT 5: Golden Death Mask ===
    // Right front area on pedestal - royal burial treasure
    data.push_back({
        "Golden Death Mask",
        "model/death_mask.glb",
        vec3(6.0f, 5.0f, -33.0f),        // Position: Right side, elevated
        vec3(0.0f, 180.0f, 0.0f),        // Rotation: Facing visitors
        vec3(1.0f, 1.0f, 1.0f),          // Scale: Life-sized mask
        "Gold funerary mask inlaid with lapis lazuli and carnelian, worn by pharaohs in burial"
    });
    
    // NOTE: Monster Totem is rendered directly via renderMonster() - not in exhibits vector
    
    return data;
}

void Room2::initializeExhibits() {
    std::vector<ExhibitData> galleryData = loadGalleryData();
    
    printf("Loading %zu Pharaonic artifacts...\n", galleryData.size());
    
    for (const auto& data : galleryData) {
        Exhibit exhibit;
        exhibit.name = data.name;
        exhibit.description = data.description;
        
        // Load the 3D model
        exhibit.model = rm.loadFBXModel(data.modelPath.c_str());
        
        // Debug output
        if (exhibit.model.vertexCount == 0) {
            printf("  ⚠ WARNING: Failed to load model: %s\n", data.modelPath.c_str());
        } else {
            printf("  ✓ Loaded: %s (vertices: %zu, indices: %zu)\n", 
                   exhibit.name.c_str(), 
                   exhibit.model.vertexCount,
                   exhibit.model.indexCount);
        }
        
        // Build transformation matrix
        mat4 transform = mat4(1.0f);
        transform = translate(transform, data.position);
        transform = rotate(transform, radians(data.rotation.y), vec3(0, 1, 0));
        transform = rotate(transform, radians(data.rotation.x), vec3(1, 0, 0));
        transform = rotate(transform, radians(data.rotation.z), vec3(0, 0, 1));
        transform = scale(transform, data.scale);
        
        exhibit.transform = transform;
        
        exhibits.push_back(exhibit);
    }
    
    printf("Total exhibits loaded: %zu\n", exhibits.size());
}

void Room2::renderExhibits(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");
    
    printf("\n=== Rendering %zu exhibits ===\n", exhibits.size());
    
    for (const auto& exhibit : exhibits) {
        printf("Rendering: %s (vertices: %zu, VAO: %u)\n", 
               exhibit.name.c_str(), 
               exhibit.model.vertexCount,
               exhibit.model.VAO);
               
        if (exhibit.model.vertexCount > 0) {
            glBindVertexArray(exhibit.model.VAO);
            
            mat4 MVP = projection * view * exhibit.transform;
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &exhibit.transform[0][0]);
            
            // Handle textures or material colors (robust logic)
            glActiveTexture(GL_TEXTURE0);
            GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
            if (exhibit.model.textureID > 0) {
                printf("  → Using texture ID: %u\n", exhibit.model.textureID);
                glBindTexture(GL_TEXTURE_2D, exhibit.model.textureID);
                if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
                glUniform1i(UseTextureID, 1);
                glUniform3fv(MaterialColorID, 1, &exhibit.model.baseColor[0]);
            } else {
                printf("  → Using material color (golden)\n");
                glBindTexture(GL_TEXTURE_2D, 0);
                if (TextureID != (GLuint)-1) glUniform1i(TextureID, 0);
                glUniform1i(UseTextureID, 0);
                glUniform3f(MaterialColorID, 0.8f, 0.7f, 0.5f);
            }
            
            glDrawElements(GL_TRIANGLES, exhibit.model.indexCount, GL_UNSIGNED_INT, 0);
            printf("  → Drew %zu indices\n", exhibit.model.indexCount);
        } else {
            printf("  ⚠ Skipping - no vertices!\n");
        }
    }
}
