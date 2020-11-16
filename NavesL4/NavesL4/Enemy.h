#pragma once

#include "EnemyBase.h"
#include "Animation.h" 

class Enemy : public EnemyBase
{
public:
	Enemy(float x, float y, Game* game);
	void draw(float scrollX = 0) override; // Va a sobrescribir
	void update() override;
	void impacted() override; // Recibe impacto y pone animación de morir
	ProjectilEnemigo* shoot(Player* player) override;
	float vxIntelligence;
	float vyIntelligence;
	int state;
	Animation* aDying;
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada
};
