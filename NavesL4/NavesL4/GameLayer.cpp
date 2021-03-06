#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	pause = true;
	message = new Actor("res/mensaje_como_jugar2.png", WIDTH * 0.5, HEIGHT * 0.5,
		WIDTH, HEIGHT, game);

	gamePad = SDL_GameControllerOpen(0);
	init();
}


void GameLayer::init() {

	audioBomb = new Audio("res/bum.wav", false);
	space = new Space(0);

	if (perdio) {
		enemiesInLevel = 0;
		points = 0;
		bombs = 0;
	}
	scrollX = 0;

	textPoints = new Text("puntos", WIDTH * 0.92, HEIGHT * 0.06, game);
	textPoints->content = to_string(points);
	textLifes = new Text("vidas", WIDTH * 0.2, HEIGHT * 0.06, game);
	textBombs = new Text("bombas", WIDTH * 0.35, HEIGHT * 0.06, game);
	textBombs->content = to_string(bombs);
	textEscudo = new Text("escudo", WIDTH * 0.55, HEIGHT * 0.06, game);
	textEscudo->content = to_string(escudoPercentage) + " %";


	background = new Background("res/fondo__.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);
	backgroundLifes = new Actor("res/corazon_.png",
		WIDTH * 0.15, HEIGHT * 0.05, 44, 36, game);
	backgroundBomb = new Actor("res/bomba_peque.png",
		WIDTH * 0.3, HEIGHT * 0.05, 30, 30, game);
	backgroundEscudo = new Actor("res/escudo.png",
		WIDTH * 0.45, HEIGHT * 0.05, 30, 30, game);

	enemies.clear(); // Vaciar por si reiniciamos el juego
	tilesDestructibles.clear(); // Vaciar por si reiniciamos el juego
	tiles.clear();
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	projectilesEnemigo.clear(); // Vaciar por si reiniciamos el juego
	recolectables.clear();
	loadMap("res/" + to_string(game->currentLevel) + ".txt");
}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por l�nea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por car�cter (en cada l�nea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
			}

		}
	}
	streamFile.close();
}

void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
	case 'C': {
		cup = new Tile("res/copa__.png", x, y, game);
		cup->y = cup->y - cup->height / 2;
		space->addDynamicActor(cup);
		break;
	}

	case 'E': {
		EnemyBase* enemy = new Enemy(x, y, game);
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		enemiesInLevel++;
		break;
	}
	case 'S': {
		EnemyBase* enemy = new EnemyStatic(x, y, game);
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		enemiesInLevel++;
		break;
	}
	case '1': {
		if (perdio) {
			player = new Player(x, y, game);
			player->y = player->y - player->height / 2;

		}
		space->addDynamicActor(player);
		textLifes->content = to_string(player->lifes);
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/plataforma__.png", x, y, game);
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'X': {
		TileDestructible* tile = new TileDestructible("res/obstacle.png", x, y, game);
		tile->y = tile->y - tile->height / 2;
		tilesDestructibles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}

	case 'V': {
		RecolectableVida* recolectable = new RecolectableVida(x, y, game);
		recolectable->y = recolectable->y - recolectable->height / 2;
		recolectables.push_back(recolectable);
		space->addDynamicActor(recolectable);
		break;
	}
	case 'B': {
		bomb = new Actor("res/bomba_peque.png",
			x, y, 30, 30, game);
		bomb->y = bomb->y - bomb->height / 2;
		space->addDynamicActor(bomb);
		break;
	}
	}
}


void GameLayer::processControls() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_CONTROLLERDEVICEADDED) {
			gamePad = SDL_GameControllerOpen(0);
			if (gamePad == NULL) {
				cout << "error en GamePad" << endl;
			}
			else {
				cout << "GamePad conectado" << endl;
			}
		}

		if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERAXISMOTION) {
			game->input = game->inputGamePad;
		}
		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = game->inputMouse;
		}
		// Procesar teclas
		if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
		if (game->input == game->inputMouse) {
			mouseToControls(event);
		}
		// Procesar Mando
		if (game->input == game->inputGamePad) {  // gamePAD
			gamePadToControls(event);
		}
	}

	if (controlContinue) {
		pause = false;
		controlContinue = false;
	}
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
		}

	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);

	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}
}

void GameLayer::update() {
	if (pause) {
		return;
	}
	if (escudoPercentage < 100) {
		escudoRecarga--;
		if (escudoRecarga == 0) {
			escudoPercentage++;
			escudoRecarga = 5;
			textEscudo->content = to_string(escudoPercentage) + " %";
		}
	}
	if (player->hasEscudo) {
		escudoActivoTime--;
		if (escudoActivoTime <= 0) {
			player->hasEscudo = false;
			escudoActivoTime = 200;
		}
	}
	// Nivel superado
	if (cup->isOverlap(player)) {
		if (enemiesInLevel <= 0) {
			game->currentLevel++;
			if (game->currentLevel > game->finalLevel) {
				game->currentLevel = 0;
			}
			message = new Actor("res/mensaje_ganar.png", WIDTH * 0.5, HEIGHT * 0.5,
				WIDTH, HEIGHT, game);
			pause = true;
			perdio = false;
			init();
		}
	}

	// Jugador se cae
	if (player->y > HEIGHT + 80) {
		init();
	}
	space->update();
	//background->update();
	player->update();
	for (auto const& enemy : enemies) {
		enemy->update();
	}
	// Colisiones
	colisionBomba();
	colisionPlayerEnemy();
	colisionProjectilPlayer();
	colisionPlayerRecolectable();
	deleteEnemies();
	deleteProjectiles();

	enemyShoot();

}


