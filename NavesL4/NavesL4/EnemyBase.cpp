#include "EnemyBase.h"

EnemyBase::EnemyBase(float x, float y, Game* game, string filename)
	: Actor(filename, x, y, 36, 40, game) {
	aExplosion = new Animation("res/explosion.png", width, height,
		120, 40, 10, 3, false, game);
}

void EnemyBase::draw(float scrollX) {
}

void EnemyBase::update()
{
	
}

ProjectilEnemigo* EnemyBase::shoot(Player* player)
{
	return nullptr;
}

void EnemyBase::impacted()
{
	cout << "Inside enemyBase impacted" << endl;
}


