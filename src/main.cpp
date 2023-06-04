#include "Shader.hpp"
#include <cmath>
#include <malloc.h>
#include <iostream>
#include <ostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/vec2.hpp"
#include "stb_image/stb_image.h"
#include "DeltaTime.hpp"
#include "Camera.hpp"
#include "Particles.hpp"
#include "InputHandler.hpp"


void mouseCallback(GLFWwindow *w, double xPos, double yPos) {
    Camera *camera = static_cast<Camera*>(glfwGetWindowUserPointer(w));
    camera->mouseCallback(w, xPos, yPos);
}

void handleInput(Camera &camera, GLFWwindow *window) {
    camera.processInput();
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, camera.isUpdating() ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }
}
int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 800, "NewtonianParticles", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to initialized window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();
   
    glViewport(0, 0, 800, 800);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glm::mat4 projection;
    glm::mat4 view = glm::mat4(1.0);
    Camera camera(1.0f, 15000.0, -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.2f, projection, view, &window);

    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, InputHandler::cursorPosCallback); 
    glfwSetMouseButtonCallback(window, InputHandler::mouseButtonCallback);
    glfwSwapInterval(0);

    const Shader shader("../res/shaders/vertex.glsl", "../res/shaders/fragment.glsl");
    shader.use();

    float n = 1.0f;
    float f = 100.0f; 
    float r = 1.0f;
    float l = -1.0f;
    float t = 1.0;
    float bot = -1.0;
    float projMat[16] = {(2*n)/(r-l), 0, (r+l)/(r-l), 0, 
                          0, (2*n)/(t-bot), (t+bot)/(t-bot), 0, 
                          0, 0, (-(f+n))/(f-n), (-2*f*n)/(f-n), 
                          0, 0, -1, 0};

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
    Particles particles(25000, shader.getID(), camera);
    InputHandler inputHandler(camera, particles, window);

    double lastTime = glfwGetTime();
    int nbFrames = 0;
    glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "projection"), 1, GL_TRUE, glm::value_ptr(projection));
    while(!glfwWindowShouldClose(window)) {

         double currentTime = glfwGetTime();
         nbFrames++;
         if ( currentTime - lastTime >= 1.0 ){
             printf("%f ms/frame\n", 1000.0/double(nbFrames));
             nbFrames = 0;
             lastTime += 1.0;
         }


        DeltaTime::calculateDeltaTime();
        inputHandler.handleInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        view = glm::mat4(1.0f);
        camera.updateView(view);

        glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));

        particles.render();
        
        // Swap the front buffer (default color) and the back buffers (our newly colored buffer)
        glfwSwapBuffers(window);

        // Handle mouse clicks, resizing, etc. so Window doesn't hang
        glfwPollEvents();
    }

    // Deletes the window specified
    glfwDestroyWindow(window);

    // Terminates GLFW 
    glfwTerminate();
    return 0;
}