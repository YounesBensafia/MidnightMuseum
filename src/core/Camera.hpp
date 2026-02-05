#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    Camera(glm::vec3 pos = glm::vec3(0.0f, 1.8f, 5.0f), 
           glm::vec3 worldup = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, 
           float pitch = 0.0f) 
        : position(pos), worldUp(worldup), yaw(yaw), pitch(pitch), 
          front(glm::vec3(0.0f, 0.0f, -1.0f)),
          movementSpeed(2.5f), mouseSensitivity(0.1f), zoom(45.0f) {
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }
    
    glm::vec3 getPosition() { return position; }
    glm::vec3 getFront() { return front; }
    glm::vec3 getRight() { return right; }
    glm::vec3 getUp() { return up; }

    void processKeyboard(int direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        
        // Create horizontal-only movement vectors (no Y component)
        glm::vec3 frontFlat = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        glm::vec3 rightFlat = glm::normalize(glm::vec3(right.x, 0.0f, right.z));
        
        // 0: FORWARD, 1: BACKWARD, 2: LEFT, 3: RIGHT
        if (direction == 0)
            position += frontFlat * velocity;
        if (direction == 1)
            position -= frontFlat * velocity;
        if (direction == 2)
            position -= rightFlat * velocity;
        if (direction == 3)
            position += rightFlat * velocity;
    }

    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void processMouseScroll(float yoffset) {
        zoom -= yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

    void updateCameraVectors() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
        
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

private:
};

#endif
