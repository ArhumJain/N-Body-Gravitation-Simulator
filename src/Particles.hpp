#pragma once
#include "Camera.hpp"
#include "glm/glm.hpp"
class Particles {
private:
    unsigned int count;

    unsigned int VAO[2];
    unsigned int VBO[2];
    unsigned int TFBO[2];
    unsigned int pointsSSBO;
    unsigned int pointMassSSBO;

    
    float pointMassData[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float *particles;
    float *particlesAlternate;
    float *transformedParticles;
    bool firstRender;
    bool currBuffer;
    

public:
    Particles(unsigned long c, unsigned int program, Camera &camera);
    ~Particles();
    
    void render();
    void enablePointMass(glm::vec3 &ray, Camera &camera);
    void disablePointMass();
};