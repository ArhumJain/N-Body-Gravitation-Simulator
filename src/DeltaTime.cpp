#include "DeltaTime.hpp"
#include "GLFW/glfw3.h"
#include <iostream>

void DeltaTime::calculateDeltaTime() {
    DeltaTime::obj->currentFrame = glfwGetTime();
    DeltaTime::obj->deltaTime = DeltaTime::obj->currentFrame - DeltaTime::obj->lastFrame;
    DeltaTime::obj->lastFrame = DeltaTime::obj->currentFrame;
}

float DeltaTime::getDeltaTime() {
    return DeltaTime::obj->deltaTime;
}

DeltaTime *DeltaTime::obj = new DeltaTime();