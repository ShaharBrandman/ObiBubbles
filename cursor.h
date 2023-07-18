#ifndef CURSOR_H
#define CURSOR_H

#include "include/glad/glad.h"
#include "main.h"
#include <GLFW/glfw3.h>

struct Cursor {
    float x;
    float y;
};

extern struct Cursor cursor;

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

#endif