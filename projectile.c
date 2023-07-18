#include "projectile.h"

void createProjectile(GLuint* VAO, GLuint* VBO, struct Projectile* projectile) {
    float angle = 2 * M_PI / PROJECTILE_SEGMENTS;
    for (int i = 0; i < PROJECTILE_SEGMENTS; ++i) {
        projectile->vertices[i * 2] = PROJECTILE_RADIUS * cos(i * angle);
        projectile->vertices[i * 2 + 1] = PROJECTILE_RADIUS * sin(i * angle);
    }

    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);

    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(projectile->vertices), projectile->vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}