#ifndef SHADERS_H
#define SHADERS_H

#include "include/glad/glad.h"

typedef struct {
    GLuint programId;
    GLuint vertexShaderId;
    GLuint fragmentShaderId;
} ShaderProgram;

int createShaderProgram(ShaderProgram* program, const char* vertexShaderPath, const char* fragmentShaderPath);
void useShaderProgram(const ShaderProgram* program);

#endif
