#pragma once

#include "Actor.h"
#include "Animation.h" 

class EnemyBase : public Actor
{
public:
	EnemyBase(float x, float y, Game* game, string filename);
	void draw(float scrollX = 0) override; // Va a sobrescribir
	virtual void update();
	virtual void impacted(); // Recibe impacto y pone animación de morir
	float vxIntelligence;
	float vyIntelligence;
	int state;
	Animation* aDying;
	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada
};
