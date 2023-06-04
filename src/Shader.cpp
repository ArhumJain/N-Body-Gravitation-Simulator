#include "Shader.hpp"
#include "glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string rawVertexCode;
    std::string rawFragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;

    try {
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);
        std::stringstream vertexShaderStream, fragmentShaderStream;

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        rawVertexCode = vertexShaderStream.str();
        rawFragmentCode = fragmentShaderStream.str();

    } catch (std::ifstream::failure e) {
        std::cout << "ERROR: SHADER FILE NOT SUCCESFULLY READ" << std::endl;
    }
    
    const char *vertexShaderCode = rawVertexCode.c_str();
    const char *fragmentShaderCode = rawFragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR: VERTEX SHADER COMPILATION FAILED:\n" << infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "ERROR: FRAGMENT SHADER COMPILATION FAILED:\n" << infoLog << std::endl;
    }

    this->ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    const char *feedbackValues[3] = {"pos", "vel", "acc"};
    // const char *feedbackValues[9] = {"posX", "posY", "posZ", "velX", "velY", "velZ", "accX", "accY", "accZ"};
    glTransformFeedbackVaryings(ID, 3, feedbackValues, GL_INTERLEAVED_ATTRIBS);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "ERROR: PROGRAM LINKING FAILED:\n" << infoLog << std::endl;
    }
}

void Shader::use() const {
    glUseProgram(this->ID);
}

void Shader::setUniformBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}
void Shader::setUniformInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value); 
}
void Shader::setUniformFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value); 
} 

unsigned int Shader::getID() const {
    return this->ID;
}
