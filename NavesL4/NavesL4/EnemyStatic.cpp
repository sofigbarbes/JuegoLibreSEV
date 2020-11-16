#include "EnemyStatic.h"
#include "math.h"

EnemyStatic::EnemyStatic(float x, float y, Game* game)
	: EnemyBase(x, y, game, "res/enemy_static.png") {
	state = game->stateMoving;

	aDying = new Animation("res/enemy_static.png", width, height,
		40, 32, 1, 1, false, game);

	aMoving = new Animation("res/enemy_static.png", width, height,
		40, 32, 1, 1, true, game);
	animation = aMoving;
	isShooter = true;
}

void EnemyStatic::update() {
	// Actualizar la animación
	bool endAnimation = animation->update();
	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}
	if (state == game->stateMoving) {
		animation = aMoving;
	}
	if (state == game->stateDying) {
		animation = aDying;
	}
	if (shootTime > 0) {
		shootTime--;
	}

}

void EnemyStatic::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}


void EnemyStatic::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}

ProjectilEnemigo* EnemyStatic::shoot(Player* player) {
	if (shootTime == 0) {
		shootTime = shootCadence;
		double h = sqrt(x * x + y * y);
		double k = 1;

		double n = h / k;

		float dx = (player->x - x) / n;
		float dy = (player->y - y) / n;

		/*if (dx > 10 || dy > 10) {
			return NULL;
		}*/
		n = h / 5;

		float x1 = x + 100 * dx;
		float y1 = y + 100 * dy;
		return new ProjectilEnemigo (x1, y1, dx, dy, game); //x,y,vx,vy,game

		/*
		int v1 = pow(player->x - x, 2);
		double modulo = sqrt( v1 + (player->y - y) ^ 2);
		//double modulo = 1;

		projectile->vx = (player->x - x) / modulo;
		projectile->vy = (player->y - y) / modulo;
		cout << "Vx: " << pow(projectile->vx , 2)<< " Vy: " << pow(projectile->vy,2) << endl;
		cout << "Suma: " << sqrt(pow(projectile->vx, 2) + pow(projectile->vy, 2)) << endl;*/
		//return projectile;

	}
	else {
		return NULL;
	}
}

