/*
 * main.c - the entry point of the program
 * Author: Shahar Brandman
 */
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "resize_window.h"
#include "shaders.h"

#define PROGRAM_NAME "Bubbles"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 600

#define NUM_SEGMENTS 360

float angle = 2 * M_PI / NUM_SEGMENTS;

float circleVertices[(NUM_SEGMENTS + 1) * 2]; //x, y

void createCircle() {
    for (int i = 0; i <= NUM_SEGMENTS; ++i) {
        float x = cos(i * angle);
        float y = sin(i * angle);
        circleVertices[i * 2] = x;
        circleVertices[i * 2 + 1] = y;
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Could not initialize GLAD");
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    ShaderProgram shaderProgram;
    if (!createShaderProgram(&shaderProgram, "shaders/vertexShader.vert", "shaders/fragmentShader.frag")) {
        printf("Failed to create shader program");
        return -1;
    }

    createCircle();

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        2, 
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);

        //glClearColor(0.4f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        useShaderProgram(&shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_SEGMENTS + 1);
        glBindVertexArray(0);

        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram.programId);

    glfwTerminate();
    return 0;
}
