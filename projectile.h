#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "include/glad/glad.h"
#include <math.h>

#define PROJECTILE_RADIUS 0.5f
#define PROJECTILE_SEGMENTS 100

struct Projectile {
    GLfloat vertices[PROJECTILE_SEGMENTS * 2];
};

//void createProjectile(GLuint* VAO, GLuint* VBO);
void createProjectile(GLuint* VAO, GLuint* VBO, struct Projectile* projectile);

#endif