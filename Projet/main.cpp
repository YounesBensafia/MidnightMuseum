#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "shader/shader/shader.hpp"
#include "model/objload.hpp"
#include "include/Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

using namespace std;
using namespace glm;

// Camera
Camera camera(vec3(0.0f, 2.0f, 10.0f));
float lastX = 320.0f;
float lastY = 240.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Mouse callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

// Scroll callback
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}

// Process input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // Check if shift is pressed for faster movement
    float speedMultiplier = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || 
        glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        speedMultiplier = 2.5f; // 2.5x faster when holding shift
    }
    
    float adjustedDeltaTime = deltaTime * speedMultiplier;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(0, adjustedDeltaTime); // FORWARD
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(1, adjustedDeltaTime); // BACKWARD
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(2, adjustedDeltaTime); // LEFT
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(3, adjustedDeltaTime); // RIGHT
}

GLuint textures[6]; // array of 6 textures

// Structure to hold a 3D model
struct Model {
    GLuint VAO;
    GLuint VBO;
    size_t vertexCount;
    string name;
};

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texture;
};

struct ModelWithMaterial {
    GLuint VAO;
    GLuint VBO;
    size_t vertexCount;
    string name;
    vector<string> materialNames; // Per-vertex material names
    map<string, Material> materials; // Material library
};

// Load a model from OBJ file with material support
ModelWithMaterial loadModelWithMaterial(const char* filepath) {
    ModelWithMaterial model;
    model.name = filepath;
    
    vector<vec3> positions;
    vector<vec2> uvs;
    vector<vec3> normals;
    
    bool res = loadOBJWithMaterials(filepath, positions, uvs, normals, model.materialNames, model.materials);
    if (!res) {
        printf("Failed to load: %s\n", filepath);
        model.vertexCount = 0;
        return model;
    }
    
    printf("Loaded %s: %zu vertices with %zu materials\n", filepath, positions.size(), model.materials.size());
    
    // Create interleaved vertex data
    vector<Vertex> vertices;
    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.position = positions[i];
        v.normal = normals[i];
        v.texture = uvs[i];
        vertices.push_back(v);
    }
    
    model.vertexCount = vertices.size();
    
    glGenVertexArrays(1, &model.VAO);
    glBindVertexArray(model.VAO);
    
    glGenBuffers(1, &model.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return model;
}

// Load a model from OBJ file
Model loadModel(const char* filepath, bool regenerateUVs = false) {
    Model model;
    model.name = filepath;
    
    vector<vec3> positions;
    vector<vec2> uvs;
    vector<vec3> normals;
    
    bool res = loadOBJ(filepath, positions, uvs, normals);
    if (!res) {
        printf("Failed to load: %s\n", filepath);
        model.vertexCount = 0;
        return model;
    }
    
    printf("Loaded %s: %zu vertices\n", filepath, positions.size());
    
    // Create interleaved vertex data
    vector<Vertex> vertices;
    for (size_t i = 0; i < positions.size(); i++) {
        Vertex v;
        v.position = positions[i];
        
        // Fix normals for planar surfaces - ensure they all point straight up for consistent lighting
        if (regenerateUVs) {
            // Force all normals to point straight up for flat surfaces
            v.normal = vec3(0, 1, 0);
        } else {
            v.normal = (i < normals.size()) ? normals[i] : vec3(0, 1, 0);
        }
        
        // Regenerate UVs for planar surfaces (like floor/walls)
        if (regenerateUVs) {
            // Use world-space coordinates with tile size (2.0 units per tile)
            float tileSize = 1.0f;
            v.texture = vec2(positions[i].x / tileSize, 
                            positions[i].z / tileSize);
        } else {
            v.texture = (i < uvs.size()) ? uvs[i] : vec2(0, 0);
        }
        vertices.push_back(v);
    }
    
    model.vertexCount = vertices.size();
    
    glGenVertexArrays(1, &model.VAO);
    glBindVertexArray(model.VAO);
    
    glGenBuffers(1, &model.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return model;
}

GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    
    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        printf("Texture loaded: %s (%dx%d, %d channels)\n", path, width, height, nrChannels);
        stbi_image_free(data);
    } else {
        printf("Failed to load texture: %s\n", path);
        stbi_image_free(data);
    }
    
    return textureID;
}

