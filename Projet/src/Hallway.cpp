#include "../include/Hallway.hpp"
#include "../include/Application.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace glm;

Hallway::Hallway(Application& app) : app(app), rm(ResourceManager::getInstance()),
    inspectionMode(false), inspectedCabinetIndex(-1), inspectionRotationY(0.0f), inspectionRotationX(0.0f),
    wasIKeyPressed(false), wasMousePressed(false), lastMouseX(0.0), lastMouseY(0.0),
    savedCameraYaw(0.0f), savedCameraPitch(0.0f), playerNearCabinet(false),
    promptVAO(0), promptVBO(0) {
}

Hallway::~Hallway() {
    // Cleanup prompt UI
    if (promptVAO != 0) {
        glDeleteVertexArrays(1, &promptVAO);
        glDeleteBuffers(1, &promptVBO);
    }
}

void Hallway::init() {
    printf("\n=== Loading Hallway ===\n");
    
    // Load carpet model for walls/floor/ceiling
    carpetModel = rm.loadModel("model/carpet.obj", true);
    displayCabinetModel = rm.loadFBXModel("model/cabinet2.glb");
    smallFossilModel = rm.loadFBXModel("model/small_fossil1.glb");
    smallFossil2Model = rm.loadFBXModel("model/small_fossil2.glb");
    
    initPromptUI();
}

void Hallway::update(float dt, GLFWwindow* window) {
    Camera& camera = app.getCamera();
    
    if (inspectionMode) {
        // Handle inspection mode input
        bool iKeyPressed = (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS);
        if (iKeyPressed && !wasIKeyPressed) {
            exitInspectionMode();
        }
        wasIKeyPressed = iKeyPressed;
        
        // Handle mouse drag for rotation on both axes
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        
        bool mousePressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        if (mousePressed) {
            if (wasMousePressed) {
                // Calculate mouse delta and update rotation
                float deltaX = static_cast<float>(mouseX - lastMouseX);
                float deltaY = static_cast<float>(mouseY - lastMouseY);
                inspectionRotationY += deltaX * 0.5f;  // Horizontal rotation
                inspectionRotationX += deltaY * 0.5f;  // Vertical rotation
            }
            lastMouseX = mouseX;
            lastMouseY = mouseY;
        }
        wasMousePressed = mousePressed;
        playerNearCabinet = false;
    } else {
        // Check if near cabinet and can inspect
        int cabinetIndex = -1;
        playerNearCabinet = isNearCabinet(camera.position, cabinetIndex);
        
        if (playerNearCabinet) {
            bool iKeyPressed = (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS);
            if (iKeyPressed && !wasIKeyPressed) {
                enterInspectionMode(cabinetIndex);
            }
            wasIKeyPressed = iKeyPressed;
        } else {
            wasIKeyPressed = false;
        }
    }
}

void Hallway::render(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    renderFloorAndCeiling(view, projection, shaderProgram);
    renderWalls(view, projection, shaderProgram);
    renderDisplayCabinets(view, projection, shaderProgram);
    renderSmallFossil(view, projection, shaderProgram);
    renderIKeyPrompt(projection);
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
    if (checkWallCollision(newPos)) return true;
    if (checkCabinetCollision(newPos)) return true;
    return false;
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
    float wallMargin = 0.5f;
    
    // === HALLWAY SIDE WALLS (x = ±6 from z = -15 to z = -22) ===
    if (newPos.z < -15.0f && newPos.z > -22.0f) {
        // East hallway wall at x = 6
        if (newPos.x > 6.0f - wallMargin) return true;
        
        // West hallway wall at x = -6
        if (newPos.x < -6.0f + wallMargin) return true;
    }
    
    return false;
}

bool Hallway::checkCabinetCollision(const vec3& newPos) {
    vec3 cabinetPositions[] = {
        vec3(5.5f, 0.0f, -18.5f),   // East cabinet
        vec3(-5.5f, 0.0f, -18.5f)   // West cabinet
    };
    
    float cabinetRadius = 1.5f;  // Collision radius around cabinet
    
    for (int i = 0; i < 2; i++) {
        float dist = length(vec2(newPos.x - cabinetPositions[i].x, 
                                 newPos.z - cabinetPositions[i].z));
        if (dist < cabinetRadius) {
            return true;
        }
    }
    
    return false;
}

