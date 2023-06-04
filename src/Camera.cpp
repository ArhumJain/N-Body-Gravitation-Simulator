#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"
#include "glm/gtc/quaternion.hpp"
#include "Camera.hpp" 
#include "DeltaTime.hpp"

Camera::Camera(float near, float far, float left, float right, float top, float bottom, float speed, float sensitivity, glm::mat4 &projection, glm::mat4 &view, GLFWwindow **w) {
    this->n = near;
    this->f = far;
    this->l = left;
    this->r = right;
    this->t = top;
    this->b = bottom;
    float projMat[16] = {(2*n)/(r-l), 0, (r+l)/(r-l), 0, 
                          0, (2*n)/(t-b), (t+b)/(t-b), 0, 
                          0, 0, (-(f+n))/(f-n), (-2*f*n)/(f-n), 
                          0, 0, -1, 0};
    projection = glm::make_mat4(projMat);
    this->projectionMatrix = glm::make_mat4(projMat);
    this->viewMatrix = &view;
    
    this->maxVelocity = speed;
    this->sensitivity = sensitivity;
    this->window = w;
    glfwGetWindowSize(*this->window, &this->winWidth, &this->winHeight);
}

void Camera::handleSmoothMovement() {
    this->velocity += (this->moveDirection * ((this->acceleration + this->fast * (this->acceleration * 5.0f)) * this->accelerating * DeltaTime::getDeltaTime()));
    if (glm::length(this->velocity) < 0.01f) {
        this->velocity *= 0.0f;
    }
    this->cameraPos += (this->velocity) * DeltaTime::getDeltaTime();
    this->velocity *= glm::pow(this->drag, DeltaTime::getDeltaTime());
    this->accelerating = false;
}

void Camera::processInput() {
    if (this->cameraMovementEnabled) {
        
        if (glfwGetKey(*window, GLFW_KEY_W) == GLFW_PRESS) {
            this->moveDirection += glm::vec3(this->facing.x, this->facing.y, this->facing.z);
            this->moveDirection = glm::normalize(this->moveDirection);
            this->accelerating = true;
        }

        if (glfwGetKey(*window, GLFW_KEY_S) == GLFW_PRESS) {
            this->moveDirection -= glm::vec3(this->facing.x, this->facing.y, this->facing.z);
            this->moveDirection = glm::normalize(this->moveDirection);

            this->accelerating = true;
        }
        if (glfwGetKey(*window, GLFW_KEY_D) == GLFW_PRESS) {
            this->moveDirection += glm::cross(glm::vec3(this->facing.x, this->facing.y, this->facing.z),
                                          glm::vec3(this->up.x, this->up.y, this->up.z));;
            this->moveDirection = glm::normalize(this->moveDirection);
            this->accelerating = true;
        }

        if (glfwGetKey(*window, GLFW_KEY_A) == GLFW_PRESS) {
            this->moveDirection -= glm::cross(glm::vec3(this->facing.x, this->facing.y, this->facing.z),
                                          glm::vec3(this->up.x, this->up.y, this->up.z));;
            this->moveDirection = glm::normalize(this->moveDirection);
            this->accelerating = true;
        }

        if (glfwGetKey(*window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            this->moveDirection.y += 1;
            this->moveDirection = glm::normalize(this->moveDirection);
            this->accelerating = true;
        }

        if (glfwGetKey(*window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            this->moveDirection.y -= 1;
            this->moveDirection = glm::normalize(this->moveDirection);
            this->accelerating = true;
        }
        
        if (glfwGetKey(*window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            this->fast = true;
        }
        
        if (glfwGetKey(*window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
            this->fast = false;
        }
    }
    this->handleSmoothMovement();
}

void Camera::mouseCallback(GLFWwindow *w, double xPos, double yPos) {
    if (this->cameraMovementEnabled) {
        if (this->initialCursor) {
            glfwGetWindowSize(w, &this->winHeight, &this->winHeight);
            this->lastCursorXPos = xPos;
            this->lastCursorYPos = yPos;
            this->initialCursor = false;
        }
        this->pitch += (yPos - this->lastCursorYPos) * this->sensitivity;
        this->yaw += (xPos - this->lastCursorXPos) * this->sensitivity;

        if (this->pitch <= -90.0f) {
            this->pitch = -90.0f;
        } else if (pitch >= 90.f) {
            this->pitch = 90.0f;
        }
        this->lastCursorXPos = xPos;
        this->lastCursorYPos = yPos;
    }
}

void Camera::updateView(glm::mat4 &view) {
    this->p = glm::quat(glm::cos(glm::radians(this->pitch/2)), glm::sin(glm::radians(this->pitch/2)), 0.0f, 0.0f);
    this->y = glm::quat(glm::cos(glm::radians(this->yaw/2)), 0.0f, glm::sin(glm::radians(this->yaw/2)), 0.0f);
    this->facing = glm::conjugate(this->p * this->y) * glm::quat(0.0f, 0.0f, 0.0f, -1.0f) * (this->p * this->y);
    this->up = glm::conjugate(this->p * this->y) * glm::quat(0.0f, 0.0f, 1.0f, 0.0f) * (this->p * this->y);
    
    view = glm::mat4_cast(this->p * this->y) * view;
    view = glm::translate(view, -cameraPos);
    view = DeltaTime::getDeltaTime() * view;

}

void Camera::moveCameraTo(float x, float y, float z) {
    this->cameraPos = glm::vec3(x, y, z);
}

void Camera::lookAt(float x, float y, float z) {
    glm::vec3 n = glm::normalize(glm::vec3(x, y, z));
    this->pitch = glm::degrees(glm::atan(n.y));
    this->yaw = glm::degrees(glm::atan(-n.x, n.z));
}

void Camera::toggleUpdate() {
    this->shouldUpdate = 1 - this->shouldUpdate;
}

void Camera::toggleCameraMovement() {
    this->cameraMovementEnabled = 1 - this->cameraMovementEnabled;
}

bool Camera::isCameraMovementEnabled() {
    return this->cameraMovementEnabled;
}

bool Camera::isUpdating() {
    return this->shouldUpdate;
}

float Camera::getCameraXPos() {
    return this->cameraPos.x;
}

float Camera::getCameraYPos() {
    return this->cameraPos.y;
}

float Camera::getCameraZPos() {
    return this->cameraPos.z;
}

double Camera::getLastCursorXPos() {
    return this->lastCursorXPos;
}

double Camera::getLastCursorYPos() {
    return this->lastCursorYPos;
}

glm::vec3 Camera::getCameraPos() {
    return this->cameraPos;
}

glm::vec3 Camera::getFacing() {
   return glm::vec3(this->facing.x, this->facing.y, this->facing.z);
}

glm::vec3 Camera::getRay(double screenX, double screenY) {
    float x = (2.0f * screenX) / this->winWidth - 1.0f;
    float y = 1.0f - (2.0f * screenY) / this->winHeight;
    float z = 1.0f; 
    
    glm::vec4 rayClip = glm::vec4(x,y,-1.0f,1.0f);

    glm::vec4 rayEye = glm::inverse(glm::transpose(this->projectionMatrix)) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
    
    glm::vec4 world = glm::inverse(*(this->viewMatrix)) * rayEye;

    glm::vec3 rayWorld = glm::vec3(world.x, world.y, world.z);
    rayWorld = glm::normalize(rayWorld);
    
    return rayWorld;
    
}
