#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Particles.hpp"
#include "Camera.hpp"
#include "DeltaTime.hpp"
#include <iostream>
#include <random>
#include <chrono>


Particles::Particles(unsigned long c, unsigned int program, Camera &camera) {
    firstRender = false;
    currBuffer = 0;
    
    int cubeCount = static_cast<int>(glm::pow(c, (1.0f/3.0f)));
    float spacing = 10.0f/cubeCount;
    this->count = glm::pow(cubeCount, 3);
    particles = new float[count * 9];
    transformedParticles = new float[count * 3];

    int i = 0;
    int l = 0;
    std::mt19937_64 rng;
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    std::uniform_real_distribution<float> unif(0, 1);
    float spreadFactor = 3.0f;
    cubeCount = 100.0f;
    for (int k=0; k<count; k++) {
        particles[i] = unif(rng) * cubeCount * spreadFactor;
        particles[i+1] = unif(rng) * cubeCount * spreadFactor;
        particles[i+2] = unif(rng) * cubeCount * spreadFactor;

        transformedParticles[l] = particles[i];
        transformedParticles[l+1] = particles[i+1];
        transformedParticles[l+2] = particles[i+2];
        for (int j=3; j<=8; j++) {
            particles[i+j] = 0.0f;
        }
        i += 9;  
        l += 3;
    }
    camera.moveCameraTo(cubeCount * spreadFactor + 10.0f, cubeCount * spreadFactor + 10.0f, cubeCount * spreadFactor + 10.0f);
    camera.lookAt(1,1,1);
    // Point Mass SSBO
    glGenBuffers(1, &this->pointMassSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->pointMassSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 5, this->pointMassData, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, this->pointMassSSBO);
    
    // Point Data SSBO
    glGenBuffers(1, &pointsSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->pointsSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 3 * count, this->transformedParticles, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, this->pointsSSBO);

    glGenVertexArrays(2, &VAO[0]);
    glGenBuffers(2, &VBO[0]);
    glGenTransformFeedbacks(2, &TFBO[0]);
    {
        glBindVertexArray(VAO[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9 * count, this->particles, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(sizeof(float) * 6));
        glEnableVertexAttribArray(2);
    } 
    
    {
        glBindVertexArray(VAO[1]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9 * count, (void *)0, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(sizeof(float) * 6));
        glEnableVertexAttribArray(2);
    }
    
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TFBO[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, VBO[0]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TFBO[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, VBO[1]);

    glUniform1ui(glGetUniformLocation(program, "particleCount"), this->count);
}

Particles::~Particles() {
    delete [] particles;
}

void Particles::render() {
    glBindVertexArray(VAO[currBuffer]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, TFBO[!currBuffer]);
    glBeginTransformFeedback(GL_POINTS);
    if (!firstRender) {
        glBindVertexArray(VAO[currBuffer]);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->pointsSSBO);
        glDrawArrays(GL_POINTS, 0, count * 9);
        firstRender = true;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    } else {
        glDrawTransformFeedback(GL_POINTS, TFBO[currBuffer]);
    }
    currBuffer = !currBuffer;
    glEndTransformFeedback();
    std::cout << "Pos X: " << transformedParticles[0] 
              << "Pos Y: " << transformedParticles[1] 
              << "Pos Z: " << transformedParticles[2] << std::endl;
    std::cout << "Pos X2: " << transformedParticles[0+3] 
              << "Pos Y2: " << transformedParticles[1+3] 
              << "Pos Z2: " << transformedParticles[2+3] << std::endl;
}

void Particles::enablePointMass(glm::vec3 &ray, Camera &camera) {
    glm::vec3 planeNormal = -camera.getFacing();
    float planeOffset = 20.0f;
    float rayDistance = -(glm::dot(glm::vec3(0.0f,0.0f,0.0f),  planeNormal) + planeOffset) / glm::dot(ray, planeNormal);
    glm::vec3 point = (ray * rayDistance) + camera.getCameraPos();
    this->pointMassData[0] = point.x;
    this->pointMassData[1] = point.y;
    this->pointMassData[2] = point.z;
    this->pointMassData[3] = 1.0f;
    this->pointMassData[4] = DeltaTime::getDeltaTime();
    std::cout << this->pointMassData[4] << std::endl;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->pointMassSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, this->pointMassSSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * 5, this->pointMassData);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void Particles::disablePointMass() {
    this->pointMassData[0] = 0.0f;
    this->pointMassData[1] = 0.0f;
    this->pointMassData[2] = 0.0f;
    this->pointMassData[3] = 0.0f;
    this->pointMassData[4] = DeltaTime::getDeltaTime();
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->pointMassSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, this->pointMassSSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * 5, this->pointMassData);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