void Hallway::renderSmallFossil(const mat4& view, const mat4& projection, GLuint shaderProgram) {
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    
    if (smallFossilModel.vertexCount > 0) {
        glBindVertexArray(smallFossilModel.VAO);
        
        // Place small fossil inside the east cabinet (same position initially)
        mat4 FossilModel = mat4(1.0f);
        FossilModel = translate(FossilModel, vec3(5.5f, 3.0f, -18.5f));  // Same as east cabinet
        FossilModel = rotate(FossilModel, radians(90.0f), vec3(0, 1, 0));  // Face into hallway
        FossilModel = scale(FossilModel, vec3(1.5f, 1.5f, 1.5f));  // Scaled up to see it
        mat4 FossilMVP = projection * view * FossilModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &FossilMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &FossilModel[0][0]);
        
        if (smallFossilModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, smallFossilModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, smallFossilModel.indexCount, GL_UNSIGNED_INT, 0);
    }
    
    // Render small_fossil2 in the west cabinet
    if (smallFossil2Model.vertexCount > 0) {
        glBindVertexArray(smallFossil2Model.VAO);
        
        // Place small fossil2 inside the west cabinet (mirrored position)
        mat4 Fossil2Model = mat4(1.0f);
        Fossil2Model = translate(Fossil2Model, vec3(-5.5f, 2.5f, -18.5f));  // West cabinet position
        Fossil2Model = rotate(Fossil2Model, radians(90.0f), vec3(0, 1, 0));  // Face into hallway
        Fossil2Model = scale(Fossil2Model, vec3(10.0f, 10.0f, 10.0f));  // Same scale
        mat4 Fossil2MVP = projection * view * Fossil2Model;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Fossil2MVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Fossil2Model[0][0]);
        
        if (smallFossil2Model.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, smallFossil2Model.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, smallFossil2Model.indexCount, GL_UNSIGNED_INT, 0);
    }
}

bool Hallway::isNearCabinet(const vec3& playerPos, int& cabinetIndex) {
    vec3 cabinetPositions[] = {
        vec3(5.5f, 0.0f, -18.5f),   // East cabinet (index 0)
        vec3(-5.5f, 0.0f, -18.5f)   // West cabinet (index 1)
    };
    
    float interactionDistance = 3.0f;  // Distance to trigger inspection
    
    for (int i = 0; i < 2; i++) {
        float dist = length(vec2(playerPos.x - cabinetPositions[i].x, 
                                 playerPos.z - cabinetPositions[i].z));
        if (dist < interactionDistance) {
            cabinetIndex = i;
            return true;
        }
    }
    
    return false;
}

void Hallway::enterInspectionMode(int cabinetIndex) {
    inspectionMode = true;
    inspectedCabinetIndex = cabinetIndex;
    inspectionRotationY = 0.0f;
    inspectionRotationX = 0.0f;
    
    // Save current camera state
    Camera& camera = app.getCamera();
    savedCameraPos = camera.position;
    savedCameraYaw = camera.yaw;
    savedCameraPitch = camera.pitch;
    
    // Position camera for closeup view
    vec3 cabinetPositions[] = {
        vec3(5.5f, 3.0f, -18.5f),   // East cabinet
        vec3(-5.5f, 2.5f, -18.5f)   // West cabinet
    };
    
    vec3 targetPos = cabinetPositions[cabinetIndex];
    vec3 cameraOffset = (cabinetIndex == 0) ? vec3(-2.0f, 0.0f, 0.0f) : vec3(2.0f, 0.0f, 0.0f);
    
    camera.position = targetPos + cameraOffset;
    camera.yaw = (cabinetIndex == 0) ? 0.0f : 180.0f;  // Look at object
    camera.pitch = 0.0f;
    camera.updateCameraVectors();
    
    printf("Entered inspection mode for cabinet %d\n", cabinetIndex);
}

void Hallway::exitInspectionMode() {
    inspectionMode = false;
    
    // Restore camera state
    Camera& camera = app.getCamera();
    camera.position = savedCameraPos;
    camera.yaw = savedCameraYaw;
    camera.pitch = savedCameraPitch;
    camera.updateCameraVectors();
    
    printf("Exited inspection mode\n");
}

void Hallway::updateInspectionCamera(mat4& view, mat4& projection) {
    if (!inspectionMode) return;
    
    Camera& camera = app.getCamera();
    view = camera.getViewMatrix();
    // Projection can stay the same
}

