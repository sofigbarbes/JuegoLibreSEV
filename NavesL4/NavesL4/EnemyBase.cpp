#include "EnemyBase.h"

EnemyBase::EnemyBase(float x, float y, Game* game, string filename)
	: Actor(filename, x, y, 36, 40, game) {

}

void EnemyBase::draw(float scrollX) {
}

void EnemyBase::update()
{
}

void EnemyBase::impacted()
{
}


