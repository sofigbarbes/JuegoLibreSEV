#pragma once

#include "EnemyBase.h"
#include "Player.h"
#include "Animation.h" 
#include "Projectile.h"

class EnemyStatic : public EnemyBase
{
public:
	EnemyStatic(float x, float y, Game* game);
	void draw(float scrollX = 0) override; // Va a sobrescribir
	ProjectilEnemigo* shoot(Player* player) override;
	void update() override;
	void impacted(); // Recibe impacto y pone animación de morir
	int shootTime = 30;
	int shootCadence = 30;
	
};