void Hallway::renderInspectionObject(GLuint shaderProgram) {
    if (!inspectionMode) return;
    
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint TextureID = glGetUniformLocation(shaderProgram, "ourTexture");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    
    Camera& camera = app.getCamera();
    mat4 projection = perspective(radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    mat4 view = camera.getViewMatrix();
    
    vec3 cabinetPositions[] = {
        vec3(5.5f, 3.0f, -18.5f),   // East cabinet
        vec3(-5.5f, 2.5f, -18.5f)   // West cabinet
    };
    
    // Render the object floating and rotating
    vec3 objectPos = cabinetPositions[inspectedCabinetIndex];
    
    if (inspectedCabinetIndex == 0 && smallFossilModel.vertexCount > 0) {
        glBindVertexArray(smallFossilModel.VAO);
        
        mat4 ObjectModel = mat4(1.0f);
        ObjectModel = translate(ObjectModel, objectPos);
        ObjectModel = rotate(ObjectModel, radians(inspectionRotationY), vec3(0, 1, 0));  // Horizontal rotation
        ObjectModel = rotate(ObjectModel, radians(inspectionRotationX), vec3(1, 0, 0));  // Vertical rotation
        ObjectModel = scale(ObjectModel, vec3(2.0f, 2.0f, 2.0f));
        mat4 ObjectMVP = projection * view * ObjectModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &ObjectMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &ObjectModel[0][0]);
        
        if (smallFossilModel.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, smallFossilModel.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, smallFossilModel.indexCount, GL_UNSIGNED_INT, 0);
        
    } else if (inspectedCabinetIndex == 1 && smallFossil2Model.vertexCount > 0) {
        glBindVertexArray(smallFossil2Model.VAO);
        
        mat4 ObjectModel = mat4(1.0f);
        ObjectModel = translate(ObjectModel, objectPos);
        ObjectModel = rotate(ObjectModel, radians(inspectionRotationY), vec3(0, 1, 0));  // Horizontal rotation
        ObjectModel = rotate(ObjectModel, radians(inspectionRotationX), vec3(1, 0, 0));  // Vertical rotation
        ObjectModel = scale(ObjectModel, vec3(12.0f, 12.0f, 12.0f));
        mat4 ObjectMVP = projection * view * ObjectModel;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &ObjectMVP[0][0]);
        glUniformMatrix4fv(ModelID, 1, GL_FALSE, &ObjectModel[0][0]);
        
        if (smallFossil2Model.textureID > 0) {
            glBindTexture(GL_TEXTURE_2D, smallFossil2Model.textureID);
        }
        glUniform1i(TextureID, 0);
        glUniform1i(UseTextureID, 1);
        glDrawElements(GL_TRIANGLES, smallFossil2Model.indexCount, GL_UNSIGNED_INT, 0);
    }
}

void Hallway::initPromptUI() {
    // Create geometry for letter "I" - simple vertical bar
    std::vector<float> promptVertices;
    
    // Helper to add a rectangle (2 triangles)
    auto addRect = [&](float x, float y, float w, float h) {
        promptVertices.insert(promptVertices.end(), {x, y + h, 0.0f});
        promptVertices.insert(promptVertices.end(), {x, y, 0.0f});
        promptVertices.insert(promptVertices.end(), {x + w, y, 0.0f});
        
        promptVertices.insert(promptVertices.end(), {x, y + h, 0.0f});
        promptVertices.insert(promptVertices.end(), {x + w, y, 0.0f});
        promptVertices.insert(promptVertices.end(), {x + w, y + h, 0.0f});
    };
    
    // Letter I - vertical bar with top and bottom serifs
    float centerX = 0.0f;
    float centerY = 0.0f;
    float barWidth = 0.03f;
    float barHeight = 0.15f;
    float serifWidth = 0.08f;
    float serifHeight = 0.025f;
    
    // Top serif
    addRect(centerX - serifWidth/2, centerY + barHeight/2 - serifHeight, serifWidth, serifHeight);
    // Vertical bar
    addRect(centerX - barWidth/2, centerY - barHeight/2, barWidth, barHeight);
    // Bottom serif
    addRect(centerX - serifWidth/2, centerY - barHeight/2, serifWidth, serifHeight);
    
    glGenVertexArrays(1, &promptVAO);
    glGenBuffers(1, &promptVBO);
    
    glBindVertexArray(promptVAO);
    glBindBuffer(GL_ARRAY_BUFFER, promptVBO);
    glBufferData(GL_ARRAY_BUFFER, promptVertices.size() * sizeof(float), promptVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void Hallway::renderIKeyPrompt(const mat4& projection) {
    if (!playerNearCabinet || promptVAO == 0 || inspectionMode) return;
    
    // Get shader program from ResourceManager
    GLuint shaderProgram = rm.getShader("mainShader");
    glUseProgram(shaderProgram);
    
    // Disable depth test for 2D overlay
    GLint oldDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
    glDepthFunc(GL_ALWAYS);
    
    glBindVertexArray(promptVAO);
    
    // Create identity matrix for 2D rendering
    mat4 model = mat4(1.0f);
    model = translate(model, vec3(0.0f, -0.8f, 0.0f));
    model = scale(model, vec3(0.8f, 0.8f, 1.0f));
    
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(shaderProgram, "Model");
    GLuint UseTextureID = glGetUniformLocation(shaderProgram, "useTexture");
    GLuint MaterialColorID = glGetUniformLocation(shaderProgram, "materialColor");
    
    mat4 orthoProj = mat4(1.0f);
    mat4 mvp = orthoProj * model;
    
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &model[0][0]);
    glUniform1i(UseTextureID, 0);
    glUniform3f(MaterialColorID, 1.0f, 1.0f, 1.0f);
    
    // Draw letter I (3 rectangles = 18 vertices)
    glDrawArrays(GL_TRIANGLES, 0, 18);
    
    glBindVertexArray(0);
    glDepthFunc(oldDepthFunc);
}