void GameLayer::colisionBomba() {
	if (bomb != nullptr) {
		if (bomb->isOverlap(player)) {
			bombs++;
			textBombs->content = to_string(bombs);
			bomb = nullptr;
		}
	}
}

void GameLayer::colisionPlayerEnemy() {
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			if (enemy->state != game->stateDying && enemy->state != game->stateDead && player->hasEscudo == false) {
				player->loseLife();
				textLifes->content = to_string(player->lifes);
				if (player->lifes <= 0) {
					perdio = true;
					init();
					return;
				}
			}
		}
	}
}


void GameLayer::colisionProjectilPlayer() {
	list<ProjectilEnemigo*> deleteProjectilesEnemigo;

	for (auto const& projectile : projectilesEnemigo) {
		if (player->isOverlap(projectile) && player->hasEscudo == false) {
			bool pInList = std::find(deleteProjectilesEnemigo.begin(),
				deleteProjectilesEnemigo.end(),
				projectile) != deleteProjectilesEnemigo.end();
			if (!pInList) {
				deleteProjectilesEnemigo.push_back(projectile);
			}
			player->loseLife();

			if (player->lifes <= 0) {
				perdio = true;
				init();
				return;
			}
			textLifes->content = to_string(player->lifes);
		}
	}
	for (auto const& delProjectile : deleteProjectilesEnemigo) {
		projectilesEnemigo.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectilesEnemigo.clear();

}

void GameLayer::deleteProjectiles() {

	list<Projectile*> deleteProjectiles;
	for (auto const& projectile : projectiles) {
		if (projectile->isInRender(scrollX) == false) {
			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}


	for (auto const& projectile : projectiles) {
		for (auto const& tile : tiles) {

			if (tile->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}
			}
		}
	}
	list<TileDestructible*> deleteTilesDestructibles;
	list<ProjectilEnemigo*> deleteProjectilesEnemigo;

	for (auto const& projectile : projectilesEnemigo) {
		for (auto const& tile : tiles) {

			if (tile->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectilesEnemigo.begin(),
					deleteProjectilesEnemigo.end(),
					projectile) != deleteProjectilesEnemigo.end();

				if (!pInList) {
					deleteProjectilesEnemigo.push_back(projectile);
				}
			}
		}for (auto const& tile : tilesDestructibles) {

			if (tile->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectilesEnemigo.begin(),
					deleteProjectilesEnemigo.end(),
					projectile) != deleteProjectilesEnemigo.end();

				if (!pInList) {
					deleteProjectilesEnemigo.push_back(projectile);
				}
			}
		}
	}


	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				if (enemy->state != game->stateDying && enemy->state != game->stateDead) {
					bool pInList = std::find(deleteProjectiles.begin(),
						deleteProjectiles.end(),
						projectile) != deleteProjectiles.end();
					if (!pInList) {
						deleteProjectiles.push_back(projectile);
					}

					if (enemy->lifes <= 1) {
						points++;
						enemiesInLevel--;
						textPoints->content = to_string(points);
					}
					enemy->impacted();

					cout << enemy->lifes << endl;

				}
			}
		}
	}


	for (auto const& tile : tilesDestructibles) {
		for (auto const& projectile : projectiles) {
			if (tile->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();
				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}

				if (tile->lifes <= 1) {
					bool eInList = std::find(deleteTilesDestructibles.begin(),
						deleteTilesDestructibles.end(),
						tile) != deleteTilesDestructibles.end();

					if (!eInList) {
						deleteTilesDestructibles.push_back(tile);
					}
				}
				tile->impacted();
			}
		}
	}


	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();

	for (auto const& delTile : deleteTilesDestructibles) {
		tilesDestructibles.remove(delTile);
		space->removeDynamicActor(delTile);
		delete delTile;
	}
	deleteTilesDestructibles.clear();

	for (auto const& delProjectile : deleteProjectilesEnemigo) {
		projectilesEnemigo.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();
	deleteProjectilesEnemigo.clear();


}


