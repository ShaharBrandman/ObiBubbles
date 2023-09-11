#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define CANNON_LENGTH 80
#define CANNON_WIDTH 20

#define PROJECTILE_RADIUS 15
#define PROJECTILE_SPEED 500.0f

#define MATRIX_ROWS 10
#define MATRIX_COLS 17
#define BUBBLE_RADIUS PROJECTILE_RADIUS
#define BUBBLE_MARGIN 5

#define BM_START_X ((WINDOW_WIDTH - (MATRIX_COLS * ((BUBBLE_RADIUS * 2) + BUBBLE_MARGIN))) / 2)
#define BM_START_Y (WINDOW_HEIGHT - (BUBBLE_RADIUS * 2))

#define BUBBLE_STACK_START_PADDING 80
#define BUBBLE_STACK_MARGIN 35
#define BUBBLE_STACK_START_X ( WINDOW_WIDTH / 2 ) + BUBBLE_STACK_START_PADDING
#define BUBBLE_STACK_START_Y ( CANNON_WIDTH / 2) + 10
#define BUBBLE_STACK_RADIUS PROJECTILE_RADIUS
#define BUBBLE_STACK_LENGTH 5

#define COLORS_LENGTH 6

// Cannon position and rotation angle
float cannonX = WINDOW_WIDTH / 2;
float cannonY = CANNON_WIDTH / 2;
float cannonRotation = 0.0f;

// Projectile position and velocity
float projectileX = -100.0f;
float projectileY = -100.0f;
float projectileVelocityX = 0.0f;
float projectileVelocityY = 0.0f;
float projectileColor[3];

// Flag indicating if the projectile is in motion
bool isProjectileMoving = false;

typedef struct {
    float x;
    float y;
    bool isPopped;
    float color[3];
} Bubble;

float colors[COLORS_LENGTH][3] = {
    { 1.0f, 0.0f, 0.0f }, //red
    { 0.0f, 1.0f, 0.0f }, //green
    { 0.0f, 0.0f, 1.0f }, //blue
    { 1.0f, 1.0f, 0.0f}, //yellow
    { 1.0f, 0.2f, 0.7f }, //pink
    { 0.3f, 0.9f, 1.0f } //light blue
};

float bubbleStack[BUBBLE_STACK_LENGTH][3]; //R, G, B

int bubbleStackIndex = 0;

Bubble bubbleMatrix[MATRIX_ROWS][MATRIX_COLS];

void InitializeBubbleStack() {
    bubbleStackIndex = 0;
    srand(time(NULL));

    for(int i=0; i<BUBBLE_STACK_LENGTH; i++) {
        int colorIndex = rand() % COLORS_LENGTH;

        bubbleStack[i][0] = colors[colorIndex][0];
        bubbleStack[i][1] = colors[colorIndex][1];
        bubbleStack[i][2] = colors[colorIndex][2];
    }
}

void initializeBubbleMatrix() {
    for (int i = 0; i < MATRIX_ROWS; ++i) {
        for (int j = 0; j < MATRIX_COLS; ++j) {
            bubbleMatrix[i][j].x = BM_START_X + (BUBBLE_RADIUS * 2 + BUBBLE_MARGIN) * j + BUBBLE_RADIUS;
            bubbleMatrix[i][j].y = BM_START_Y - (BUBBLE_RADIUS * 2 + BUBBLE_MARGIN) * i;
            bubbleMatrix[i][j].isPopped = false;

            int colorIndex = rand() % COLORS_LENGTH;
            bubbleMatrix[i][j].color[0] = colors[colorIndex][0];
            bubbleMatrix[i][j].color[1] = colors[colorIndex][1];
            bubbleMatrix[i][j].color[2] = colors[colorIndex][2];
        }
    }
}

void drawBubble(int x, int y, int r, float color[3]) {
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(color[0], color[1], color[2]);
    for (int i = 0; i <= 360; ++i) {
        float angle = i * M_PI / 180.0f;
        float cx = x + r * cos(angle);
        float cy = y + r * sin(angle);
        glVertex2f(cx, cy);
    }
    glEnd();
}

