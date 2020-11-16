#pragma once

#include "Actor.h"


class ProjectilEnemigo : public Actor
{
public:
	ProjectilEnemigo(float x, float y, float dx, float dy, Game* game);
	void update();
};

