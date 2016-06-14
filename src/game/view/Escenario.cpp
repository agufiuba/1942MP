/*
 * Escenario.cpp
 *
 *  Created on: 24/04/2016
 *      Author: gonzalo
 */

#include "Escenario.h"

#include "../../colision/Colision.h"

using namespace std;

Escenario::Escenario(GameConf* configuracion, XM_SDL* sdl, Client* client) {
	this->unCliente = client;
	musica = new Music("musicaDeFondo.mp3");
	musica->play();
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

	controllers = new HandlerPlayersControllers(gRenderer, resolucion, this->unCliente->getGameData());

	this->healthView = NULL;
	this->scoreView = NULL;
	this->teamScoreView = NULL;
	this->teamAlphaScoreView = NULL;
	this->teamBetaScoreView = NULL;
	flota = 0;
}

Escenario::~Escenario() {
	delete this->player;
	resolucion->~Resolucion();
	delete musica;
	escenarioCreado = false;
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
/*  Aca estan los Vivir y las Colisiones */
//	cout<<"1"<<endl;
	myControl->hacerVivir();
	controllers->hacerVivir();
//	cout<<"2"<<endl;
	this->getPowerUp();
//	cout<<"3"<<endl;
	hPowerUp->hacerVivir();
//	cout<<"4"<<endl;
	this->actualizarEnemigos();
//	cout<<"5"<<endl;
	this->hitEnemy(&(myControl->controlDeMisiles->getVivibles()->vectorObjetos));
//	cout<<"6"<<endl;
	if (!unCliente->getGameData()->practiceMode){
//		cout<<"Con colision"<<endl;
		this->planesColision();
		this->enemyOtherPlayerColision();
	} else {
		this->enemyOtherPlayerMissileColision();
	}
/*  Termina el bloque */

	// Render health
	this->healthView->update( this->player->getHealth() );
	this->healthView->render();
	GameData* gd = this->unCliente->getGameData();
	// Render score and team score
	if ( gd->cooperativeMode ) {
	  this->scoreView->update( this->player->getScore() );
	  this->scoreView->render( "R" );
	  this->teamScoreView->update( this->unCliente->getTeamScore() );
	  this->teamScoreView->render();
	// Team mode
	} else if ( gd->teamMode ) {
	  int team = this->player->getTeam();
	  // Alpha team
	  if ( team == 1 ) {
	  this->teamAlphaScoreView->update( this->unCliente->getTeamScore() );
	  this->teamAlphaScoreView->render( "R" );
	  this->teamBetaScoreView->update( this->unCliente->getRivalTeamScore() );
	  this->teamBetaScoreView->render();
	  // Beta team
	  } else {
	    this->teamBetaScoreView->update( this->unCliente->getTeamScore() );
	    this->teamBetaScoreView->render( "R" );
	    this->teamAlphaScoreView->update( this->unCliente->getRivalTeamScore() );
	    this->teamAlphaScoreView->render();
	  }
	}

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
  GameData* gd = this->unCliente->getGameData();
  // Coop mode
  if ( gd->cooperativeMode ) {
    this->scoreView = new ScoreView( this->escenarioScreen, this->player->getScore() );
    this->teamScoreView = new ScoreView( this->escenarioScreen, this->unCliente->getTeamScore(), "Team Score" );
  // Team mode
  } else if ( gd->teamMode ) {
    int team = this->player->getTeam();
    // Alpha team
    if ( team == 1 ) {
      this->teamAlphaScoreView = new ScoreView( this->escenarioScreen, this->unCliente->getTeamScore(), "Alpha Score" );
      this->teamBetaScoreView = new ScoreView( this->escenarioScreen, this->unCliente->getRivalTeamScore(), "Beta Score" );
    // Beta team
    } else {
      this->teamBetaScoreView = new ScoreView( this->escenarioScreen, this->unCliente->getTeamScore(), "Beta Score" );
      this->teamAlphaScoreView = new ScoreView( this->escenarioScreen, this->unCliente->getRivalTeamScore(), "Alpha Score" );
    }
  }
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

void Escenario::configurarPowerUps() {
	hPowerUp = new HandlerPowerUp(gRenderer, resolucion);
	unCliente->setPowerUpHandler(hPowerUp);
	if (gc->powerUps.size() <= 0) return;
//	cout << gc->powerUps.size() << " power ups creados" << endl;
	for (int i = 0; i < gc->powerUps.size(); i++) {
		string tipo = gc->powerUps[i]->tipo;
		Posicion* posicion = new Posicion(gc->powerUps[i]->x, gc->powerUps[i]->y);
		hPowerUp->setPowerUp(new PowerUp(gRenderer, resolucion, posicion, myControl, tipo, to_string(i)));
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
	if (unCliente->getGameData()->practiceMode)
		cout<<"Modo Practica"<<endl;
	pixelesRecorridos = 0;
	configurarFondosVivibles();
	configurarPowerUps();

	Posicion* posicionEscenario = new Posicion(0, 0);
	escenarioCreado = true;
  	crearEnemigo(300, 400);
	crearFlota(0, 400);
	crearGrande();

/*	thread tPowerUps(&Escenario::getPowerUp, this);
	tPowerUps.detach();

	thread tShot(&Escenario::hitEnemy, this);
	tShot.detach();*/
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

			// TODO: remove when adding score via game events, testing purposes only
			// this->unCliente->addScoreToPlayer( 1 );	
			
			if (this->unCliente->reset) {
				SDL_Event* eventReset = new SDL_Event();
				eventReset->key.keysym.sym = SDLK_r;
				this->unCliente->setStageOffset(0);
				return eventReset;
			}

			if ( this->unCliente->wins() ) {
			  this->loadScoreScreen();
			  quit = true;
			  break;
			}

			if ( this->unCliente->losses() ) {
			  this->loadScoreScreen( -1 );
			  quit = true;
			  break;
			}

			while (this->sdl->nextEvent(&evento)) {

				myControl->press(&evento);

				if( evento.type == SDL_KEYDOWN ) {
					  // TODO: remove, only for testing score additions
					  if ( evento.key.keysym.sym == SDLK_g ) {
					    this->unCliente->addScoreToPlayer( 50 );
					  }
					if (evento.type == SDL_QUIT || evento.key.keysym.sym == SDLK_q || evento.key.keysym.sym == SDLK_r || this->unCliente->reset) {
					  // reset client score on game reset
					  if( evento.key.keysym.sym == SDLK_r ) {
					    this->unCliente->requestScoreReset();
					  }
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

					//TODO: Este fue agregado para PROBAR el cambio a ametralladora
					// Si tocas t cuando el powerUp salio de la pantalla obviamente va a tirar segmentation fault
					if (evento.key.keysym.sym == SDLK_t) {
						hPowerUp->activar("1");
					}

					//TODO: Este fue agregado para PROBAR el destroy que lo debe hacer el server
					// Si tocas t cuando el powerUp salio de la pantalla obviamente va a tirar segmentation fault
					if (evento.key.keysym.sym == SDLK_x) {
						hPowerUp->activar("2");
					}

//					TODO: Esto fue agregado para probar cuando me disparan
					if (evento.key.keysym.sym == SDLK_d) {
						Avion* avion = (Avion*)myControl->getVivible();
						Misil* disparoEnemigo = new Misil(gRenderer, new Posicion(0,0), resolucion, NULL);
						avion->recibirMisil(disparoEnemigo);
						this->player->takeHit();
						if( !( this->player->isAlive() ) ) {
						  this->unCliente->sendPlayerDeath();
						}
					}
				}

			}

			verificarEstacionamiento(numeroNivel);

			if (isFinNivel(numeroNivel)) {
				musica->fadeOut(4000);
	    
				// load score screen
				this->loadScoreScreen( numeroNivel );

				delete musica;
				musica = new Music("musicaDeFondo.mp3");
				musica->play();
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


void Escenario::verificarEstacionamiento(int numeroNivel) {
	Avion* avion = (Avion*)myControl->getVivible();
	if (!avion->estaEstacionando() && (pixelesRecorridos + 400) >= LONGITUD_NIVEL * numeroNivel) {
		cout << "verificar estacionamiento" << endl;
		avion->inicializoEstacionar();
		Evento* e;
		CompanionEvent* ce = new CompanionEvent();
		unCliente->sendData(ce->estaciono(avion->getId()));
	}
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

void Escenario::loadScoreScreen( int stage ) {
  // load score data
  this->loadScoreData();

  GameData* gd = this->unCliente->getGameData();

  // load cooperative mode score screen
  if ( gd->cooperativeMode ) {
    this->loadCoopModeScoreScreen( stage );
  // load team mode score screen
  } else {
    this->loadTeamModeScoreScreen( stage );
  }

  // if win or lose, exit
  if ( stage <= 0 ) exit(0);

  // reset score table
  this->unCliente->resetScores();
  // load waiting screen
  this->loadWaitForPlayersScreen();
}

void Escenario::loadScoreData() {
  // Request clients playing
  this->unCliente->requestScoreTable();
  // wait for clients playing
  while( this->unCliente->getClientsPlaying() == 0 );
 
  GameData* gd = this->unCliente->getGameData();
  int scoreTableSize = this->unCliente->getClientsPlaying();
  scoreTableSize += gd->teamMode ? 2 : 1;
  
  // wait for player score data
  while ( this->unCliente->getPlayersScoreData().size() != scoreTableSize );
  this->unCliente->resetClientsPlaying();
}

void Escenario::loadTeamModeScoreScreen( int stage ) {
  bool runningScreen = true;
  SDL_Event e;
  Timer timer;
  int fps = 10;
  int playersScoreTableSize = this->unCliente->getPlayersScoreData().size() - 2;
  Screen* scoreScreen = new Screen( this->sdl );
  string stageCompleteText = "Stage " + to_string( stage ) + " Complete !!";
  string buttonText = "Continue";
  // win screen
  if ( stage == 0 ) {
    stageCompleteText = "Congratulations. You Win !!";
    buttonText = "Finish";
  // game over screen
  } else if ( stage == -1 ) {
    stageCompleteText = "Game Over";
    buttonText = "Finish";
    // Load skull image
    scoreScreen->loadTexture( "skull", "skull.bmp" );
  }
  string scoreHeaderText = "Team Mode Score Ranking";
  string nameText = "Name";
  string scoreText = "Score";
  string teamText = "Team";
  string totalText = "Total";

  // Load max score ribbon
  scoreScreen->loadTexture( "topScore", "topScore.bmp" );

  // Load text
  scoreScreen->loadText( "stageComplete", stageCompleteText, { 53, 167, 84, 255 } );
  scoreScreen->loadText( "scoreText", scoreHeaderText, { 255, 0, 0, 255 } );
  scoreScreen->loadText( "nameHeader", nameText, { 191, 189, 37, 255 } );
  scoreScreen->loadText( "teamHeader", teamText, { 191, 189, 37, 255 } );
  scoreScreen->loadText( "scoreHeader", scoreText, { 191, 189, 37, 255 } );
  scoreScreen->loadText( "totalHeader", totalText, { 191, 189, 37, 255 } );
  scoreScreen->loadText( "buttonText", buttonText, { 0, 0, 0, 255 } );

  // Get max score
  int maxScore = 0;
  for( int i = 0; i < playersScoreTableSize; i++ ) {  
    PlayerScore* ps = this->unCliente->getPlayersScoreData()[i];
    if ( ps->score > maxScore ) {
      maxScore = ps->score;
    }
  }

  // Load ranking score table data 
  for( int i = 0; i < playersScoreTableSize; i++ ) {  
    PlayerScore* ps = this->unCliente->getPlayersScoreData()[i];
    string team = ps->team == 1 ? "Alpha" : "Beta";
    
    if ( ps->score == maxScore ) {
      scoreScreen->loadText( string( ps->name ), string( ps->name ), { 12, 246, 246, 255 } );
      scoreScreen->loadText( string( ps->name ) + "team", team, { 12, 246, 246, 255 } );
      scoreScreen->loadText( string( ps->name ) + "score", to_string( ps->score ), { 12, 246, 246, 255 } );
    } else {
      scoreScreen->loadText( string( ps->name ), string( ps->name ), { 255, 255, 255, 255 } );
      scoreScreen->loadText( string( ps->name ) + "team", team, { 255, 255, 255, 255 } );
      scoreScreen->loadText( string( ps->name ) + "score", to_string( ps->score ), { 255, 255, 255, 255 } );
    }
    // Load plane
    scoreScreen->loadTexture( string( ps->color ), "score/avion_" + string( ps->color ) + ".bmp" );
  }

  int scoreTableSize = this->unCliente->getPlayersScoreData().size();
  int alphaTotal = ( this->unCliente->getPlayersScoreData()[ scoreTableSize - 2 ] )->score;
  int betaTotal = ( this->unCliente->getPlayersScoreData()[ scoreTableSize - 1 ] )->score;
  
  // load totals
  scoreScreen->loadText( "alphaTotal", to_string( alphaTotal ), { 255, 255, 255, 255 } );
  scoreScreen->loadText( "alphaTotalTop", to_string( alphaTotal ), { 12, 246, 246, 255 } );
  scoreScreen->loadText( "betaTotal", to_string( betaTotal ), { 255, 255, 255, 255 } );
  scoreScreen->loadText( "betaTotalTop", to_string( betaTotal ), { 12, 246, 246, 255 } );

  int buttonWidth = 250;
  int buttonCenter = scoreScreen->getRectCenter( buttonWidth ); 
  int buttonTextCenter = scoreScreen->getTextCenter( buttonText );

  // Load prompts
  scoreScreen->loadRectangle( "continue", buttonCenter, 600, buttonWidth, 50 );

  int gap = scoreScreen->getTextHeight( scoreText );
  int rowPadding = 120;
  int topPadding = 60;
  // Get center positions
  int stageCompleteTextCenter = scoreScreen->getTextCenter( stageCompleteText ); 
  int scoreTextCenter = scoreScreen->getTextCenter( scoreHeaderText );
  int nameHeaderSpace = scoreScreen->getTextWidth( nameText ) + rowPadding;
  int teamHeaderSpace = scoreScreen->getTextWidth( teamText ) + rowPadding;
  int scoreHeaderSpace = scoreScreen->getTextWidth( scoreText ) + rowPadding;
  int scoreTableWidth = nameHeaderSpace + teamHeaderSpace + scoreHeaderSpace + scoreScreen->getTextWidth( totalText );
  int nameHeaderCenter = ( scoreScreen->getCanvasWidth() - scoreTableWidth ) / 2;
  int teamHeaderCenter = nameHeaderCenter + nameHeaderSpace;
  int scoreHeaderCenter = teamHeaderCenter + teamHeaderSpace;
  int totalHeaderCenter = scoreHeaderCenter + scoreHeaderSpace;
  int scoreRightLimit = scoreHeaderCenter + scoreScreen->getTextWidth( scoreText );
  int teamRightLimit = teamHeaderCenter + scoreScreen->getTextWidth( teamText );
  int totalRightLimit = totalHeaderCenter + scoreScreen->getTextWidth( totalText );
  int imageCenter = nameHeaderCenter - 65;

  bool clicked = false;
  int mouseX, mouseY;
  double gapMult = 5.5;
  double gapStep = 1.5;

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
    scoreScreen->renderTexture( "nameHeader", nameHeaderCenter, topPadding + gap * 4 );
    scoreScreen->renderTexture( "teamHeader", teamHeaderCenter, topPadding + gap * 4 );
    scoreScreen->renderTexture( "scoreHeader", scoreHeaderCenter, topPadding + gap * 4 );
    scoreScreen->renderTexture( "totalHeader", totalHeaderCenter, topPadding + gap * 4 );
    if ( stage == -1 ) {
      scoreScreen->renderTexture( "skull", stageCompleteTextCenter - scoreScreen->getTextureWidth( "skull" ) - 15, topPadding );
      scoreScreen->renderTexture( "skull", stageCompleteTextCenter + scoreScreen->getTextWidth( stageCompleteText ) + 15, topPadding );
    }

    // Render players score and data
    for( int i = 0; i < playersScoreTableSize; i++ ) {  
      PlayerScore* ps = this->unCliente->getPlayersScoreData()[i];
      string team = ps->team == 1 ? "Alpha" : "Beta";
      string alphaTotalID = "alphaTotal", betaTotalID = "betaTotal"; 
      scoreScreen->renderTexture( string( ps->name ), 
				  nameHeaderCenter, 
				  topPadding + ( gap * ( gapMult + ( i * gapStep ) ) ) );
      scoreScreen->renderTexture( string( ps->name ) + "team", 
				  teamRightLimit - scoreScreen->getTextWidth( team ), 
				  topPadding + ( gap * ( gapMult + ( i * gapStep ) ) ) );
      scoreScreen->renderTexture( string( ps->name ) + "score", 
				  scoreRightLimit - scoreScreen->getTextWidth( to_string( ps->score ) ), 
				  topPadding + ( gap * ( gapMult + ( i * gapStep ) ) ) );
      scoreScreen->renderTexture( string( ps->color ), 
				  imageCenter, 
				  topPadding + ( gap * ( gapMult + ( i * gapStep ) ) ) );
      if( ps->score == maxScore ) {
	scoreScreen->renderTexture( "topScore", totalRightLimit + 15, topPadding - 15 + ( gap * ( gapMult + ( i * gapStep ) ) ) );
	alphaTotalID = "alphaTotalTop", betaTotalID = "betaTotalTop";
      }
      
      if ( ps->team == 1 ) {
	scoreScreen->renderTexture( alphaTotalID, 
				    totalRightLimit - scoreScreen->getTextWidth( to_string( alphaTotal ) ), 
				    topPadding + ( gap * ( gapMult + ( i * gapStep ) ) ) );
      } else {
	scoreScreen->renderTexture( betaTotalID, 
				    totalRightLimit - scoreScreen->getTextWidth( to_string( betaTotal ) ), 
				    topPadding + ( gap * ( gapMult + ( i * gapStep ) ) ) );
      }
    }
  
    scoreScreen->setRenderDrawColor( 160, 160, 160, 255 );
    scoreScreen->renderRectangle( "continue" );
    scoreScreen->renderTexture( "buttonText", buttonTextCenter, 605 );

    if( clicked ) {
      clicked = false;
      if( ( mouseX > buttonCenter ) && ( mouseX < ( buttonCenter + 250 ) )
	  && ( mouseY > 600 ) && ( mouseY < ( 600 + 50 ) ) ) {
	// send ready signal
	this->unCliente->sendStageClearReady();
	break;
      }
    }

    //Update screen
    this->sdl->updateWindow();

    if( timer.tiempoActual() < 1000 / fps ){
      SDL_Delay( ( 1000 / fps ) - timer.tiempoActual() );
    }
  }

  //Disable text input
  SDL_StopTextInput();
  delete remainingHealth;
  delete scoreScreen;
}

void Escenario::loadCoopModeScoreScreen( int stage ) {
  bool runningScreen = true;
  SDL_Event e;
  Timer timer;
  int fps = 10;
  int playersScoreTableSize = this->unCliente->getPlayersScoreData().size() - 1;
  Screen* scoreScreen = new Screen( this->sdl );
  string stageCompleteText = "Stage " + to_string( stage ) + " Complete !!";
  string buttonText = "Continue";
  // win screen
  if ( stage == 0 ) {
    stageCompleteText = "Congratulations. You Win !!";
    buttonText = "Finish";
  // game over screen
  } else if ( stage == -1 ) {
    stageCompleteText = "Game Over";
    buttonText = "Finish";
    // Load skull image
    scoreScreen->loadTexture( "skull", "skull.bmp" );
  }

  string scoreHeaderText = "Coop Mode Score Ranking";
  string nameText = "Name";
  string scoreText = "Score";
  string totalText = "Total";

  // Load max score ribbon
  scoreScreen->loadTexture( "topScore", "topScore.bmp" );

  // Load text
  scoreScreen->loadText( "stageComplete", stageCompleteText, { 53, 167, 84, 255 } );
  scoreScreen->loadText( "scoreText", scoreHeaderText, { 255, 0, 0, 255 } );
  scoreScreen->loadText( "nameHeader", nameText, { 191, 189, 37, 255 } );
  scoreScreen->loadText( "scoreHeader", scoreText, { 191, 189, 37, 255 } );
  scoreScreen->loadText( "totalHeader", totalText, { 191, 189, 37, 255 } );
  scoreScreen->loadText( "buttonText", buttonText, { 0, 0, 0, 255 } );

  // Get max score
  int maxScore = 0;
  for( int i = 0; i < playersScoreTableSize; i++ ) {  
    PlayerScore* ps = this->unCliente->getPlayersScoreData()[i];
    if ( ps->score > maxScore ) {
      maxScore = ps->score;
    }
  }

  // Load ranking score table data 
  for( int i = 0; i < playersScoreTableSize; i++ ) {  
    PlayerScore* ps = this->unCliente->getPlayersScoreData()[i];
    
    if ( ps->score == maxScore ) {
      scoreScreen->loadText( string( ps->name ), string( ps->name ), { 12, 246, 246, 255 } );
      scoreScreen->loadText( string( ps->name ) + "score", to_string( ps->score ), { 12, 246, 246, 255 } );
    } else {
      scoreScreen->loadText( string( ps->name ), string( ps->name ), { 255, 255, 255, 255 } );
      scoreScreen->loadText( string( ps->name ) + "score", to_string( ps->score ), { 255, 255, 255, 255 } );
    }
    // Load plane
    scoreScreen->loadTexture( string( ps->color ), "score/avion_" + string( ps->color ) + ".bmp" );

  }

  int scoreTableSize = this->unCliente->getPlayersScoreData().size();
  int teamTotal = ( this->unCliente->getPlayersScoreData()[ scoreTableSize - 1 ] )->score;

  // load total
  scoreScreen->loadText( "teamTotal", to_string( teamTotal ), { 255, 255, 255, 255 } );
  scoreScreen->loadText( "teamTotalTop", to_string( teamTotal ), { 12, 246, 246, 255 } );

  int buttonWidth = 250;
  int buttonCenter = scoreScreen->getRectCenter( buttonWidth ); 
  int buttonTextCenter = scoreScreen->getTextCenter( buttonText );

  // Load prompts
  scoreScreen->loadRectangle( "continue", buttonCenter, 600, buttonWidth, 50 );

  int gap = scoreScreen->getTextHeight( scoreText );
  int rowPadding = 120;
  int topPadding = 60;
  // Get center positions
  int stageCompleteTextCenter = scoreScreen->getTextCenter( stageCompleteText ); 
  int scoreTextCenter = scoreScreen->getTextCenter( scoreHeaderText );
  int nameHeaderSpace = scoreScreen->getTextWidth( nameText ) + rowPadding;
  int scoreHeaderSpace = scoreScreen->getTextWidth( scoreText ) + rowPadding;
  int scoreTableWidth = nameHeaderSpace + scoreHeaderSpace + scoreScreen->getTextWidth( totalText );
  int nameHeaderCenter = ( scoreScreen->getCanvasWidth() - scoreTableWidth ) / 2;
  int scoreHeaderCenter = nameHeaderCenter + nameHeaderSpace;
  int totalHeaderCenter = scoreHeaderCenter + scoreHeaderSpace;
  int scoreRightLimit = scoreHeaderCenter + scoreScreen->getTextWidth( scoreText );
  int totalRightLimit = totalHeaderCenter + scoreScreen->getTextWidth( totalText );
  int imageCenter = nameHeaderCenter - 65;

  bool clicked = false;
  int mouseX, mouseY;
  double gapMult = 5.5;
  double gapStep = 1.5;

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
    scoreScreen->renderTexture( "nameHeader", nameHeaderCenter, topPadding + gap * 4 );
    scoreScreen->renderTexture( "scoreHeader", scoreHeaderCenter, topPadding + gap * 4 );
    scoreScreen->renderTexture( "totalHeader", totalHeaderCenter, topPadding + gap * 4 );
    if ( stage == -1 ) {
      scoreScreen->renderTexture( "skull", stageCompleteTextCenter - scoreScreen->getTextureWidth( "skull" ) - 15, topPadding );
      scoreScreen->renderTexture( "skull", stageCompleteTextCenter + scoreScreen->getTextWidth( stageCompleteText ) + 15, topPadding );
    }

    // Render players score and data
    for( int i = 0; i < playersScoreTableSize; i++ ) {  
      PlayerScore* ps = this->unCliente->getPlayersScoreData()[i];
      string teamTotalID = "teamTotal";
      scoreScreen->renderTexture( string( ps->name ), 
				  nameHeaderCenter, 
				  topPadding + ( gap * ( gapMult + ( i * gapStep ) ) ) );
      scoreScreen->renderTexture( string( ps->name ) + "score", 
				  scoreRightLimit - scoreScreen->getTextWidth( to_string( ps->score ) ), 
				  topPadding + ( gap * ( gapMult + ( i * gapStep ) ) ) );
      scoreScreen->renderTexture( string( ps->color ), 
				  imageCenter, 
				  topPadding + ( gap * ( gapMult + ( i * gapStep ) ) ) );
      if( ps->score == maxScore ) {
	scoreScreen->renderTexture( "topScore", totalRightLimit + 15, topPadding - 15 + ( gap * ( gapMult + ( i * gapStep ) ) ) );
	teamTotalID = "teamTotalTop";
      }
      
      scoreScreen->renderTexture( teamTotalID, 
				  totalRightLimit - scoreScreen->getTextWidth( to_string( teamTotal ) ), 
				  topPadding + ( gap * ( gapMult + ( i * gapStep ) ) ) );
    }
  
    scoreScreen->setRenderDrawColor( 160, 160, 160, 255 );
    scoreScreen->renderRectangle( "continue" );
    scoreScreen->renderTexture( "buttonText", buttonTextCenter, 605 );

    if( clicked ) {
      clicked = false;
      if( ( mouseX > buttonCenter ) && ( mouseX < ( buttonCenter + 250 ) )
	  && ( mouseY > 600 ) && ( mouseY < ( 600 + 50 ) ) ) {
	// send ready signal
	this->unCliente->sendStageClearReady();
	break;
      }
    }

    //Update screen
    this->sdl->updateWindow();

    if( timer.tiempoActual() < 1000 / fps ){
      SDL_Delay( ( 1000 / fps ) - timer.tiempoActual() );
    }
  }

  //Disable text input
  SDL_StopTextInput();
  delete remainingHealth;
  delete scoreScreen;
}

void Escenario::loadWaitForPlayersScreen() {
  bool runningScreen = true;
  SDL_Event e;
  Timer timer;
  int fps = 10;
  string waitingText = "Esperando Jugadores...";
  Screen* waitingScreen = new Screen( this->sdl );
  waitingScreen->loadTexture( "logo", "windowImages/1942logoPrincipal.bmp" );
  waitingScreen->loadText( "waiting", waitingText, { 255, 255, 255, 255 } );

  int logoCenter = waitingScreen->getTextureCenter( "logo" );
  int waitingCenter = waitingScreen->getTextCenter( waitingText );

  // Enable text input
  SDL_StartTextInput();

  while( runningScreen && !( this->unCliente->stageClearReady ) ) {
    timer.correr();
    while (this->sdl->nextEvent(&e)) {
      if (e.type == SDL_QUIT) {
	runningScreen = false;
	exit(0);
      } 
    }
    // Set window background
    this->sdl->setWindowBG(0, 0, 0);
  
    // Render logo
    waitingScreen->renderTexture( "logo", logoCenter, 90 );
    waitingScreen->renderTexture( "waiting", waitingCenter, 300 );
    
    //Update screen
    this->sdl->updateWindow();

    if( timer.tiempoActual() < 1000 / fps ){
      SDL_Delay( ( 1000 / fps ) - timer.tiempoActual() );
    }
  }

  //Disable text input
  SDL_StopTextInput();
  delete waitingScreen;
}

void Escenario::getPowerUp() {
		Vivible* avion = myControl->getVivible();
		if (avion->tieneHP()) {
			int x = avion->getX();
			int y = avion->getY();
			int xp = x + avion->getAncho()-5;
			int yp = y + avion->getLargo()-5;
			for (map<string, PowerUp*>::iterator it = hPowerUp->mapaPowerUp.begin(); it != hPowerUp->mapaPowerUp.end(); it++) {
				bool touched = false;
				int x2 = it->second->getX();
				int xp2 = x2 + it->second->getAncho();
				int y2 = it->second->getY();
				int yp2 = y2 + it->second->getLargo();
				touched = Colision::is(x, y, xp, yp, x2, y2, xp2, yp2);
				if (touched && it->second->aunVive()) {
					char resp = it->second->activarPowerUp();
					CompanionEvent* ce = new CompanionEvent();
					if (resp == 's') {
						unCliente->sendData(ce->ametralladora(myControl->getVivible()->getId()));
					}
					if (resp == 'd') {
						unCliente->setDestroyEnemys();
						unCliente->sendData(ce->destroy(myControl->getVivible()->getId()));
					}
					if (resp == 'b') {
						this->unCliente->addScoreToPlayer( 250 );
					}
					unCliente->sendData(ce->powerUpDestroy(it->second->getId()));
					delete ce;

					it->second->morir();
				}
			}
		}
}

void Escenario::crearEnemigo(int x, int y) {
	Posicion* p = new Posicion(x, y);
	Enemy* e = new Enemy(escenarioScreen, gRenderer, resolucion, p, gc->avion);
	enemigos.push_back(e);
}

void Escenario::crearFlota(int x, int y) {
    for (int i = 0; i < 5; i++) {
        Posicion* p = new Posicion(x, y);
        Enemy* e = new Enemy(escenarioScreen, gRenderer, resolucion, p, gc->avion);
        e->flota = flota;
        e->posFlota = i;
        enemigos.push_back(e);
    }
    flota++;
}

void Escenario::crearGrande() {
	Posicion* p = new Posicion(400, 900);
	Enemy* e = new Enemy(escenarioScreen, gRenderer, resolucion, p, gc->avion);
	e->flota = -2;
	enemigos.push_back(e);
}

void Escenario::hitEnemy(vector<Vivible*>* disparos) {
		int eliminar = -1;
		for (vector<Vivible*>::iterator it = disparos->begin(); it != disparos->end(); it++) {
			bool touched = false;
			int x = (*it)->posX;
			int xp = x + (*it)->getAncho();
			int y = (*it)->posY;
			int yp = y + (*it)->getLargo();
			for (int var = 0; var < enemigos.size(); ++var) {
				int x2 = enemigos[var]->getX();
				int x2p = x2 + enemigos[var]->getAncho()-5;
				int y2 = enemigos[var]->getY();
				int y2p = y2 + enemigos[var]->getLargo()-5;
				touched = Colision::is(x, y, xp, yp, x2, y2, x2p, y2p);
				if (touched && enemigos[var]->aunVive()) {
					cout << "**** CHOQUE: ENEMIGOS VS MISILES ****" << endl;
					enemigos[var]->recibirMisil((Misil*)*it);
					(*it)->morir();
				}
			}
		}
}

void Escenario::actualizarEnemigos(){
	if(unCliente->destroyEnemys()) {
		this->deleteEnemys();
		unCliente->setNotDestroyEnemys();
	}
	int eliminar = -1;
	for (int i=0; i < enemigos.size(); i++) {
		if (enemigos[i]->aunVive()){
			if(enemigos[i]->flota == -1)
				enemigos[i]->vivirRandom();
			else
				if(enemigos[i]->flota == -2)
					enemigos[i]->vivirGrande();
				else
					enemigos[i]->vivirFlota();
		}else{
			eliminar = i;
		}
	}
	if (eliminar >= 0 ){
		Enemy* objEliminar = enemigos[eliminar];
		delete objEliminar;
		enemigos.erase(enemigos.begin()+eliminar);
	}
}

void Escenario::deleteEnemys() {
	for(int i=0;i<this->enemigos.size();i++) {
		cout<<"Elimino un enemigo"<<endl;
		this->enemigos[i]->morir();
	}
}

void Escenario::planesColision(){
	Vivible* avion = myControl->getVivible();
	bool touched = false;
	if (avion->tieneHP()) {
		int x = avion->getX();
		int y = avion->getY();
		int xp = x + avion->getAncho()-5;
		int yp = y + avion->getLargo()-5;
		for (int var = 0; var < enemigos.size(); ++var) {
			int x2 = enemigos[var]->getX();
			int x2p = x2 + enemigos[var]->getAncho();
			int y2 = enemigos[var]->getY();
			int y2p = y2 + enemigos[var]->getLargo();
			touched = Colision::is(x, y, xp, yp, x2, y2, x2p, y2p);
			if (touched && enemigos[var]->aunVive()) {
				cout << "**** CHOQUE: ENEMIGOS VS MI AVION ****" << endl;
				enemigos[var]->morir();
				myControl->getVivible()->morir();
				this->unCliente->sendPlayerDeath();
			}
		}
	}
}

void Escenario::enemyOtherPlayerColision() {
	map<string, IController*>* othersPlanes = &(this->controllers->mapaControllers);
	int eliminar = -1;
	for (map<string, IController*>::iterator it = othersPlanes->begin(); it != othersPlanes->end(); it++) {
		if (it->second->getVivible()->aunVive()){
			bool touched = false;
			int x = it->second->getVivible()->getX();
			int xp = x + it->second->getVivible()->getAncho();
			int y = it->second->getVivible()->getY();
			int yp = y + it->second->getVivible()->getLargo();
			for (int var = 0; var < enemigos.size(); ++var) {
				int x2 = enemigos[var]->getX();
				int x2p = x2 + enemigos[var]->getAncho();
				int y2 = enemigos[var]->getY();
				int y2p = y2 + enemigos[var]->getLargo();
				touched = Colision::is(x, y, xp, yp, x2, y2, x2p, y2p);
				if (touched && enemigos[var]->aunVive()) {
					cout << "**** CHOQUE: ENEMIGOS VS AVION DE OTRO JUGADOR ****" << endl;
					enemigos[var]->morir();
					it->second->getVivible()->morir();
				}
			}
			this->hitEnemy(&(it->second->getMissiles()->getVivibles()->vectorObjetos));
		}
	}
}

/* SOLO EN MODO PRACTICA*/
void Escenario::enemyOtherPlayerMissileColision() {
	map<string, IController*>* othersPlanes = &(this->controllers->mapaControllers);
	for (map<string, IController*>::iterator it = othersPlanes->begin(); it != othersPlanes->end(); it++) {
		if (it->second->getVivible()->aunVive()){
			this->hitEnemy(&(it->second->getMissiles()->getVivibles()->vectorObjetos));
		}
	}
}

