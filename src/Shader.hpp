#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // shader program
    GLuint ID;
    // constructor
    Shader(const char *VertexPath, const char *FragmentPath);
    // activate the shader program
    void useID();
};
#endif
