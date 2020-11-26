#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 35, 35, game) {

	onAir = false;
	orientation = game->orientationUp;
	state = game->stateMoving;
	audioShoot = new Audio("res/piu.wav", false);
	aShootingRight = new Animation("res/jugador__corriendo_derecha.png",
		width, height, 160, 40, 6, 4, false, game);
	aShootingLeft = new Animation("res/jugador__corriendo_izquierda.png",
		width, height, 160, 40, 6, 4, false, game);

	aJumpingRight = new Animation("res/jugador_saltando_derecha.png",
		width, height, 160, 40, 6, 4, true, game);
	aJumpingLeft = new Animation("res/jugador_saltando_izquierda.png",
		width, height, 160, 40, 6, 4, true, game);
	aIdleRight = new Animation("res/jugador__idle_derecha.png", width, height,
		164, 40, 6, 4, true, game);
	aIdleLeft = new Animation("res/jugador__idle_izquierda.png", width, height,
		164, 40, 6, 4, true, game);
	aIdleUp = new Animation("res/jugador__idle_arriba.png", width, height,
		164, 40, 6, 4, true, game);
	aIdleDown = new Animation("res/jugador__idle_abajo.png", width, height,
		164, 40, 6, 4, true, game);
	aRunningRight = new Animation("res/jugador__corriendo_derecha.png", width, height,
		164, 40, 4, 4, true, game);
	aRunningLeft = new Animation("res/jugador__corriendo_izquierda.png", width, height,
		164, 40, 4, 4, true, game);
	aRunningUp = new Animation("res/jugador__corriendo_arriba.png", width, height,
		164, 40, 4, 4, true, game);
	aRunningDown = new Animation("res/jugador__corriendo_abajo.png", width, height,
		164, 40, 4, 4, true, game);
	aEscudo = new Animation("res/animacion_escudo.png", width, height,
		164, 40, 6, 4, true, game);
	animation = aIdleRight;

}


void Player::update() {
	// En el aire y moviéndose, PASA a estar saltando
	if (hasEscudo) {
		aEscudo->update();
	}

	if (invulnerableTime > 0) {
		invulnerableTime--;
	}
	bool endAnimation = animation->update();

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba disparando
		if (state == game->stateShooting) {
			state = game->stateMoving;
		}
	}
	// Establecer orientación
	if (vx > 0) {
		orientation = game->orientationRight;
	}
	if (vx < 0) {
		orientation = game->orientationLeft;
	}
	if (vx == 0 && vy < 0) {
		orientation = game->orientationUp;
	}
	if (vx == 0 && vy > 0) {
		orientation = game->orientationDown;
	}

	// Selección de animación basada en estados

	if (state == game->stateMoving) {
		//cout << "vx: " << vx << "   vy: " << vy << endl;

		if (vx != 0) {
			if (orientation == game->orientationRight) {
				animation = aRunningRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aRunningLeft;
			}
		}
		if (vx == 0) {
			if (vy != 0) {
				if (orientation == game->orientationUp) {
					animation = aRunningUp;
				}if (orientation == game->orientationDown) {
					animation = aRunningDown;
				}
			}
			else {
				if (orientation == game->orientationRight) {
					animation = aIdleRight;
				}
				if (orientation == game->orientationLeft) {
					animation = aIdleLeft;
				}
				if (orientation == game->orientationUp) {
					animation = aIdleUp;
				}
				if (orientation == game->orientationDown) {
					animation = aIdleDown;
				}
			}
		}
	}

	if (shootTime > 0) {
		shootTime--;
	}
}

void Player::moveX(float axis) {
	vx = axis * 5;
}

void Player::moveY(float axis) {
	vy = axis * 5;
}

Projectile* Player::shoot() {

	if (shootTime == 0) {
		audioShoot->play();
		aShootingLeft->currentFrame = 0; //"Rebobinar" aniamción
		aShootingRight->currentFrame = 0; //"Rebobinar" aniamción
		shootTime = shootCadence;
		Projectile* projectile = new Projectile(x, y, game);
		if (orientation == game->orientationLeft) {
			projectile->vx = projectile->vx - 9;
			projectile->vy = vy;

		}
		else if (orientation == game->orientationRight) {
			projectile->vx = projectile->vx + 9;
			projectile->vy = vy;
		}
		else if (orientation == game->orientationDown) {
			projectile->vy = projectile->vy + 9;
			projectile->vx = vx;
		}
		else if (orientation == game->orientationUp) {
			projectile->vy = projectile->vy - 9;
			projectile->vx = vx;
		}
		return projectile;
	}
	else {
		return NULL;
	}
}

void Player::draw(float scrollX) {
	if (hasEscudo) {
		aEscudo->draw(x - scrollX, y);
	}
	if (invulnerableTime == 0) {
		animation->draw(x - scrollX, y);
	}
	else {
		if (invulnerableTime % 10 >= 0 && invulnerableTime % 10 <= 5) {
			animation->draw(x - scrollX, y);
		}
	}
}

void Player::jump() {
	if (!onAir) {
		vy = -16;
		onAir = true;
	}
}

void Player::loseLife() {
	if (invulnerableTime <= 0) {
		if (lifes > 0) {
			lifes--;
			invulnerableTime = 80;
		}
	}
}
