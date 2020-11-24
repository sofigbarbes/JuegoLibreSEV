#pragma once
#include "Actor.h"

class TileDestructible : public Actor
{
public:
	TileDestructible(string filename, float x, float y, Game* game);
	int lifes;
	void impacted();
};

