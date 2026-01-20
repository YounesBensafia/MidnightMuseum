#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include "Camera.hpp"

class Application {
public:
    Application(const std::string& title, int width, int height);
    ~Application();

    // Initialize window and context
    bool init();
    
    // Start the main loop. verifyLoop returns false to quit.
    void run(std::function<void(float)> updateCallback, std::function<void()> renderCallback);

    GLFWwindow* getWindow() const { return window; }
    Camera& getCamera() { return camera; }

    // Callbacks need to be static or friend functions to work with C-style GLFW callbacks
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

private:
    void processInput(float deltaTime);
    void update(float deltaTime);
    void render();

    GLFWwindow* window;
    int width, height;
    std::string title;
    
    // Application State
    static Camera camera;
    static float lastX, lastY;
    static bool firstMouse;
    static float deltaTime;
    static float lastFrame;

    // Initialization
    bool initWindow();
};

#endif
