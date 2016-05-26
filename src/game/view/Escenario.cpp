/*
 * Escenario.cpp
 *
 *  Created on: 24/04/2016
 *      Author: gonzalo
 */

#include "Escenario.h"
using namespace std;

Escenario::Escenario(GameConf* configuracion, XM_SDL* sdl) {
	this->gc = configuracion;
	this->sdl = sdl;
	this->SCREEN_WIDTH = gc->escenario->ancho;
	this->SCREEN_HEIGHT = gc->escenario->alto;
	this->LONGITUD_NIVEL = gc->escenario->longitudNivel;
	this->CANTIDAD_NIVELES = gc->escenario->cantidadNiveles;
	resolucion = Resolucion::INSTANCE(SCREEN_WIDTH, SCREEN_HEIGHT);

	int spriteN = GameParser::findSprite(gc->sprites, gc->escenario->fondo);
	if (spriteN != -1) {

		DIR_FONDO_PANTALLA = gc->sprites[spriteN]->path;
	}

	gRenderer = sdl->getRenderer();
	escenarioScreen = new Screen(this->sdl);
	escenarioScreen->loadTexture("agua", "fondos/" + DIR_FONDO_PANTALLA);

	controllers = new HandlerPlayersControllers(gRenderer, resolucion);

	//TODO: hay que cargar desde el XML donde van a salir los PowerUps
	hPowerUp = new HandlerPowerUp(gRenderer, resolucion);
  hPowerUp->setPowerUp(new PowerUp(gRenderer, resolucion, new Posicion(350, 600)));
	this->healthView = NULL;
}

Escenario::~Escenario() {
	resolucion->~Resolucion();
	delete escenarioScreen;
	delete myControl;
	delete controllers;
	limpiarFondosVivibles();
}

void Escenario::actualizarEscenario(Posicion* pos) {

	escenarioScreen->renderTexture("agua", pos->getX(), pos->getYsdl());

	for (int i = 0; i < fondosVivibles.size(); i++) {
		fondosVivibles[i]->vivir();
	}

	controllers->hacerVivir();
	myControl->hacerVivir();
	hPowerUp->hacerVivir();
	this->healthView->update( this->player->getHealth() );
	this->healthView->render();

	this->sdl->updateWindow();
	// set new offset on client
	this->unCliente->setStageOffset(pixelesRecorridos);
}

void Escenario::aplicarFPS(Uint32 start) {
	Uint32 dif = (SDL_GetTicks() - start);
	int time = 1000 / FRAMES_PER_SECOND;
	if (time > dif) {
		SDL_Delay(time - dif);
	}
}

void Escenario::setClient(Client* cliente) {
	this->unCliente = cliente;
}

void Escenario::setPlayer( Player* player ) {
  this->player = player;
  this->healthView = new HealthView( this->escenarioScreen, this->player->getHealth() );
}

void Escenario::configurarFondosVivibles() {

	int cantidadRepeticionesPorNivel = 1;
	if (LONGITUD_NIVEL > LIMITE_IMAGEN) {
		cantidadRepeticionesPorNivel = LONGITUD_NIVEL / LIMITE_IMAGEN;
	}

	int cantRepeticiones = (CANTIDAD_NIVELES * cantidadRepeticionesPorNivel) + 1;

	for (int i = 0; i < gc->elementos.size(); i++) {
		int x_gc = gc->elementos[i]->x;
		int y_gc = gc->elementos[i]->y;
		int index = GameParser::findSprite(gc->sprites,gc->elementos[i]->spriteID);

		if (index >= 0) {

			for (int j = 0; j < cantRepeticiones; j++) {
				int x = x_gc;
				int y = y_gc + (LIMITE_IMAGEN * j);
				Posicion* p = new Posicion(x, y);
				string jString  = to_string(j);
				Isla* isla = new Isla(jString, p, gc->sprites[index], escenarioScreen);
				fondosVivibles.push_back(isla);
			}
		}
	}
}

HandlerPlayersControllers* Escenario::getHandler() {
	return this->controllers;
}

void Escenario::configurarAvionAmigo(PlayerData* playerData) {
	Vivible* avionAmigo = new Avion(playerData, escenarioScreen, gRenderer, resolucion, new Posicion(playerData->x, playerData->y), gc->avion);
	controllers->setPlayer((Avion*) avionAmigo);
}

void Escenario::configurarMiAvion(PlayerData* playerData) {
	Vivible* avion = new Avion(playerData, escenarioScreen, gRenderer, resolucion, new Posicion(playerData->x, playerData->y), gc->avion);
	myControl = new Controller(avion, gRenderer, resolucion, this->unCliente);
}

void Escenario::setFondosVivibles(int x, int y) {

	for (int i = 0; i < fondosVivibles.size(); i++) {
		fondosVivibles[i]->vivir(x, y);
	}

}

