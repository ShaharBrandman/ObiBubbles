#include "cursor.h"

#include "main.h"

struct Cursor cursor = { 0.0f, 0.0f };

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    cursor.x = (float)(2.0 * xpos / WINDOW_WIDTH - 1.0);
    cursor.y = (float)(2.0 * ypos / WINDOW_HEIGHT - 1.0);
}