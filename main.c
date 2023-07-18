/*
 * main.c - the entry point of the program
 * Author: Shahar Brandman
 */
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "main.h"
#include "resize_window.h"
#include "shaders.h"
#include "projectile.h"
#include "cursor.h"

GLuint VAO, VBO, shaderProgram;

struct Projectile projectile;

void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void render(struct Projectile* projectile, struct Cursor* cursor) {
    createProjectile(&VAO, &VBO, projectile);

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    GLuint translation = glGetUniformLocation(shaderProgram, "translation");
    glUniform2f(translation, cursor->x, -cursor->y);

    glDrawArrays(GL_TRIANGLE_FAN, 0, PROJECTILE_SEGMENTS);

    glBindVertexArray(0);
    glUseProgram(0);
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "failed to initalize glfw");
        return -1;
    }

    //setting up OpenGL (Core profile v3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //set up glfw error callback
    glfwSetErrorCallback(errorCallback);

    //creating window
    GLFWwindow* window = glfwCreateWindow(
        WINDOW_HEIGHT,
        WINDOW_WIDTH,
        PROGRAM_NAME,
        NULL,
        NULL
    );

    if (!window) {
        printf("Could not create Window");
        return -1;
    }

    //setting up window context and callbacks
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Could not initialize GLAD");
        return -1;
    }

    glViewport(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

    //setting up shaders
    if (!createShaderProgram(&shaderProgram, "shaders/vertexShader.vert", "shaders/fragmentShader.frag")) {
        printf("Failed to create shader program");
        return -1;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);

        render(&projectile, &cursor);

        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
