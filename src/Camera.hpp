#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"
class Camera {
    
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat initialFacing = glm::quat(0.0f, 0.0f, 0.0f, -1.0f);
    glm::quat facing = glm::quat(0.0f, 0.0f, 0.0f, -1.0f);
    glm::quat up = glm::quat(0.0f, 0.0f, 1.0f, 0.0f);
    glm::quat p = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
    glm::quat y = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
 
    glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 moveDirection = glm::vec3(0.0f, 0.0f, 0.0f);

    float pitch = 0;
    float yaw = 0;
    float totalPitch = 0;
    float totalYaw = 0;
    
    bool accelerating = false;
    bool fast = false;
    float acceleration = 600.0f;
    float velocityMag;
    float maxVelocity;
    float drag = 0.05f;
    float sensitivity;
    
    GLFWwindow **window;
    bool initialCursor = true;
    double lastCursorXPos = 0;
    double lastCursorYPos = 0;
    int winWidth, winHeight;
    bool shouldUpdate = false;
    bool cameraMovementEnabled = false;

    glm::mat4 projectionMatrix;
    glm::mat4 *viewMatrix;

    void handleSmoothMovement();
public:
    float n, f, l, r, t, b;
    Camera(float near, float far, float left, float right, float top, float bottom, float speed, float sensitivity, glm::mat4 &projection, glm::mat4 &view, GLFWwindow **window);
    void processInput();
    void updateView(glm::mat4 &view);
    void mouseCallback(GLFWwindow *w, double xPos, double yPos);
    void moveCameraTo(float x, float y, float z);
    void lookAt(float x, float y, float z);
    void toggleUpdate();
    void toggleCameraMovement();
    
    double getLastCursorXPos();
    double getLastCursorYPos();

    float getCameraXPos();
    float getCameraYPos();
    float getCameraZPos();
    glm::vec3 getCameraPos();

    bool isUpdating();
    bool isCameraMovementEnabled();

    glm::vec3 getRay(double screenX, double screenY);
    glm::vec3 getFacing();
};