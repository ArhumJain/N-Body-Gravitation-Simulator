#ifndef SHADER_H
#define SHADER_H

#pragma once
#include <string>

class Shader {
private:
    unsigned int ID;
public:

    Shader(const char* vertexPath, const char* fragmentPath);

    void use() const;
    void setUniformInt(const std::string &name, int value) const;
    void setUniformBool(const std::string &name, bool value) const;
    void setUniformFloat(const std::string &name, float value) const;

    unsigned int getID() const;
};
#endif
