#pragma once
#include "Camera.hpp"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "Particles.hpp"


class InputHandler {
    Camera *camera;
    GLFWwindow *window;
    Particles *particles;
    

    bool fpsModeKeyState = false; // State for if key 'V' is pressed/not pressed to handle view mode toggling
    bool leftMouseButtonState = 0;
    
    glm::vec3 currentRay;
    
public:
    InputHandler(Camera &cam, Particles &p, GLFWwindow *win);
    static void cursorPosCallback(GLFWwindow *w, double xPos, double yPos);
    static void mouseButtonCallback(GLFWwindow *w, int button, int action, int mods);
    void handleInput();
};