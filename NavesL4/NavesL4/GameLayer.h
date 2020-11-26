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
#include "TileDestructible.h"
#include "Pad.h"

#include "Space.h" // importar

#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include <list>
#include "RecolectableVida.h"

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void colisionPlayerRecolectable();
	void deleteEnemies();
	void enemyShoot();
	void draw() override;
	void keysToControls(SDL_Event event);
	void showHelp();
	void hideHelp();
	void mouseToControls(SDL_Event event); // USO DE MOUSE
	void gamePadToControls(SDL_Event event); // USO DE GAMEPAD
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void calculateScroll();
	void bombAction();
	void escudoAction();
	void colisionBomba();

	void colisionPlayerEnemy();
	void colisionProjectilPlayer();
	void deleteProjectiles();

	Actor* message;
	Actor* help;
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
	list<TileDestructible*> tilesDestructibles;

	Text* textPoints;
	int points;
	Text* textLifes;
	Text* textBombs;
	Text* textEscudo;
	int bombs;
	Actor* backgroundLifes;
	Actor* backgroundBomb;

	int newEnemyTime = 0;
	Player* player;
	Actor* bomb;
	Background* background;
	Actor* backgroundPoints;
	Actor* backgroundEscudo;
	list<EnemyBase*> enemies;
	list<Projectile*> projectiles;
	list<ProjectilEnemigo*> projectilesEnemigo;
	list<RecolectableVida*> recolectables;
	Audio* audioBomb;
	bool controlContinue = false;
	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;

	int enemiesInLevel = 0;

	int bombTime = 0;
	int escudoRecarga = 5;
	int escudoActivoTime = 200;
	int escudoPercentage = 0;
	bool perdio = true;
};

