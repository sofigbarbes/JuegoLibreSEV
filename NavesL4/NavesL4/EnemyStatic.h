#pragma once

#include "EnemyBase.h"
#include "Animation.h" 

class EnemyStatic : public EnemyBase
{
public:
	EnemyStatic(float x, float y, Game* game);
	void draw(float scrollX = 0) override; // Va a sobrescribir
	void update() override;
	void impacted(); // Recibe impacto y pone animación de morir
	int state;
};