SDL_Event* Escenario::run() {

	pixelesRecorridos = 0;
	configurarFondosVivibles();
	Posicion* posicionEscenario = new Posicion(0, 0);

	//Reinicia mediante R no entra a buscar el offset, sino si (caso: salio por Q y vuelve a ingresar)
	if (!this->unCliente->reinicia) {

		int offset = this->unCliente->getStageOffset();
		if (offset != 0) {
			pixelesRecorridos = offset + desfasajeConexion;
			setFondosVivibles(0, pixelesRecorridos);
		}
	}

	actualizarEscenario(posicionEscenario);
	Uint32 start;
	bool quit = false;

	for (int numeroNivel = 1; numeroNivel < (CANTIDAD_NIVELES + 1); numeroNivel++) {

		while (!quit && this->unCliente->isConnected()) {

			start = SDL_GetTicks();

			if (this->unCliente->reset) {
				SDL_Event* eventReset = new SDL_Event();
				eventReset->key.keysym.sym = SDLK_r;
				this->unCliente->setStageOffset(0);
				return eventReset;
			}

			while (this->sdl->nextEvent(&evento) && evento.type != SDL_MOUSEMOTION) {

				myControl->press(&evento);

				if( evento.type == SDL_KEYDOWN ) {
					if (evento.type == SDL_QUIT || evento.key.keysym.sym == SDLK_q || evento.key.keysym.sym == SDLK_r || this->unCliente->reset) {
						quit = true;

						PlayerData* p = new PlayerData();

						strcpy(p->name, (myControl->getVivible())->getId().c_str());
						p->x = myControl->getVivible()->getX();
						p->y = myControl->getVivible()->getY();

						while (!this->unCliente->sendDataPosicion(p))
							;
						usleep(100);

						break;
					}

//					TODO: Esto fue agregado para probar cuando me disparan
					if (evento.key.keysym.sym == SDLK_d) {
						Avion* avion = (Avion*)myControl->getVivible();
						Misil* disparoEnemigo = new Misil(gRenderer, new Posicion(0,0), resolucion, NULL);
						avion->recibirMisil(disparoEnemigo);
					}
				}

			}

			if (isFinNivel(numeroNivel)) {

//				TODO: Aca deberia ir la inscripcion de fin de nivel
				cout << "SE TERMINO EL NIVEL " << numeroNivel << endl;

				// Send player score
				PlayerScore* playerScore = new PlayerScore;
				strcpy( playerScore->name, this->player->getName().c_str() );
				strcpy( playerScore->color, this->player->getColor().c_str() );
				playerScore->score = this->player->getScore();

				this->unCliente->sendScore( playerScore );
				delete playerScore;

				// TODO: change hardcoded 1 for actual number of connected players 
				// wait for player score data
				while ( this->unCliente->getPlayersScoreData().size() != 1 );
				this->loadSinglePlayerScoreScreen( numeroNivel );

				break;
			} else {

				pixelesRecorridos -= VELOCIDAD_SCREEN;
				if (posicionEscenario->getY() > SCREEN_HEIGHT) {
					posicionEscenario->mover(0, VELOCIDAD_SCREEN);
				} else {
					posicionEscenario->setPosicion(0, LIMITE_IMAGEN);
				}

				actualizarEscenario(posicionEscenario);
				aplicarFPS(start);
			}
		}

	}

	delete posicionEscenario;
	return &evento;
}

bool Escenario::isFinNivel(int numeroNivel) {
	return pixelesRecorridos >= LONGITUD_NIVEL * numeroNivel;
}

void Escenario::limpiarFondosVivibles() {

	if (fondosVivibles.size() > 0) {
		for (int i = 0; i < fondosVivibles.size(); i++) {
			delete fondosVivibles[i];
		}
	}
	fondosVivibles.clear();
}

