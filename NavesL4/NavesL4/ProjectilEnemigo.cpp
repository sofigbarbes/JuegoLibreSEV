#include "ProjectilEnemigo.h"

ProjectilEnemigo::ProjectilEnemigo(float x, float y, float dx, float dy, Game* game) :
	Actor("res/projectil_enemigo_static.png", x, y, 20, 20, game) {
    vx = 9 * dx;
    vy = 9 * dy;
}

void ProjectilEnemigo::update() {
    x = x + vx;
    y = y + vy;
}


