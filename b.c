#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define CANNON_LENGTH 80
#define CANNON_WIDTH 20
#define PROJECTILE_RADIUS 25
#define PROJECTILE_SPEED 500.0f

// Cannon position and rotation angle
float cannonX = WINDOW_WIDTH / 2;
float cannonY = CANNON_WIDTH / 2;
float cannonRotation = 0.0f;

// Projectile position and velocity
float projectileX = -100.0f;
float projectileY = -100.0f;
float projectileVelocityX = 0.0f;
float projectileVelocityY = 0.0f;

// Flag indicating if the projectile is in motion
bool isProjectileMoving = false;

void drawCannon() {
    glPushMatrix();
    glTranslatef(cannonX, cannonY, 0.0f);
    glRotatef(cannonRotation, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);

    if (isProjectileMoving) {
        glColor3f(1.0f, 0.0f, 0.0f); //black
    }
    else {
        glColor3f(1.0f, 1.0f, 1.0f); //white
    }

    glVertex2f(0.0f, -CANNON_WIDTH / 2);
    glVertex2f(CANNON_LENGTH, -CANNON_WIDTH / 2);
    glVertex2f(CANNON_LENGTH, CANNON_WIDTH / 2);
    glVertex2f(0.0f, CANNON_WIDTH / 2);
    glEnd();
    glPopMatrix();
}

void drawCircle() {
    for (int i = 0; i <= 360; ++i) {
        float angle = i * M_PI / 180.0f;
        float x = PROJECTILE_RADIUS * cos(angle);
        float y = PROJECTILE_RADIUS * sin(angle);
        glVertex2f(x, y);
    }
}

void drawProjectile() {
    if (isProjectileMoving) {
        glPushMatrix();
        glTranslatef(projectileX, projectileY, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircle();
        glEnd();
        glPopMatrix();
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !isProjectileMoving) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        float mouseX = (float)xpos;
        float mouseY = (float)(WINDOW_HEIGHT - ypos); // Invert y-axis
        isProjectileMoving = true;

        // Calculate direction vector from cannon to cursor position
        float dx = mouseX - cannonX;
        float dy = mouseY - cannonY;
        float distance = sqrt(dx * dx + dy * dy);

        // Normalize the direction vector and set projectile velocity
        if (distance > 0.0f) {
            dx /= distance;
            dy /= distance;
            projectileVelocityX = dx * PROJECTILE_SPEED;
            projectileVelocityY = dy * PROJECTILE_SPEED;
        }

        // Set initial projectile position
        projectileX = cannonX;
        projectileY = cannonY;
    }
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    float mouseX = (float)xpos;
    float mouseY = (float)(WINDOW_HEIGHT - ypos); // Invert y-axis

    // Calculate rotation angle based on cursor position
    float dx = mouseX - cannonX;
    float dy = mouseY - cannonY;
    cannonRotation = atan2(dy, dx) * 180.0f / M_PI;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Cannon Game", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Set the viewport and orthographic projection
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);

    // Set mouse button callback
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // Set cursor position callback
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Clear the buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the cannon
        drawCannon();

        // Update the projectile position if it's in motion
        if (isProjectileMoving) {
            float deltaTime = glfwGetTime(); // Get elapsed time

            projectileX += projectileVelocityX * deltaTime;
            projectileY += projectileVelocityY * deltaTime;

            // Check if the projectile is off-screen
            //Left side
            if (projectileX - PROJECTILE_RADIUS < 0) {
                projectileX = PROJECTILE_RADIUS;
                projectileVelocityX *= -1;
            }
            //Right Side
            if (projectileX + PROJECTILE_RADIUS > WINDOW_WIDTH) {
                projectileX = WINDOW_WIDTH - PROJECTILE_RADIUS;
                projectileVelocityX *= -1;
            }

            if (projectileY - PROJECTILE_RADIUS < 0) {
                projectileY = PROJECTILE_RADIUS;
                //projectileVelocityY *= -1;
                isProjectileMoving = false;
                projectileX = -100.0f;
                projectileY = -100.0f;
            }
            if (projectileY + PROJECTILE_RADIUS > WINDOW_HEIGHT) {
                projectileY = WINDOW_HEIGHT - PROJECTILE_RADIUS;
                //projectileVelocityY *= -1;
                isProjectileMoving = false;
                projectileX = -100.0f;
                projectileY = -100.0f;
            }

            glfwSetTime(0.0); // Reset the elapsed time
        }

        // Draw the projectile
        drawProjectile();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();
    return 0;
}