void Escenario::loadSinglePlayerScoreScreen( int stage ) {
  bool runningScreen = true;
  SDL_Event e;
  Timer timer;
  int fps = 10;
  string stageCompleteText = "Stage " + to_string( stage ) + " Complete !!";
  string scoreHeaderText = "Score Ranking";
  string nameText = "Name";
  string scoreText = "Score";

  Screen* scoreScreen= new Screen( this->sdl );
  scoreScreen->setCanvasWidth( this->sdl->getWindowWidth() );

  // Load text
  scoreScreen->loadText( "stageComplete", stageCompleteText, { 53, 167, 84, 255 } );
  scoreScreen->loadText( "scoreText", scoreHeaderText, { 255, 0, 0, 255 } );
  scoreScreen->loadText( "nameHeader", nameText, { 191, 189, 37, 255 } );
  scoreScreen->loadText( "scoreHeader", scoreText, { 191, 189, 37, 255 } );
  scoreScreen->loadText( "continueText", "Continue", { 0, 0, 0, 255 } );

  // Load ranking score table data 
  for( int i = 0; i < this->unCliente->getPlayersScoreData().size(); i++ ) {  
    PlayerScore* ps = this->unCliente->getPlayersScoreData()[i];
    scoreScreen->loadText( string( ps->name ), string( ps->name ), { 255, 255, 255, 255 } );
    scoreScreen->loadText( string( ps->name ) + "score", to_string( ps->score ), { 255, 255, 255, 255 } );
    // Load plane
    scoreScreen->loadTexture( string( ps->color ), "score/avion_" + string( ps->color ) + ".bmp" );
  }

  int buttonWidth = 250;
  int buttonCenter = scoreScreen->getRectCenter( buttonWidth ); 
  int continueTextCenter = scoreScreen->getTextCenter( "continue" );

  // Load prompts
  scoreScreen->loadRectangle( "continue", buttonCenter, 600, buttonWidth, 50 );

  int gap = scoreScreen->getTextHeight( scoreText );
  int rowPadding = 200;
  int topPadding = 60;
  // Get center positions
  int stageCompleteTextCenter = scoreScreen->getTextCenter( stageCompleteText ); 
  int scoreTextCenter = scoreScreen->getTextCenter( scoreHeaderText );
  int nameHeaderSpace = scoreScreen->getTextWidth( nameText ) + rowPadding;
  int scoreHeaderCenter = stageCompleteTextCenter + nameHeaderSpace;
  int scoreRightLimit = scoreHeaderCenter + scoreScreen->getTextWidth( "Score" );
  int imageCenter = stageCompleteTextCenter - 65;

  bool clicked = false;
  int mouseX, mouseY;
  int gapMult = 5.5;
  int gapStep = 1.5;

  HealthView* remainingHealth = new HealthView( scoreScreen, this->player->getHealth() );

  // Enable text input
  SDL_StartTextInput();

  while( runningScreen ) {
    timer.correr();
    // Get events
    while( this->sdl->nextEvent( &e ) ) {
      if( e.type == SDL_QUIT ) {
	runningScreen = false;
	break;
      }
      if (e.button.type == SDL_MOUSEBUTTONDOWN) {
	if (e.button.button == SDL_BUTTON_LEFT) {
	  clicked = true;
	  // Get the mouse offsets
	  mouseX = e.button.x;
	  mouseY = e.button.y;
	}
      }
    }
    // Set window background
    this->sdl->setWindowBG(0, 0, 0);

    remainingHealth->render();

    // Render text textures
    scoreScreen->renderTexture( "stageComplete", stageCompleteTextCenter, topPadding );
    scoreScreen->renderTexture( "scoreText", scoreTextCenter, topPadding + gap * 2 );
    scoreScreen->renderTexture( "nameHeader", stageCompleteTextCenter, topPadding + gap * 4 );
    scoreScreen->renderTexture( "scoreHeader", scoreHeaderCenter, topPadding + gap * 4 );

    // Render players score and data
    for( int i = 0; i < this->unCliente->getPlayersScoreData().size(); i++ ) {  
      PlayerScore* ps = this->unCliente->getPlayersScoreData()[i];
      scoreScreen->renderTexture( string( ps->name ), 
				  stageCompleteTextCenter, 
				  topPadding + gap * ( gapMult + ( ( i + 1 ) * gapStep ) ) );
      scoreScreen->renderTexture( string( ps->name ) + "score", 
				  scoreRightLimit - scoreScreen->getTextWidth( to_string( ps->score ) ), 
				  topPadding + gap * ( gapMult + ( ( i + 1 ) * gapStep ) ) );
      scoreScreen->renderTexture( string( ps->color ), 
				  imageCenter, 
				  topPadding + gap * ( gapMult + ( ( i + 1 ) * gapStep ) ) );
    }

    scoreScreen->setRenderDrawColor( 160, 160, 160, 255 );
    scoreScreen->renderRectangle( "continue" );
    scoreScreen->renderTexture( "continueText", continueTextCenter, 605 );

    if( clicked ) {
      clicked = false;
      if( ( mouseX > buttonCenter ) && ( mouseX < ( buttonCenter + 250 ) )
	  && ( mouseY > 600 ) && ( mouseY < ( 600 + 50 ) ) ) {
	runningScreen = false;
	break;
      }
    }

    //Update screen
    this->sdl->updateWindow();

    if( timer.tiempoActual() < 1000 / fps ){
      SDL_Delay( ( 1000 / fps ) - timer.tiempoActual() );
    }
  }

  this->unCliente->resetScores();
  //Disable text input
  SDL_StopTextInput();
  delete scoreScreen;
}