void GameLayer::colisionPlayerRecolectable() {
	list<RecolectableVida*> deleteRecolectables;
	for (auto const& recolectable : recolectables) {
		if (player->isOverlap(recolectable)) {
			player->lifes++;
			textLifes->content = to_string(player->lifes);

			deleteRecolectables.push_back(recolectable);

			bool eInList = std::find(deleteRecolectables.begin(),
				deleteRecolectables.end(),
				recolectable) != deleteRecolectables.end();

			if (!eInList) {
				deleteRecolectables.push_back(recolectable);
			}
		}
	}
	for (auto const& delRec : deleteRecolectables) {
		recolectables.remove(delRec);
	}
	deleteRecolectables.clear();

}

void GameLayer::deleteEnemies() {
	list<EnemyBase*> deleteEnemies;


	for (EnemyBase* enemy : enemies) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();

			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
			delete(enemy);
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	deleteEnemies.clear();

}
void GameLayer::enemyShoot() {
	for (EnemyBase* e : enemies) {
		if (e->isShooter) {
			ProjectilEnemigo* p = e->shoot(player);
			if (p != NULL) {
				space->addDynamicActor(p);
				projectilesEnemigo.push_back(p);
			}
		}
	}

}
void GameLayer::calculateScroll() {
	// limite izquierda
	if (player->x > WIDTH * 0.3) {
		if (player->x - scrollX < WIDTH * 0.3) {
			scrollX = player->x - WIDTH * 0.3;
		}
	}

	// limite derecha
	if (player->x < mapWidth - WIDTH * 0.3) {
		if (player->x - scrollX > WIDTH * 0.7) {
			scrollX = player->x - WIDTH * 0.7;
		}
	}
}


void GameLayer::draw() {
	calculateScroll();

	background->draw();
	cup->draw(scrollX);
	player->draw(scrollX);

	if (bomb != nullptr) {
		bomb->draw(scrollX);
	}
	for (auto const& tile : tiles) {
		tile->draw(scrollX);
	}
	for (auto const& tile : tilesDestructibles) {
		tile->draw(scrollX);
	}
	for (auto const& recolectable : recolectables) {
		recolectable->draw(scrollX);
	}
	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX);
	}
	for (auto const& projectile : projectilesEnemigo) {
		projectile->draw(scrollX);
	}
	for (auto const& enemy : enemies) {
		enemy->draw(scrollX);
	}

	backgroundPoints->draw();
	textPoints->draw();

	backgroundEscudo->draw();
	textEscudo->draw();

	backgroundLifes->draw();
	textLifes->draw();

	backgroundBomb->draw();
	textBombs->draw();

	if (pause) {
		message->draw();
	}

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::gamePadToControls(SDL_Event event) {

	// Leer los botones
	bool buttonA = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_A);
	bool buttonB = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_B);
	bool buttonX = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_X);
	bool buttonY = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_Y);

	int stickX = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_LEFTX);
	int stickY = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_LEFTY);

	// Retorna aproximadamente entre [-32800, 32800], el centro deber�a estar en 0
	// Si el mando tiene "holgura" el centro varia [-4000 , 4000]
	if (stickX > 4000) {
		controlMoveX = 1;
	}
	else if (stickX < -4000) {
		controlMoveX = -1;
	}
	else {
		controlMoveX = 0;
	}

	if (stickY > 4000) {
		controlMoveY = 1;
	}
	else if (stickY < -4000) {
		controlMoveY = -1;
	}
	else {
		controlMoveY = 0;
	}
	
	if (buttonX) {
		controlShoot = true;
	}
	else {
		controlShoot = false;
	}
	if (buttonA) {
		controlContinue = true;
	}
	if (buttonB) {
		escudoAction();
	}
	if (buttonY) {
		bombAction();
	}
}


void GameLayer::mouseToControls(SDL_Event event) {
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		controlContinue = true;
	}
	
}


void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		controlContinue = true;

		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_q: // bomba
			bombAction();
			break;
		case SDLK_e: // bomba
			escudoAction();
			break;
		case SDLK_F1:
			showHelp();
			break;
		case SDLK_SPACE: // dispara
			controlContinue = true;
			controlShoot = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_F1:
			hideHelp();
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}
	}
}

void GameLayer::showHelp() {
	message = new Actor("res/controles_.png", WIDTH * 0.5, HEIGHT * 0.5,
		WIDTH, HEIGHT, game);
	controlContinue = false;
	pause = true;


}
void GameLayer::hideHelp() {
	controlContinue = true;
	pause = false;
}

void GameLayer::bombAction() {
	if (bombs != 0) {
		bombs--;
		audioBomb->play();
		textBombs->content = to_string(bombs);
		for (EnemyBase* enemy : enemies) {
			if (enemy->isInRender()) {
				//cambiar animacion de morir
				enemy->aDying = enemy->aExplosion;
				enemy->impacted();
				if (enemy->lifes <= 1) {
					points++;
					enemiesInLevel--;
					textPoints->content = to_string(points);
				}
			}
		}
	}
}

void GameLayer::escudoAction() {
	if (escudoPercentage == 100) {
		cout << "escudo" << endl;
		escudoPercentage = 0;
		player->hasEscudo = true;
	}
}


