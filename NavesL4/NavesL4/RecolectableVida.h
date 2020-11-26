#pragma once
#include "Actor.h"
#include "Animation.h"

class RecolectableVida : public Actor
{
public:
	RecolectableVida(float x, float y, Game* game);
	Animation* animation; // Referencia a la animación mostrada

};
