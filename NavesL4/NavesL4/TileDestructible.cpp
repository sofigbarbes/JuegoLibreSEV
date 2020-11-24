#include "TileDestructible.h"

TileDestructible::TileDestructible(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, 40, 32, game) {
	lifes = 1;

}

void TileDestructible::impacted() {
	lifes--;
}