void InitTextures(void)
{
    printf("\n=== Loading Textures ===\n");
    
    // Load floor tile texture
    textures[0] = loadTexture("textures/tiles/tiles_0099_color_1k.jpg");
    
    // Load wall wood texture
    textures[1] = loadTexture("textures/wall/wood_0013_color_1k.jpg");
    
    // Load fossils bones texture
    textures[2] = loadTexture("model/bones.png");
    
    // Load additional textures (you can add more)
    textures[3] = loadTexture("textures/tiles/tiles_0099_color_1k.jpg");
    textures[4] = loadTexture("textures/tiles/tiles_0099_color_1k.jpg");
    textures[5] = loadTexture("textures/tiles/tiles_0099_color_1k.jpg");
}

int main()
{
    if (!glfwInit())
    {
        printf("Could not initialize GLFW.\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(1280, 720, "Museum 3D - Synthese d'Image", NULL, NULL);
    if (!window)
    {
        printf("Failed to create GLFW window.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // Setup callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Could not initialize GLAD" << endl;
        return -1;
    }

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Load multiple models
    printf("\n=== Loading Museum Models ===\n");
    Model carpetModel = loadModel("model/carpet.obj", true); // Regenerate UVs for proper tiling
    ModelWithMaterial museumModel = loadModelWithMaterial("model/museum.obj");
    Model fossilsModel = loadModel("model/fossils1.obj", false);
    // Add more models here as you get them:
    // Model dinoSkullModel = loadModel("model/dino_skull.obj");
    // Model armorModel = loadModel("model/armor.obj");

    GLuint ShaderProgram = LoadShaders("shader/shader/SimpleVertexShader.vertexshader",
                                       "shader/shader/SimpleFragmentShader.fragmentshader");

    InitTextures();

    GLuint MatrixID = glGetUniformLocation(ShaderProgram, "MVP");
    GLuint ModelID = glGetUniformLocation(ShaderProgram, "Model");
    
    vec3 lightColor(1.0f, 1.0f, 1.0f);
    GLuint LightClrID = glGetUniformLocation(ShaderProgram, "lightColor");
    
    GLuint LightPosID = glGetUniformLocation(ShaderProgram, "lightPos");
    GLuint NumLightsID = glGetUniformLocation(ShaderProgram, "numLights");
    
    vec3 viewPos(0.0f, 2.0f, 5.0f);
    GLuint ViewPosID = glGetUniformLocation(ShaderProgram, "viewPos");
    
    GLuint TextureID = glGetUniformLocation(ShaderProgram, "ourTexture");
    GLuint UseTextureID = glGetUniformLocation(ShaderProgram, "useTexture");
    GLuint MaterialColorID = glGetUniformLocation(ShaderProgram, "materialColor");

    glClearColor(0.15f, 0.15f, 0.2f, 1.0f); // Dark museum atmosphere
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Corner lights at ceiling level - 2 per room positioned like lamps
    vec3 cornerLights[] = {
        vec3(-12.0f, 7.5f, -12.0f),  // Main room - back left corner
        vec3(12.0f, 7.5f, -12.0f),   // Main room - back right corner
        vec3(-5.0f, 7.5f, -20.0f),   // Hallway - left side
        vec3(5.0f, 7.5f, -20.0f),    // Hallway - right side
        vec3(-10.0f, 7.5f, -31.0f),  // Second room - back left corner
        vec3(10.0f, 7.5f, -31.0f)    // Second room - back right corner
    };
    int numLights = 6;

    while (!glfwWindowShouldClose(window))
    {
        // Delta time calculation
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Input
        processInput(window);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float time = currentFrame;
        
        // Update view matrix from camera
        mat4 View = camera.getViewMatrix();
        mat4 Projection = perspective(radians(camera.zoom), 1280.0f / 720.0f, 0.1f, 100.0f);
        
        viewPos = camera.position;

        glUseProgram(ShaderProgram);
        glUniform3fv(LightClrID, 1, &lightColor[0]);
        // Send all light positions to shader
        for (int i = 0; i < numLights; i++) {
            std::string lightPosName = "lightPos[" + std::to_string(i) + "]";
            GLuint lightID = glGetUniformLocation(ShaderProgram, lightPosName.c_str());
            glUniform3fv(lightID, 1, &cornerLights[i][0]);
        }
        glUniform1i(NumLightsID, numLights);
        glUniform3fv(ViewPosID, 1, &viewPos[0]);
        
        // === DRAW MUSEUM ARCHITECTURE (Using Carpet Model) ===
        if (carpetModel.vertexCount > 0) {
            glBindVertexArray(carpetModel.VAO);
            
            // === ONE LARGE FLOOR FOR ENTIRE MUSEUM ===
            mat4 FloorModel = mat4(1.0f);
            FloorModel = translate(FloorModel, vec3(0.0f, 0.0f, -14.0f)); // Center between all rooms
            FloorModel = scale(FloorModel, vec3(15.0f, 1.0f, 20.0f)); // Large enough to cover all rooms
            mat4 FloorMVP = Projection * View * FloorModel;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &FloorMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &FloorModel[0][0]);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            glUniform1i(TextureID, 0);
            glUniform1i(UseTextureID, 1); // Use texture
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // === CEILINGS ===
            mat4 HallCeiling = mat4(1.0f);
            HallCeiling = translate(HallCeiling, vec3(0.0f, 8.0f, -19.0f));
            HallCeiling = rotate(HallCeiling, radians(180.0f), vec3(1, 0, 0));
            HallCeiling = scale(HallCeiling, vec3(3.0f, 1.0f, 2.5f));
            mat4 HallCeilingMVP = Projection * View * HallCeiling;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &HallCeilingMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallCeiling[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // === SECOND ROOM (Beyond hallway) ===
            // CEILING - Second Room
            mat4 Ceiling2 = mat4(1.0f);
            Ceiling2 = translate(Ceiling2, vec3(0.0f, 8.0f, -28.0f));
            Ceiling2 = rotate(Ceiling2, radians(180.0f), vec3(1, 0, 0));
            Ceiling2 = scale(Ceiling2, vec3(6.0f, 1.0f, 5.0f));
            mat4 Ceiling2MVP = Projection * View * Ceiling2;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Ceiling2MVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Ceiling2[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // CEILING
            mat4 CeilingModel = mat4(1.0f);
            CeilingModel = translate(CeilingModel, vec3(0.0f, 8.0f, 0.0f));
            CeilingModel = rotate(CeilingModel, radians(180.0f), vec3(1, 0, 0)); // Flip
            CeilingModel = scale(CeilingModel, vec3(7.0f, 1.0f, 7.0f));
            mat4 CeilingMVP = Projection * View * CeilingModel;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &CeilingMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &CeilingModel[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glUniform1i(UseTextureID, 1); // Use texture
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // WALL - NORTH (back) - WITH DOORWAY TO HALLWAY
            float wallThickness = 0.3f;
            
            // North wall - Left side of doorway (front face)
            mat4 WallNorthLeft = mat4(1.0f);
            WallNorthLeft = translate(WallNorthLeft, vec3(-8.0f, 4.0f, -14.0f));
            WallNorthLeft = rotate(WallNorthLeft, radians(90.0f), vec3(1, 0, 0));
            WallNorthLeft = scale(WallNorthLeft, vec3(3.0f, 1.0f, 4.0f));
            mat4 WallNorthLeftMVP = Projection * View * WallNorthLeft;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthLeftMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthLeft[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glUniform1i(UseTextureID, 1);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // North wall - Left side (back face for thickness)
            mat4 WallNorthLeftBack = mat4(1.0f);
            WallNorthLeftBack = translate(WallNorthLeftBack, vec3(-8.0f, 4.0f, -14.0f - wallThickness));
            WallNorthLeftBack = rotate(WallNorthLeftBack, radians(-90.0f), vec3(1, 0, 0));
            WallNorthLeftBack = scale(WallNorthLeftBack, vec3(3.0f, 1.0f, 4.0f));
            mat4 WallNorthLeftBackMVP = Projection * View * WallNorthLeftBack;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthLeftBackMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthLeftBack[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // North wall - Right side of doorway (front face)
            mat4 WallNorthRight = mat4(1.0f);
            WallNorthRight = translate(WallNorthRight, vec3(8.0f, 4.0f, -14.0f));
            WallNorthRight = rotate(WallNorthRight, radians(90.0f), vec3(1, 0, 0));
            WallNorthRight = scale(WallNorthRight, vec3(3.0f, 1.0f, 4.0f));
            mat4 WallNorthRightMVP = Projection * View * WallNorthRight;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthRightMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthRight[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // North wall - Right side (back face)
            mat4 WallNorthRightBack = mat4(1.0f);
            WallNorthRightBack = translate(WallNorthRightBack, vec3(8.0f, 4.0f, -14.0f - wallThickness));
            WallNorthRightBack = rotate(WallNorthRightBack, radians(-90.0f), vec3(1, 0, 0));
            WallNorthRightBack = scale(WallNorthRightBack, vec3(3.0f, 1.0f, 4.0f));
            mat4 WallNorthRightBackMVP = Projection * View * WallNorthRightBack;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthRightBackMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthRightBack[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // North wall - Top of doorway (front face)
            mat4 WallNorthTop = mat4(1.0f);
            WallNorthTop = translate(WallNorthTop, vec3(0.0f, 6.5f, -14.0f));
            WallNorthTop = rotate(WallNorthTop, radians(90.0f), vec3(1, 0, 0));
            WallNorthTop = scale(WallNorthTop, vec3(2.0f, 1.0f, 1.5f));
            mat4 WallNorthTopMVP = Projection * View * WallNorthTop;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthTopMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthTop[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // North wall - Top (back face)
            mat4 WallNorthTopBack = mat4(1.0f);
            WallNorthTopBack = translate(WallNorthTopBack, vec3(0.0f, 6.5f, -14.0f - wallThickness));
            WallNorthTopBack = rotate(WallNorthTopBack, radians(-90.0f), vec3(1, 0, 0));
            WallNorthTopBack = scale(WallNorthTopBack, vec3(2.0f, 1.0f, 1.5f));
            mat4 WallNorthTopBackMVP = Projection * View * WallNorthTopBack;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallNorthTopBackMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallNorthTopBack[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // WALL - SOUTH (front)
            mat4 WallSouth = mat4(1.0f);
            WallSouth = translate(WallSouth, vec3(0.0f, 4.0f, 14.0f));
            WallSouth = rotate(WallSouth, radians(-90.0f), vec3(1, 0, 0));
            WallSouth = scale(WallSouth, vec3(7.0f, 1.0f, 4.0f));
            mat4 WallSouthMVP = Projection * View * WallSouth;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallSouthMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallSouth[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glUniform1i(UseTextureID, 1); // Use texture
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // WALL - EAST (right) - Solid wall
            mat4 WallEast = mat4(1.0f);
            WallEast = translate(WallEast, vec3(14.0f, 4.0f, 0.0f));
            WallEast = rotate(WallEast, radians(90.0f), vec3(1, 0, 0));
            WallEast = rotate(WallEast, radians(90.0f), vec3(0, 0, 1));
            WallEast = scale(WallEast, vec3(7.0f, 1.0f, 4.0f));
            mat4 WallEastMVP = Projection * View * WallEast;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallEastMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallEast[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glUniform1i(UseTextureID, 1);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // WALL - WEST (left)
            mat4 WallWest = mat4(1.0f);
            WallWest = translate(WallWest, vec3(-14.0f, 4.0f, 0.0f));
            WallWest = rotate(WallWest, radians(90.0f), vec3(1, 0, 0)); // Stand up vertically
            WallWest = rotate(WallWest, radians(-90.0f), vec3(0, 0, 1)); // Orient along Z axis
            WallWest = scale(WallWest, vec3(7.0f, 1.0f, 4.0f));
            mat4 WallWestMVP = Projection * View * WallWest;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &WallWestMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &WallWest[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glUniform1i(UseTextureID, 1); // Use texture
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // === HALLWAY WALLS ===
            // Hallway East Wall (with thickness)
            mat4 HallEast = mat4(1.0f);
            HallEast = translate(HallEast, vec3(6.0f, 4.0f, -18.5f));
            HallEast = rotate(HallEast, radians(90.0f), vec3(1, 0, 0));
            HallEast = rotate(HallEast, radians(90.0f), vec3(0, 0, 1));
            HallEast = scale(HallEast, vec3(2.2f, 1.0f, 4.0f));
            mat4 HallEastMVP = Projection * View * HallEast;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &HallEastMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallEast[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            mat4 HallEastBack = mat4(1.0f);
            HallEastBack = translate(HallEastBack, vec3(6.0f + wallThickness, 4.0f, -18.5f));
            HallEastBack = rotate(HallEastBack, radians(90.0f), vec3(1, 0, 0));
            HallEastBack = rotate(HallEastBack, radians(-90.0f), vec3(0, 0, 1));
            HallEastBack = scale(HallEastBack, vec3(2.2f, 1.0f, 4.0f));
            mat4 HallEastBackMVP = Projection * View * HallEastBack;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &HallEastBackMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallEastBack[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // Hallway West Wall (with thickness)
            mat4 HallWest = mat4(1.0f);
            HallWest = translate(HallWest, vec3(-6.0f, 4.0f, -18.5f));
            HallWest = rotate(HallWest, radians(90.0f), vec3(1, 0, 0));
            HallWest = rotate(HallWest, radians(-90.0f), vec3(0, 0, 1));
            HallWest = scale(HallWest, vec3(2.2f, 1.0f, 4.0f));
            mat4 HallWestMVP = Projection * View * HallWest;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &HallWestMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallWest[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            mat4 HallWestBack = mat4(1.0f);
            HallWestBack = translate(HallWestBack, vec3(-6.0f - wallThickness, 4.0f, -18.5f));
            HallWestBack = rotate(HallWestBack, radians(90.0f), vec3(1, 0, 0));
            HallWestBack = rotate(HallWestBack, radians(90.0f), vec3(0, 0, 1));
            HallWestBack = scale(HallWestBack, vec3(2.2f, 1.0f, 4.0f));
            mat4 HallWestBackMVP = Projection * View * HallWestBack;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &HallWestBackMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &HallWestBack[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // === SECOND ROOM WALLS ===
            // Second Room South Wall - WITH DOORWAY FROM HALLWAY
            // South wall - Left side of doorway (front face)
            mat4 Room2SouthLeft = mat4(1.0f);
            Room2SouthLeft = translate(Room2SouthLeft, vec3(-7.0f, 4.0f, -23.0f));
            Room2SouthLeft = rotate(Room2SouthLeft, radians(-90.0f), vec3(1, 0, 0));
            Room2SouthLeft = scale(Room2SouthLeft, vec3(2.5f, 1.0f, 4.0f));
            mat4 Room2SouthLeftMVP = Projection * View * Room2SouthLeft;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2SouthLeftMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthLeft[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // South wall - Left side (back face)
            mat4 Room2SouthLeftBack = mat4(1.0f);
            Room2SouthLeftBack = translate(Room2SouthLeftBack, vec3(-7.0f, 4.0f, -23.0f + wallThickness));
            Room2SouthLeftBack = rotate(Room2SouthLeftBack, radians(90.0f), vec3(1, 0, 0));
            Room2SouthLeftBack = scale(Room2SouthLeftBack, vec3(2.5f, 1.0f, 4.0f));
            mat4 Room2SouthLeftBackMVP = Projection * View * Room2SouthLeftBack;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2SouthLeftBackMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthLeftBack[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // South wall - Right side of doorway (front face)
            mat4 Room2SouthRight = mat4(1.0f);
            Room2SouthRight = translate(Room2SouthRight, vec3(7.0f, 4.0f, -23.0f));
            Room2SouthRight = rotate(Room2SouthRight, radians(-90.0f), vec3(1, 0, 0));
            Room2SouthRight = scale(Room2SouthRight, vec3(2.5f, 1.0f, 4.0f));
            mat4 Room2SouthRightMVP = Projection * View * Room2SouthRight;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2SouthRightMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthRight[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // South wall - Right side (back face)
            mat4 Room2SouthRightBack = mat4(1.0f);
            Room2SouthRightBack = translate(Room2SouthRightBack, vec3(7.0f, 4.0f, -23.0f + wallThickness));
            Room2SouthRightBack = rotate(Room2SouthRightBack, radians(90.0f), vec3(1, 0, 0));
            Room2SouthRightBack = scale(Room2SouthRightBack, vec3(2.5f, 1.0f, 4.0f));
            mat4 Room2SouthRightBackMVP = Projection * View * Room2SouthRightBack;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2SouthRightBackMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthRightBack[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // South wall - Top of doorway (front face)
            mat4 Room2SouthTop = mat4(1.0f);
            Room2SouthTop = translate(Room2SouthTop, vec3(0.0f, 6.5f, -23.0f));
            Room2SouthTop = rotate(Room2SouthTop, radians(-90.0f), vec3(1, 0, 0));
            Room2SouthTop = scale(Room2SouthTop, vec3(2.0f, 1.0f, 1.5f));
            mat4 Room2SouthTopMVP = Projection * View * Room2SouthTop;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2SouthTopMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthTop[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // South wall - Top (back face)
            mat4 Room2SouthTopBack = mat4(1.0f);
            Room2SouthTopBack = translate(Room2SouthTopBack, vec3(0.0f, 6.5f, -23.0f + wallThickness));
            Room2SouthTopBack = rotate(Room2SouthTopBack, radians(90.0f), vec3(1, 0, 0));
            Room2SouthTopBack = scale(Room2SouthTopBack, vec3(2.0f, 1.0f, 1.5f));
            mat4 Room2SouthTopBackMVP = Projection * View * Room2SouthTopBack;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2SouthTopBackMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2SouthTopBack[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // Second Room North Wall (far end) with thickness
            mat4 Room2North = mat4(1.0f);
            Room2North = translate(Room2North, vec3(0.0f, 4.0f, -38.0f));
            Room2North = rotate(Room2North, radians(90.0f), vec3(1, 0, 0));
            Room2North = scale(Room2North, vec3(6.0f, 1.0f, 4.0f));
            mat4 Room2NorthMVP = Projection * View * Room2North;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2NorthMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2North[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            mat4 Room2NorthBack = mat4(1.0f);
            Room2NorthBack = translate(Room2NorthBack, vec3(0.0f, 4.0f, -38.0f - wallThickness));
            Room2NorthBack = rotate(Room2NorthBack, radians(-90.0f), vec3(1, 0, 0));
            Room2NorthBack = scale(Room2NorthBack, vec3(6.0f, 1.0f, 4.0f));
            mat4 Room2NorthBackMVP = Projection * View * Room2NorthBack;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2NorthBackMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2NorthBack[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // Second Room East Wall (with thickness)
            mat4 Room2East = mat4(1.0f);
            Room2East = translate(Room2East, vec3(12.0f, 4.0f, -28.0f));
            Room2East = rotate(Room2East, radians(90.0f), vec3(1, 0, 0));
            Room2East = rotate(Room2East, radians(90.0f), vec3(0, 0, 1));
            Room2East = scale(Room2East, vec3(5.0f, 1.0f, 4.0f));
            mat4 Room2EastMVP = Projection * View * Room2East;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2EastMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2East[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            mat4 Room2EastBack = mat4(1.0f);
            Room2EastBack = translate(Room2EastBack, vec3(12.0f + wallThickness, 4.0f, -28.0f));
            Room2EastBack = rotate(Room2EastBack, radians(90.0f), vec3(1, 0, 0));
            Room2EastBack = rotate(Room2EastBack, radians(-90.0f), vec3(0, 0, 1));
            Room2EastBack = scale(Room2EastBack, vec3(5.0f, 1.0f, 4.0f));
            mat4 Room2EastBackMVP = Projection * View * Room2EastBack;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2EastBackMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2EastBack[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            // Second Room West Wall (with thickness)
            mat4 Room2West = mat4(1.0f);
            Room2West = translate(Room2West, vec3(-12.0f, 4.0f, -28.0f));
            Room2West = rotate(Room2West, radians(90.0f), vec3(1, 0, 0));
            Room2West = rotate(Room2West, radians(-90.0f), vec3(0, 0, 1));
            Room2West = scale(Room2West, vec3(5.0f, 1.0f, 4.0f));
            mat4 Room2WestMVP = Projection * View * Room2West;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2WestMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2West[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
            
            mat4 Room2WestBack = mat4(1.0f);
            Room2WestBack = translate(Room2WestBack, vec3(-12.0f - wallThickness, 4.0f, -28.0f));
            Room2WestBack = rotate(Room2WestBack, radians(90.0f), vec3(1, 0, 0));
            Room2WestBack = rotate(Room2WestBack, radians(90.0f), vec3(0, 0, 1));
            Room2WestBack = scale(Room2WestBack, vec3(5.0f, 1.0f, 4.0f));
            mat4 Room2WestBackMVP = Projection * View * Room2WestBack;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &Room2WestBackMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Room2WestBack[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, carpetModel.vertexCount);
        }
        
        // === DRAW FOSSILS MODEL ===
        if (fossilsModel.vertexCount > 0) {
            glBindVertexArray(fossilsModel.VAO);
            
            mat4 FossilModel = mat4(1.0f);
            FossilModel = translate(FossilModel, vec3(0.0f, 0.0f, 0.0f)); // Center of first room, on the floor
            FossilModel = rotate(FossilModel, radians(45.0f), vec3(0, 1, 0)); // Rotate for better view
            FossilModel = scale(FossilModel, vec3(0.005f, 0.005f, 0.005f)); // Scale down - model is very large
            mat4 FossilMVP = Projection * View * FossilModel;
            
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &FossilMVP[0][0]);
            glUniformMatrix4fv(ModelID, 1, GL_FALSE, &FossilModel[0][0]);
            glBindTexture(GL_TEXTURE_2D, textures[2]); // bones texture
            glUniform1i(UseTextureID, 1); // Use texture
            glDrawArrays(GL_TRIANGLES, 0, fossilsModel.vertexCount);
        }
        
        // === DRAW MUSEUM TABLES WITH PROPER MATERIALS ===
        if (museumModel.vertexCount > 0) {
            glBindVertexArray(museumModel.VAO);
            glUniform1i(UseTextureID, 0); // Use material colors
            
            // Table positions: 2 on left side, 2 on right side
            vec3 tablePositions[] = {
                vec3(-10.0f, 0.0f, -8.0f),  // Left front
                vec3(-10.0f, 0.0f, 2.0f),   // Left back
                vec3(10.0f, 0.0f, -8.0f),   // Right front
                vec3(10.0f, 0.0f, 2.0f)     // Right back
            };
            
            // Draw each table
            for (int t = 0; t < 4; t++) {
                mat4 Model = mat4(1.0f);
                Model = translate(Model, tablePositions[t]);
                Model = scale(Model, vec3(1.0f, 1.0f, 1.0f));
                mat4 MVP = Projection * View * Model;

                glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
                glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);
                
                // Draw with per-triangle materials
                string currentMat = "";
                for (size_t i = 0; i < museumModel.vertexCount; i += 3) {
                    if (i < museumModel.materialNames.size()) {
                        string matName = museumModel.materialNames[i];
                        if (matName != currentMat) {
                            currentMat = matName;
                            if (museumModel.materials.find(matName) != museumModel.materials.end()) {
                                Material& mat = museumModel.materials[matName];
                                glUniform3fv(MaterialColorID, 1, &mat.Kd[0]);
                            }
                        }
                    }
                    glDrawArrays(GL_TRIANGLES, i, 3);
                }
            }
        }
        
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &carpetModel.VAO);
    glDeleteBuffers(1, &carpetModel.VBO);
    glDeleteVertexArrays(1, &museumModel.VAO);
    glDeleteBuffers(1, &museumModel.VBO);
    glDeleteVertexArrays(1, &fossilsModel.VAO);
    glDeleteBuffers(1, &fossilsModel.VBO);
    glDeleteProgram(ShaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
