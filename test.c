#include <GL/glut.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

// Window dimensions
int windowWidth = 800;
int windowHeight = 600;

// Number of rows and columns of bubbles
int numRows = 10;
int numColumns = 17;

// Bubble radius and gap between bubbles
float bubbleRadius = 20.0f;
float bubbleGap = 5.0f;

// Bubble colors
typedef enum {
    COLOR_PINK,
    COLOR_GREEN,
    COLOR_RED,
    COLOR_BLUE,
    COLOR_YELLOW,
    NUM_COLORS
} BubbleColor;

// Structure to represent a bubble
typedef struct {
    float x;
    float y;
    BubbleColor color;
    bool popped;
} Bubble;

// Array to store the bubbles
Bubble bubbles[10][17];

// Cannon attributes
float cannonPosX;
float cannonPosY;
float cannonAngle = 45.0f;

// Active bubble attributes
bool activeBubbleExists = false;
float activeBubbleX;
float activeBubbleY;
BubbleColor activeBubbleColor;

// Bubble stack attributes
BubbleColor bubbleStack[5];
int stackIndex = -1;

int headIndex = 0;

// Movement attributes
float activeBubbleSpeed = 1.5f;
float activeBubbleDirectionX;
float activeBubbleDirectionY;

// Function to initialize the bubbles
void initBubbles() {
    float startX = bubbleRadius + bubbleGap;
    float startY = windowHeight - (bubbleRadius + bubbleGap);

    float currX = startX;
    float currY = startY;

    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numColumns; j++) {
            BubbleColor randomColor = rand() % NUM_COLORS;
            bubbles[i][j].x = currX;
            bubbles[i][j].y = currY;
            bubbles[i][j].color = randomColor;
            bubbles[i][j].popped = false;

            currX += (2 * bubbleRadius + bubbleGap);
        }

        currX = startX;
        currY -= (2 * bubbleRadius + bubbleGap);
    }
}

// Function to render the bubbles
void renderBubbles() {
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numColumns; j++) {
            if (!bubbles[i][j].popped) {
                glPushMatrix();
                glTranslatef(bubbles[i][j].x, bubbles[i][j].y, 0.0f);

                // Set bubble color based on its color attribute
                switch (bubbles[i][j].color) {
                    case COLOR_PINK:
                        glColor3f(1.0f, 0.0f, 1.0f);
                        break;
                    case COLOR_GREEN:
                        glColor3f(0.0f, 1.0f, 0.0f);
                        break;
                    case COLOR_RED:
                        glColor3f(1.0f, 0.0f, 0.0f);
                        break;
                    case COLOR_BLUE:
                        glColor3f(0.0f, 0.0f, 1.0f);
                        break;
                    case COLOR_YELLOW:
                        glColor3f(1.0f, 1.0f, 0.0f);
                        break;
                }

                // Render the bubble as a circle
                glBegin(GL_TRIANGLE_FAN);
                glVertex2f(0.0f, 0.0f);
                for (int k = 0; k <= 360; k += 10) {
                    float angle = k * 3.14159f / 180.0f;
                    float x = bubbleRadius * cos(angle);
                    float y = bubbleRadius * sin(angle);
                    glVertex2f(x, y);
                }
                glEnd();

                glPopMatrix();
            }
        }
    }

    // Render the active bubble
    if (activeBubbleExists) {
        glPushMatrix();
        glTranslatef(activeBubbleX, activeBubbleY, 0.0f);

        // Set active bubble color
        switch (activeBubbleColor) {
            case COLOR_PINK:
                glColor3f(1.0f, 0.0f, 1.0f);
                break;
            case COLOR_GREEN:
                glColor3f(0.0f, 1.0f, 0.0f);
                break;
            case COLOR_RED:
                glColor3f(1.0f, 0.0f, 0.0f);
                break;
            case COLOR_BLUE:
                glColor3f(0.0f, 0.0f, 1.0f);
                break;
            case COLOR_YELLOW:
                glColor3f(1.0f, 1.0f, 0.0f);
                break;
        }

        // Render the active bubble as a circle
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(0.0f, 0.0f);
        for (int k = 0; k <= 360; k += 10) {
            float angle = k * 3.14159f / 180.0f;
            float x = bubbleRadius * cos(angle);
            float y = bubbleRadius * sin(angle);
            glVertex2f(x, y);
        }
        glEnd();

        glPopMatrix();
    }
}

// Function to render the cannon
void renderCannon() {
    glPushMatrix();
    glTranslatef(cannonPosX, cannonPosY, 0.0f);
    glRotatef(cannonAngle, 0.0f, 0.0f, 1.0f);

    if (activeBubbleExists) {
        switch (activeBubbleColor) {
            case COLOR_PINK:
                glColor3f(1.0f, 0.0f, 1.0f);
                break;
            case COLOR_GREEN:
                glColor3f(0.0f, 1.0f, 0.0f);
                break;
            case COLOR_RED:
                glColor3f(1.0f, 0.0f, 0.0f);
                break;
            case COLOR_BLUE:
                glColor3f(0.0f, 0.0f, 1.0f);
                break;
            case COLOR_YELLOW:
                glColor3f(1.0f, 1.0f, 0.0f);
                break;
        }
    } else {
        glColor3f(1.0f, 1.0f, 1.0f);  // Set cannon color to white
    }

    // Draw the cannon
    glBegin(GL_QUADS);
    glVertex2f(-10.0f, 0.0f);
    glVertex2f(-10.0f, bubbleRadius * 2.5f);
    glVertex2f(10.0f, bubbleRadius * 2.5f);
    glVertex2f(10.0f, 0.0f);
    glEnd();

    glPopMatrix();
}

