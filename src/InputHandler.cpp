#include "InputHandler.hpp"
#include "GLFW/glfw3.h"
#include "Particles.hpp"
#include <iostream>

InputHandler::InputHandler(Camera &cam, Particles &p, GLFWwindow *win) {
    this->camera = &cam;
    this->window = win;
    this->particles = &p;
}

void InputHandler::handleInput() {
    this->camera->processInput();
    if (glfwGetKey(this->window, GLFW_KEY_V) == GLFW_PRESS && !this->fpsModeKeyState) {
        if (this->camera->isCameraMovementEnabled()) {
            glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(this->window, this->camera->getLastCursorXPos(), this->camera->getLastCursorYPos());
        }
        this->camera->toggleCameraMovement();
        this->fpsModeKeyState = true;
    } if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE) {
        this->fpsModeKeyState = false;
    }

    this->leftMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    if (this->leftMouseButtonState == GLFW_PRESS && !this->camera->isCameraMovementEnabled()) {
        double x, y;
        glfwGetCursorPos(this->window, &x, &y);
        currentRay = this->camera->getRay(x, y);
        this->particles->enablePointMass(currentRay, *this->camera);
    } 
    if (this->leftMouseButtonState == GLFW_RELEASE) {
        this->particles->disablePointMass();
    }
}

void InputHandler::cursorPosCallback(GLFWwindow *w, double xPos, double yPos) {
    Camera *cam = static_cast<Camera*>(glfwGetWindowUserPointer(w));
    cam->mouseCallback(w, xPos, yPos);
}

void InputHandler::mouseButtonCallback(GLFWwindow *w, int button, int action, int mods) {
    Camera *cam = static_cast<Camera*>(glfwGetWindowUserPointer(w));
}