#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	pause = true;
	message = new Actor("res/mensaje_como_jugar.png", WIDTH * 0.5, HEIGHT * 0.5,
		WIDTH, HEIGHT, game);

	gamePad = SDL_GameControllerOpen(0);
	init();
}


void GameLayer::init() {
	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);

	buttonJump = new Actor("res/boton_salto.png", WIDTH * 0.9, HEIGHT * 0.55, 100, 100, game);
	buttonShoot = new Actor("res/boton_disparo.png", WIDTH * 0.75, HEIGHT * 0.83, 100, 100, game);

	space = new Space(0);
	scrollX = 0;
	tiles.clear();

	enemiesInLevel = 0;

	points = 0;
	textPoints = new Text("puntos", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);
	textLifes = new Text("vidas", WIDTH * 0.2, HEIGHT * 0.04, game);


	background = new Background("res/fondo__.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);
	backgroundLifes = new Actor("res/corazon.png",
		WIDTH * 0.15, HEIGHT * 0.05, 24, 24, game);

	enemies.clear(); // Vaciar por si reiniciamos el juego
	projectiles.clear(); // Vaciar por si reiniciamos el juego
	projectilesEnemigo.clear(); // Vaciar por si reiniciamos el juego

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
		// modificaci�n para empezar a contar desde el suelo.
		cup->y = cup->y - cup->height / 2;
		space->addDynamicActor(cup); // Realmente no hace falta
		break;
	}
	case 'E': {
		EnemyBase* enemy = new Enemy(x, y, game);
		// modificaci�n para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		enemiesInLevel++;
		break;
	}
	case 'O': {
		EnemyBase* enemy = new EnemyStatic(x, y, game);
		// modificaci�n para empezar a contar desde el suelo.
		enemy->y = enemy->y - enemy->height / 2;
		enemies.push_back(enemy);
		space->addDynamicActor(enemy);
		enemiesInLevel++;
		break;
	}
	case '1': {
		player = new Player(x, y, game);
		// modificaci�n para empezar a contar desde el suelo.
		player->y = player->y - player->height / 2;
		space->addDynamicActor(player);
		textLifes->content = to_string(player->lifes);
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/plataforma__.png", x, y, game);
		// modificaci�n para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	case 'X': {
		Tile* tile = new Tile("res/obstacle.png", x, y, game);
		// modificaci�n para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	}
}


void GameLayer::processControls() {
	// obtener controles
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

		// Cambio autom�tico de input
		// PONER el GamePad
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
	// Nivel superado
	if (cup->isOverlap(player)) {
		if (enemiesInLevel == 0) {
			game->currentLevel++;
			if (game->currentLevel > game->finalLevel) {
				game->currentLevel = 0;
			}
			message = new Actor("res/mensaje_ganar.png", WIDTH * 0.5, HEIGHT * 0.5,
				WIDTH, HEIGHT, game);
			pause = true;
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
	for (auto const& enemy : enemies) {
		if (player->isOverlap(enemy)) {
			if (enemy->state != game->stateDying && enemy->state != game->stateDead) {
				player->loseLife();
				player->loseLife();
				textLifes->content = to_string(player->lifes);
				if (player->lifes <= 0) {
					init();
					return;
				}
			}
		}
	}



	list<EnemyBase*> deleteEnemies;
	list<Projectile*> deleteProjectiles;
	list<ProjectilEnemigo*> deleteProjectilesEnemigo;
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
		}
	}


	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				if (enemy->state != game->stateDying && enemy->state != game->stateDead) {
					bool pInList = std::find(deleteProjectiles.begin(),
						deleteProjectiles.end(),
						projectile) != deleteProjectiles.end();
					enemy->state = game->stateDying;
					if (!pInList) {
						deleteProjectiles.push_back(projectile);
					}
					enemy->impacted();
					points++;
					textPoints->content = to_string(points);
					enemiesInLevel--;

				}
			}
		}
	}

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
		delete(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();

	for (auto const& delProjectile : deleteProjectilesEnemigo) {
		projectilesEnemigo.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();
	deleteProjectilesEnemigo.clear();

	enemyShoot();

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
	for (auto const& tile : tiles) {
		tile->draw(scrollX);
	}

	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX);
	}
	for (auto const& projectile : projectilesEnemigo) {
		projectile->draw(scrollX);
	}
	cup->draw(scrollX);
	player->draw(scrollX);
	for (auto const& enemy : enemies) {
		enemy->draw(scrollX);
	}

	backgroundPoints->draw();
	textPoints->draw();

	backgroundLifes->draw();
	textLifes->draw();


	if (pause) {
		message->draw();
	}

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::gamePadToControls(SDL_Event event) {

	// Leer los botones
	bool buttonA = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_A);
	bool buttonB = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_B);
	// SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B
	// SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y
	int stickX = SDL_GameControllerGetAxis(gamePad, SDL_CONTROLLER_AXIS_LEFTX);

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

	if (buttonA) {
		controlShoot = true;
	}
	else {
		controlShoot = false;
	}

	if (buttonB) {
		controlMoveY = -1; // Saltar
	}
	else {
		controlMoveY = 0;
	}
}


void GameLayer::mouseToControls(SDL_Event event) {
	// Modificaci�n de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;
	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		controlContinue = true;
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = true;
			// CLICK TAMBIEN TE MUEVE
			controlMoveX = pad->getOrientationX(motionX);
		}
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = true;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = -1;
		}

	}
	// Cada vez que se mueve
	if (event.type == SDL_MOUSEMOTION) {
		if (pad->clicked && pad->containsPoint(motionX, motionY)) {
			controlMoveX = pad->getOrientationX(motionX);
			// Rango de -20 a 20 es igual que 0
			if (controlMoveX > -20 && controlMoveX < 20) {
				controlMoveX = 0;
			}

		}
		else {
			pad->clicked = false; // han sacado el rat�n del pad
			controlMoveX = 0;
		}
		if (buttonShoot->containsPoint(motionX, motionY) == false) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY) == false) {
			controlMoveY = 0;
		}

	}
	// Cada vez que levantan el click
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = false;
			// LEVANTAR EL CLICK TAMBIEN TE PARA
			controlMoveX = 0;
		}

		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = 0;
		}

	}
}


void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
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
		case SDLK_SPACE: // dispara
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
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}

	}

}