// Function to handle window resizing
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
}

// Function to handle rendering
void render() {
    glClear(GL_COLOR_BUFFER_BIT);
    // Render the bubbles
    renderBubbles();
    renderCannon();
    glFlush();
}

// Function to update the active bubble position
void updateActiveBubblePosition(int value) {
    if (activeBubbleExists) {
        activeBubbleX += activeBubbleDirectionX * activeBubbleSpeed;
        activeBubbleY += activeBubbleDirectionY * activeBubbleSpeed;

        // Check for collision with the top wall
        if (activeBubbleY >= windowHeight - bubbleRadius) {
            // Add the active bubble to the grid
            int gridX = (int)(activeBubbleX / (2 * bubbleRadius + bubbleGap));
            int gridY = (int)(activeBubbleY / (2 * bubbleRadius + bubbleGap));

            if (gridX >= 0 && gridX < numColumns && gridY >= 0 && gridY < numRows) {
                bubbles[gridY][gridX].x = activeBubbleX;
                bubbles[gridY][gridX].y = activeBubbleY;
                bubbles[gridY][gridX].color = activeBubbleColor;
                bubbles[gridY][gridX].popped = false;
            }

            // Reset active bubble attributes
            activeBubbleExists = false;
            activeBubbleX = 0.0f;
            activeBubbleY = 0.0f;
            activeBubbleColor = COLOR_PINK;
        }

        // Check for collision with other bubbles
        int gridX = (int)(activeBubbleX / (2 * bubbleRadius + bubbleGap));
        int gridY = (int)(activeBubbleY / (2 * bubbleRadius + bubbleGap));

        if (gridX >= 0 && gridX < numColumns && gridY >= 0 && gridY < numRows) {
            if (!bubbles[gridY][gridX].popped) {
                // Collision with a bubble, so stop the active bubble's movement
                activeBubbleExists = false;
                activeBubbleX = bubbles[gridY][gridX].x;
                activeBubbleY = bubbles[gridY][gridX].y;
                activeBubbleColor = bubbles[gridY][gridX].color;
                bubbles[gridY][gridX].popped = true;
            }
        }

        // Check for collision with the window boundaries
        if (activeBubbleX <= bubbleRadius || activeBubbleX >= windowWidth - bubbleRadius) {
            activeBubbleDirectionX = -activeBubbleDirectionX;  // Invert the X direction to bounce back
        }

        // Check if the active bubble is below the cannon
        if (activeBubbleY <= cannonPosY + bubbleRadius) {
            // Destroy the active bubble
            activeBubbleExists = false;
            activeBubbleX = 0.0f;
            activeBubbleY = 0.0f;
            activeBubbleColor = COLOR_PINK;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(10, updateActiveBubblePosition, 0);
}

// Function to handle mouse movement
void mouseMove(int x, int y) {
    float dx = x - cannonPosX;
    float dy = windowHeight - y - cannonPosY;
    cannonAngle = atan2(dy, dx) * 180.0f / 3.14159f;
    cannonAngle = fmod(cannonAngle + 360.0f, 360.0f);

    cannonAngle = cannonAngle - 90.0f;

    glutPostRedisplay();
}

// Function to handle mouse clicks
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !activeBubbleExists) {
        // Shoot a random color bubble
        BubbleColor randomColor = rand() % NUM_COLORS;

        // Calculate the direction of the shot
        float dx = cos(cannonAngle * 3.14159f / 180.0f);
        float dy = sin(cannonAngle * 3.14159f / 180.0f);

        // Calculate the initial position of the shot bubble
        float startX = cannonPosX + dx * (bubbleRadius + bubbleGap);
        float startY = cannonPosY + dy * (bubbleRadius + bubbleGap);

        // Set the active bubble attributes
        activeBubbleExists = true;
        activeBubbleX = startX;
        activeBubbleY = startY;
        activeBubbleColor = randomColor;
        activeBubbleDirectionX = dx;
        activeBubbleDirectionY = dy;

        glutPostRedisplay();
    }
}

// Main function
int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Bubble Shooter");

    // Set the callback functions
    glutReshapeFunc(reshape);
    glutDisplayFunc(render);

    glutPassiveMotionFunc(mouseMove);
    glutMouseFunc(mouseClick);

    // Set the background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Initialize the bubbles
    initBubbles();

    // Initialize the cannon position
    cannonPosX = windowWidth / 2.0f;
    cannonPosY = bubbleRadius;

    // Set the timer function to update the active bubble position
    glutTimerFunc(10, updateActiveBubblePosition, 0);

    // Enter the main loop
    glutMainLoop();

    return 0;
}