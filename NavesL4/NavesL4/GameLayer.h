#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"

#include "Enemy.h"
#include "EnemyBase.h"
#include "EnemyStatic.h"
#include "Projectile.h"
#include "ProjectilEnemigo.h"
#include "Text.h"
#include "Tile.h"
#include "Pad.h"

#include "Space.h" // importar

#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void enemyShoot();
	void draw() override;
	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event); // USO DE MOUSE
	void gamePadToControls(SDL_Event event); // USO DE GAMEPAD
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void calculateScroll();
	Actor* message;
	bool pause;
	// Elementos de interfaz
	SDL_GameController* gamePad;
	Pad* pad;
	Actor* buttonJump;
	Actor* buttonShoot;

	Tile* cup; // Elemento de final de nivel
	Space* space;
	float scrollX;
	int mapWidth;
	list<Tile*> tiles;

	Text* textPoints;
	int points;
	Text* textLifes;
	Actor* backgroundLifes;

	int newEnemyTime = 0;
	Player* player;
	Background* background;
	Actor* backgroundPoints;
	list<EnemyBase*> enemies;
	list<Projectile*> projectiles;
	list<ProjectilEnemigo*> projectilesEnemigo;

	bool controlContinue = false;
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;

	int enemiesInLevel = 0;

};

