#include "EnemyStatic.h"

EnemyStatic::EnemyStatic(float x, float y, Game* game)
	: EnemyBase(x, y, game, "res/enemy_static.png") {
	state = game->stateMoving;

	aDying = new Animation("res/enemy_static.png", width, height,
		40, 32, 1, 1, false, game);

	aMoving = new Animation("res/enemy_static.png", width, height,
		40, 32, 1, 1, true, game);
	animation = aMoving;
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

}

void EnemyStatic::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}


void EnemyStatic::draw(float scrollX) {
	animation->draw(x - scrollX, y);
}