void drawBubbleMatrix() {
    for (int i = 0; i < MATRIX_ROWS; ++i) {
        for (int j = 0; j < MATRIX_COLS; ++j) {
            if (!bubbleMatrix[i][j].isPopped) {
                glPushMatrix();
                glTranslatef(bubbleMatrix[i][j].x, bubbleMatrix[i][j].y, 0.0f);
                drawBubble(0.0f, 0.0f, BUBBLE_RADIUS, bubbleMatrix[i][j].color);
                drawCircle();
                glPopMatrix();
            }
        }
    }
}

void popBubble(float x, float y) {
    for (int i = 0; i < MATRIX_ROWS; ++i) {
        for (int j = 0; j < MATRIX_COLS; ++j) {
            if (!bubbleMatrix[i][j].isPopped) {
                float dx = x - bubbleMatrix[i][j].x;
                float dy = y - bubbleMatrix[i][j].y;
                float distanceSquared = dx * dx + dy * dy;

                if (distanceSquared <= BUBBLE_RADIUS * BUBBLE_RADIUS) {
                    bubbleMatrix[i][j].isPopped = true;
                    return;
                }
            }
        }
    }
}

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
        glColor3f(
            bubbleStack[bubbleStackIndex][0],
            bubbleStack[bubbleStackIndex][1],
            bubbleStack[bubbleStackIndex][2]
        );
        drawCircle();
        glEnd();
        glPopMatrix();
    }
}

void drawBubbleStack() {
    for(int i=bubbleStackIndex; i<BUBBLE_STACK_LENGTH; i++) {
        drawBubble(BUBBLE_STACK_START_X + (i * BUBBLE_STACK_MARGIN), BUBBLE_STACK_START_Y, BUBBLE_RADIUS, bubbleStack[i]);
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

        //move on to the next bubble
        if(bubbleStackIndex >= BUBBLE_STACK_LENGTH) {
            InitializeBubbleStack();
        }
        else {
            bubbleStackIndex++;
        }
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

void checkForCollision() {
    for (int i = 0; i < MATRIX_ROWS; ++i) {
        for (int j = 0; j < MATRIX_COLS; ++j) {
            if(!bubbleMatrix[i][j].isPopped) {
                float dx = projectileX - bubbleMatrix[i][j].x;
                float dy = projectileY - bubbleMatrix[i][j].y;
                float distance = sqrt( (dx * dx) + (dy * dy) );

                if (distance <= PROJECTILE_RADIUS + BUBBLE_RADIUS) {
                    popBubble(bubbleMatrix[i][j].x, bubbleMatrix[i][j].y);
                }
            }
        }
    }
}

void render() {
    // Draw the cannon
    drawCannon();

    //draw bubbles matrix
    drawBubbleMatrix();

    //draw bubble stack
    drawBubbleStack();

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

        //below the screen
        if (projectileY - PROJECTILE_RADIUS < 0) {
            projectileY = PROJECTILE_RADIUS;
            //projectileVelocityY *= -1;
            isProjectileMoving = false;
            projectileX = -100.0f;
            projectileY = -100.0f;
        }

        //above the screen
        if (projectileY + PROJECTILE_RADIUS > WINDOW_HEIGHT) {
            projectileY = WINDOW_HEIGHT - PROJECTILE_RADIUS;
            //projectileVelocityY *= -1;
            isProjectileMoving = false;
            projectileX = -100.0f;
            projectileY = -100.0f;
        }

        //collision for other bubbles
        checkForCollision();

        glfwSetTime(0.0); // Reset the elapsed time
    }

    // Draw the projectile
    drawProjectile();
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Obi Bubbles", NULL, NULL);
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

    InitializeBubbleStack();

    initializeBubbleMatrix();

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Clear the buffer
        glClear(GL_COLOR_BUFFER_BIT);

        render();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();
    return 0;
}
