#ifndef MAIN_H
#define MAIN_H

#define PROGRAM_NAME "Bubbles"
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 600

#include "projectile.h"
#include "cursor.h"

void errorCallback(int error, const char* description);
//void render();
void render(struct Projectile* projectile, struct Cursor* cursor);
int main();

#endif