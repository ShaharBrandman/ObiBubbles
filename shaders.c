#include <stdio.h>
#include <stdlib.h>

#include "shaders.h"

char* readFileContent(const char* path) {
    FILE* file = fopen(path, "r");

    if (file == NULL) {
        printf("Couldn't open file");
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* fileContents = (char*)malloc(fileSize + 1);
    if (fileContents == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    fread(fileContents, fileSize, 1, file);
    fileContents[fileSize] = '\0';

    fclose(file);

    return fileContents;
}

int compileShader(GLuint shaderId, const char* shaderSource) {
    glShaderSource(shaderId, 1, &shaderSource, NULL);
    glCompileShader(shaderId);

    GLint success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLint logSize;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logSize);

        char* logMessage = (char*)malloc(logSize);
        glGetShaderInfoLog(shaderId, logSize, NULL, logMessage);

        printf("Failed to compile shader:\n%s\n", logMessage);

        free(logMessage);
        return 0;
    }

    return 1;
}

int createShaderProgram(ShaderProgram* program, const char* vertexShaderPath, const char* fragmentShaderPath) {
    char* vertexShaderSource = readFileContent(vertexShaderPath);
    char* fragmentShaderSource = readFileContent(fragmentShaderPath);

    if (vertexShaderSource == NULL || fragmentShaderSource == NULL) {
        printf("Failed to read shader source\n");
        return 0;
    }

    program->vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    program->fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    if (!compileShader(program->vertexShaderId, vertexShaderSource) ||
        !compileShader(program->fragmentShaderId, fragmentShaderSource)) {
        printf("Failed to compile shaders\n");
        free(vertexShaderSource);
        free(fragmentShaderSource);
        return 0;
    }

    program->programId = glCreateProgram();
    glAttachShader(program->programId, program->vertexShaderId);
    glAttachShader(program->programId, program->fragmentShaderId);
    glLinkProgram(program->programId);

    GLint success;
    glGetProgramiv(program->programId, GL_LINK_STATUS, &success);

    if (!success) {
        printf("Failed to link shader program\n");
        free(vertexShaderSource);
        free(fragmentShaderSource);
        return 0;
    }

    free(vertexShaderSource);
    free(fragmentShaderSource);
    return 1;
}

void useShaderProgram(const ShaderProgram* program) {
    glUseProgram(program->programId);
}