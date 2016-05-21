#include "Game.h"
#include "controller/Timer.h"
#include "view/Screen.h"
#include "../xml/parser/GameParser.h"

Game::Game( uint32_t sdlFlags ) {
  this->sdlHandler = new XM_SDL( sdlFlags );
  this->running = false;
  this->windowWidth = 800;
  this->windowHeight = 600;
  this->windowTitle = "1942MP Arcade";
  this->fps = 10;
  this->serverIP = "127.0.0.1";
  this->serverPort = "8000";
  this->clientId = "";
  this->planeId = "";
}

Game::~Game() {
  delete sdlHandler;
}

void Game::crearGameConfHardcodeada() {
  gc = GameParser::parse("gameconf.xml");
  //	EscenarioConf* escenarioConf = new EscenarioConf();
  //	escenarioConf->ancho = 800;
  //	escenarioConf->alto = 600;
  //	escenarioConf->fondo = "agua";

  //TODO: esto comentado se puede probar. si se quiere probar, descomentarlo
  //	ElementoConf* el1 = new ElementoConf();
  //	el1->spriteID = "isla1";
  //	el1->x = 200;
  //	el1->y = 200;
  //
  //	ElementoConf* el2 = new ElementoConf();
  //	el2->spriteID = "portaavionGFEDGS";
  //	el2->x = 600;
  //	el2->y = 600;
  //
  //	escenarioConf->elementos.push_back(el1);
  //	escenarioConf->elementos.push_back(el2);

  //	gc->escenario = escenarioConf;
}

void Game::recorroConfig(GameConf* game){
/*
	cout<<"CONFIGURACION"<<endl;
	cout<<"AVION"<<endl;
	cout<<game->avion->avionSpriteID<<endl;
	cout<<game->avion->disparosSpriteID<<endl;
	cout<<game->avion->velocidadDesplazamiento<<endl;
	cout<<game->avion->velocidadDisparos<<endl;
//	cout<<game->avion->vueltaSpriteID<<endl;
	cout<<"Escenario"<<endl;
	cout<<game->escenario->fondo<<endl;
	cout<<"MAX CLIENTE"<<endl;
	cout<<game->maxClients<<endl;
	cout<<"ELEMENTOS"<<endl;
	for (int var = 0; var < game->elementos.size(); ++var) {
		cout<<game->elementos[var]->spriteID<<endl;
	}
	cout<<"SPRITES"<<endl;
	for (int var = 0; var < game->sprites.size(); ++var) {
		cout<<game->sprites[var]->id<<endl;
		cout<<game->sprites[var]->path<<endl;
	}
	*/
}

void Game::cargarEscenario() {
  SDL_Event* exitEven = new SDL_Event();
  exitEven->key.keysym.sym = SDLK_r;
  bool inicia =true;
  while (exitEven->key.keysym.sym == SDLK_r || this->unCliente->reset) {
	if (!inicia){
		this->unCliente->reinicia = true;
		cout<<"Se reinicio"<<endl;
	    this->unCliente->setConfigComplete(false);
		this->unCliente->sendGetConfig();
		while(!unCliente->isConfigComplete()){}
	}
    this->unCliente->reset =false;
//    crearGameConfHardcodeada();
//    escenario = new Escenario(gc);

	recorroConfig(this->unCliente->getConfig());
    escenario = new Escenario(this->unCliente->getConfig());
    escenario->setClient(unCliente);

    for( int i = 0; i < this->unCliente->getPlayers().size(); i++) {
      if( this->clientId == this->unCliente->getPlayers()[i]->name ) {
    	  escenario->configurarMiAvion(this->unCliente->getPlayers()[i]);
      } else {
    	  escenario->configurarAvionAmigo(this->unCliente->getPlayers()[i]);
      }
    }

    unCliente->setHandler(escenario->getHandler());
    exitEven = escenario->run();
    if( !( this->unCliente->isConnected() ) ) {
      this->loadTimeoutScreen();
      delete escenario;
      break;
    }
    delete escenario;
    this->unCliente->resetConfig();
    inicia = false;
  }
  // Notify server of player disconnection
  this->unCliente->sendPlayerDisconnection();
}

void Game::start() {
  if( this->sdlHandler->createWindow( this->windowTitle.c_str(), this->windowWidth, this->windowHeight ) ) {
    this->running = true;
    this->loadConnectionScreen();
    this->cargarEscenario();
  }
}

void Game::setWindowWidth( int width ) {
  this->windowWidth = width;
}

void Game::setWindowHeight( int height ) {
  this->windowHeight = height;
}

void Game::setWindowTitle( string title ) {
  this->windowTitle = title;
}

void Game::loadConnectionScreen() {
  SDL_Event e;
  Timer timer;
  string acceptButtonText = "ACEPTAR";

  Screen* initialScreen = new Screen( this->sdlHandler );
  initialScreen->loadTexture( "logo", "windowImages/1942logoPrincipal.bmp" );
  initialScreen->loadText( "serverIP", this->serverIP );
  initialScreen->loadText( "serverPort", this->serverPort );
  initialScreen->loadText( "accept", "ACEPTAR" );

  int logoCenter = ( this->windowWidth - initialScreen->getTextureWidth( "logo" ) ) / 2;
  int promptCenter = logoCenter - 20;
  int buttonCenter = promptCenter + 15;
  int textCenter = promptCenter + 20;
  int buttonTextCenter = buttonCenter + ( ( 230 - initialScreen->getTextureWidth( "accept" ) ) / 2 );
  int IPPromptOutline = 300, IPPromptOutline2 = 301, IPPromptOutline3 = 302;
  int portPromptOutline = 375, portPromptOutline2 = 376, portPromptOutline3 = 377;
  int mouseX, mouseY;

  bool runningScreen = true;
  bool firstPromptSelected = true;
  bool clicked = false;

  // Create prompts
  initialScreen->loadRectangle( "promptIP", promptCenter, 300, 260, 50 );
  initialScreen->loadRectangle( "promptPort", promptCenter, 375, 260, 50 );
  initialScreen->loadRectangle( "button", promptCenter + 15, 475, 230, 50 );

  // Enable text input
  SDL_StartTextInput();

  while( runningScreen ) {
    timer.correr();
    bool renderText = false;

    // Get events
    while( this->sdlHandler->nextEvent( &e ) ) {
      if( e.type == SDL_QUIT ) {
	runningScreen = false;
	this->running = false;
      } //Special key input
      else if( e.type == SDL_KEYDOWN ) {
	// Handle Tab
	if( e.key.keysym.sym == SDLK_TAB ) {
	  firstPromptSelected = !firstPromptSelected;
	}
	//Handle backspace
	else if( firstPromptSelected ) {
	  if( e.key.keysym.sym == SDLK_BACKSPACE && this->serverIP.length() > 0 ) {
	    //lop off character
	    this->serverIP.pop_back();
	    renderText = true;
	  }
	} else if( e.key.keysym.sym == SDLK_BACKSPACE && this->serverPort.length() > 0 ) {
	  //lop off character
	  this->serverPort.pop_back();
	  renderText = true;
	}
      }
      //Special text input event
      else if( e.type == SDL_TEXTINPUT )
      {
	//Not copy or pasting
	if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' )
	      && SDL_GetModState() & KMOD_CTRL ) )
	{
	  //Append character
	  if( firstPromptSelected ) {
	    // Character limit for IP
	    if( this->serverIP.length() < 15 )
	      this->serverIP += e.text.text;
	  } else {
	    // Character limit for port
	    if( this->serverPort.length() < 5 )
	      this->serverPort += e.text.text;
	  }
	  renderText = true;
	}
      }
      // Handle button events
      else if( e.button.type == SDL_MOUSEBUTTONDOWN ) {
	if( e.button.button == SDL_BUTTON_LEFT ) {
	  clicked = true;
	  // Get the mouse offsets
	  mouseX = e.button.x;
	  mouseY = e.button.y;
	}
      }
    }

    //Rerender text if needed
    if( renderText )
    {
      if( firstPromptSelected ) {
	//Text is not empty
	if( this->serverIP != "" )
	{
	  //Render new text
	  initialScreen->loadText( "serverIP", this->serverIP );
	}
	//Text is empty
	else
	{
	  //Render space texture
	  initialScreen->loadText( "serverIP", " " );
	}
      } else {

	//Text is not empty
	if( this->serverPort != "" )
	{
	  //Render new text
	  initialScreen->loadText( "serverPort", this->serverPort );
	}
	//Text is empty
	else
	{
	  //Render space texture
	  initialScreen->loadText( "serverPort", " " );
	}
      }
    }

    // Set window background
    sdlHandler->setWindowBG(0, 0, 0);

    // Render logo
    initialScreen->renderTexture( "logo", logoCenter, 90 );

    // Set prompt color
    initialScreen->setRenderDrawColor( 255, 255, 255, 255 );
    // Render prompts
    initialScreen->renderRectangle( "promptIP" );
    initialScreen->renderRectangle( "promptPort" );

    initialScreen->setRenderDrawColor( 160, 160, 160, 255 );
    if( clicked ) {
      clicked = false;
      if( ( mouseX > buttonCenter ) && ( mouseX < ( buttonCenter + 230 ) )
	  && ( mouseY > 475 ) && ( mouseY < ( 475 + 50 ) ) ) {
	runningScreen = false;
	//Disable text input
	SDL_StopTextInput();
	delete initialScreen;
	this->loadValidationScreen();
	break;
      }
    }
    initialScreen->renderRectangle( "button" );

    // Set outline color
    initialScreen->setRenderDrawColor( 19, 144, 27, 255 );

    if( firstPromptSelected ) {
      initialScreen->loadRectangle( "outline", promptCenter, IPPromptOutline, 260, 50 );
      initialScreen->loadRectangle( "outline2", promptCenter + 1, IPPromptOutline2, 258, 48 );
      initialScreen->loadRectangle( "outline3", promptCenter + 2, IPPromptOutline3, 256, 46 );
    } else {
      initialScreen->loadRectangle( "outline", promptCenter, portPromptOutline, 260, 50 );
      initialScreen->loadRectangle( "outline2", promptCenter + 1, portPromptOutline2, 258, 48 );
      initialScreen->loadRectangle( "outline3", promptCenter + 2, portPromptOutline3, 256, 46 );
    }

    // Render outlines
    initialScreen->renderRectangle( "outline", true );
    initialScreen->renderRectangle( "outline2", true );
    initialScreen->renderRectangle( "outline3", true );

    // Render text textures
    initialScreen->renderTexture( "serverIP", textCenter, 305 );
    initialScreen->renderTexture( "serverPort", textCenter, 380 );
    initialScreen->renderTexture( "accept", buttonTextCenter, 480 );

    //Update screen
    sdlHandler->updateWindow();

    if( timer.tiempoActual() < 1000 / this->fps ){
      SDL_Delay( ( 1000 / this->fps ) - timer.tiempoActual() );
    }
  }
}

void Game::loadValidationScreen() {
  if( !( this->running ) ) return;
  bool runningScreen = true;
  SDL_Event e;
  Timer timer;
  string connectingText = "Conectando al servidor...";
  string connectedText = "Se ha conectado al servidor";
  string failureText = "No se pudo conectar al servidor";

  Screen* validationScreen = new Screen( this->sdlHandler );
  validationScreen->loadTexture( "logo", "windowImages/1942logoPrincipal.bmp" );
  validationScreen->loadText( "connecting", connectingText, { 255, 255, 255, 255 } );
  validationScreen->loadText( "connected", connectedText, { 255, 255, 255, 255 } );
  validationScreen->loadText( "failure", failureText, { 255, 255, 255, 255 } );

  int logoCenter = ( this->windowWidth - validationScreen->getTextureWidth( "logo" ) ) / 2;
  int promptCenter = logoCenter - 20;
  int textCenter = promptCenter;

  string continueButtonText = "CONTINUAR";
  validationScreen->loadText( "continue", continueButtonText, {255,255,255,255} );
  string backButtonText = "VOLVER";
  validationScreen->loadText( "back", backButtonText, {255,255,255,255} );
  validationScreen->loadRectangle( "button", promptCenter + 15, 475, 230, 50 );
  int buttonCenter = promptCenter + 15;
  int continueTextCenter = buttonCenter + ( ( 230 - validationScreen->getTextureWidth( "continue" ) ) / 2 );
  int backTextCenter = buttonCenter + ( ( 230 - validationScreen->getTextureWidth( "back" ) ) / 2 );

  // Enable text input
  SDL_StartTextInput();

  //Enviar al servidor IP y puerto
  this->unCliente = new Client(serverIP,serverPort);

  bool connected = false;
  bool connectionFailed = false;
  bool clicked = false;
  int mouseX = 0;
  int mouseY = 0;

  while( runningScreen ) {
    timer.correr();
    bool renderText = false;
    // Get events
    while( this->sdlHandler->nextEvent( &e ) ) {
      if( e.type == SDL_QUIT ) {
	runningScreen = false;
	this->running = false;
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
    this->sdlHandler->setWindowBG(0, 0, 0);

    // Render logo
    validationScreen->renderTexture( "logo", logoCenter, 90 );

    if (clicked) {
      clicked = false;
      if ((mouseX > buttonCenter) && (mouseX < (buttonCenter + 230))
	  && (mouseY > 475) && (mouseY < (475 + 50))) {
	runningScreen = false;
	//Disable text input
	SDL_StopTextInput();
	delete validationScreen;
	if (connectionFailed){
	  this->loadConnectionScreen();
	} else {
	  bool endSelectingPlane = false;
	  bool primerIntento = true;
	  while(!endSelectingPlane){
	    PlanesActives* planes = this->unCliente->getPlanesActives();
	    this->bluePlaneActive = planes->blue;
	    this->redPlaneActive = planes->red;
	    this->greenPlaneActive = planes->green;
	    this->yellowPlaneActive = planes->yellow;
	    this->clientId = "";
	    this->planeId = "";
	    if (primerIntento){
	    	this->loadselectionPlane(true);
	    }else{
	    	this->loadselectionPlane(this->unCliente->isPlayerOk());
	    }
	    primerIntento = false;
	    endSelectingPlane = this->unCliente->isPlayerOk();
	  }
	  this->loadWaitingGame();
	}
	break;
      }
    }

    // Render text textures
    if (!connected){
      validationScreen->renderTexture( "connecting", textCenter, 305 );
    } else {
      if (connectionFailed) {
	validationScreen->renderTexture( "failure", textCenter, 305 );
	validationScreen->setRenderDrawColor( 234, 25 ,25 , 255 );
	validationScreen->renderRectangle( "button" );
	validationScreen->renderTexture( "back", backTextCenter, 480 );
      } else {
	validationScreen->renderTexture( "connected", textCenter, 305 );
	validationScreen->setRenderDrawColor( 19, 144, 27, 255 );
	validationScreen->renderRectangle( "button" );
	validationScreen->renderTexture( "continue", continueTextCenter, 480 );
      }
    }


    //Update screen
    this->sdlHandler->updateWindow();

    if( timer.tiempoActual() < 1000 / this->fps ){
      SDL_Delay( ( 1000 / this->fps ) - timer.tiempoActual() );
    }

    if (!connected) {
      connectionFailed = !this->unCliente->connectToServer();
      connected = true;
    }
  }
}

void Game::loadTimeoutScreen() {
  if( !( this->running ) ) return;
  bool runningScreen = true;
  SDL_Event e;
  Timer timer;
  string timeoutText = "Se ha perdido la conexion con el servidor";

  Screen* timeoutScreen = new Screen( this->sdlHandler );
  timeoutScreen->loadTexture( "logo", "windowImages/1942logoPrincipal.bmp" );
  timeoutScreen->loadText( "timeout", timeoutText, { 255, 0, 0, 255 } );

  int logoCenter = ( this->windowWidth - timeoutScreen->getTextureWidth( "logo" ) ) / 2;
  int textCenter = logoCenter - 150;

  // Enable text input
  SDL_StartTextInput();

  while( runningScreen ) {
    timer.correr();
    // Get events
    while( this->sdlHandler->nextEvent( &e ) ) {
      if( e.type == SDL_QUIT ) {
	runningScreen = false;
	this->running = false;
	break;
      }
    }
    // Set window background
    this->sdlHandler->setWindowBG(0, 0, 0);

    // Render logo
    timeoutScreen->renderTexture( "logo", logoCenter, 90 );

    // Render text textures
    timeoutScreen->renderTexture( "timeout", textCenter, 305 );

    //Update screen
    this->sdlHandler->updateWindow();

    if( timer.tiempoActual() < 1000 / this->fps ){
      SDL_Delay( ( 1000 / this->fps ) - timer.tiempoActual() );
    }
  }

  //Disable text input
  SDL_StopTextInput();
  delete timeoutScreen;
}

void Game::setPlanesActives(bool blue,bool red,bool green,bool yellow){
  this->bluePlaneActive = blue;
  this->redPlaneActive = red;
  this->greenPlaneActive = green;
  this->yellowPlaneActive = yellow;
}

void Game::loadselectionPlane(bool selectedOk) {
  SDL_Event e;
  Timer timer;

  Screen* initialScreen = new Screen(this->sdlHandler);

  //	this->setInitialScreen()
  initialScreen->loadTexture("logo", "windowImages/1942logoPrincipal.bmp");
  initialScreen->loadText("clientText", "  Jugador: ", { 0, 255, 100 });
  initialScreen->loadText("planeText", " Avion: ", { 0, 255, 100 });
  initialScreen->loadText("accept", "CONTINUAR");
  initialScreen->loadText("rechazo", " Rechazado por maximo clientes", { 255, 0, 0 });

  if (bluePlaneActive){
    initialScreen->loadTexture("bluePlane", "avionAzul.bmp");
  }
  if (redPlaneActive){
    initialScreen->loadTexture("redPlane", "avionRojo.bmp");
  }
  if (greenPlaneActive){
    initialScreen->loadTexture("greenPlane", "avionVerde.bmp");
  }
  if (yellowPlaneActive){
    initialScreen->loadTexture("yellowPlane", "avionAmarillo.bmp");
  }

  int logoCenter = (this->windowWidth - initialScreen->getTextureWidth("logo")) / 2;
  int promptCenter = logoCenter - 20;
  int buttonCenter = promptCenter + 15;
  int textCenter = promptCenter + 20;
  int textoIzquierda = 100;
  int buttonTextCenter = buttonCenter	+ ((230 - initialScreen->getTextureWidth("accept")) / 2);
  int inputClientIdPosY = 250;
  int inputPlaneIdPosY = 350;
  int clientIdPromptOutline = inputClientIdPosY, clientIdPromptOutline2 =	inputClientIdPosY + 1, clientIdPromptOutline3 = inputClientIdPosY + 2;
  int bluePlanePromptOutline = inputPlaneIdPosY, bluePlanePromptOutline2 = inputPlaneIdPosY + 1, bluePlanePromptOutline3 = inputPlaneIdPosY + 2;
  int redPlanePromptOutline = inputPlaneIdPosY, redPlanePromptOutline2 = inputPlaneIdPosY + 1, redPlanePromptOutline3 = inputPlaneIdPosY + 2;
  int greenPlanePromptOutline = inputPlaneIdPosY, greenPlanePromptOutline2 = inputPlaneIdPosY + 1, greenPlanePromptOutline3 = inputPlaneIdPosY + 2;
  int yellowPlanePromptOutline = inputPlaneIdPosY, yellowPlanePromptOutline2 = inputPlaneIdPosY + 1, yellowPlanePromptOutline3 = inputPlaneIdPosY + 2;

  int mouseX, mouseY;

  bool runningScreen = true;
  bool firstPromptSelected = true;
  bool bluePromptSelected = false;
  bool redPromptSelected = false;
  bool greenPromptSelected = false;
  bool yellowPromptSelected = false;
  bool clicked = false;

  // Create prompts
  initialScreen->loadRectangle("promptClientId", promptCenter,inputClientIdPosY, 260, 50);
  initialScreen->loadRectangle("button", promptCenter + 15, 475, 230, 50);

  // Enable text input
  SDL_StartTextInput();

  while (runningScreen) {
    timer.correr();
    bool renderText = false;

    // Get events
    while (this->sdlHandler->nextEvent(&e)) {
      if (e.type == SDL_QUIT) {
	runningScreen = false;
	this->running = false;
	exit(1);
      } //Special key input
      else if (e.type == SDL_KEYDOWN) {
	// Handle Tab
	if (e.key.keysym.sym == SDLK_TAB && bluePromptSelected) {

	  bluePromptSelected = !bluePromptSelected;
	  if (this->redPlaneActive){
	    redPromptSelected = !redPromptSelected;
	  } else if (this->greenPlaneActive){
	    greenPromptSelected = !greenPromptSelected;
	  } else if (this->yellowPlaneActive){
	    yellowPromptSelected = !yellowPromptSelected;
	  } else{
	    bluePromptSelected = !bluePromptSelected;
	  }

	} else 	if (e.key.keysym.sym == SDLK_TAB && redPromptSelected) {

	  redPromptSelected = !redPromptSelected;
	  if (this->greenPlaneActive){
	    greenPromptSelected = !greenPromptSelected;
	  } else if (this->yellowPlaneActive){
	    yellowPromptSelected = !yellowPromptSelected;
	  } else if (this->bluePlaneActive){
	    bluePromptSelected = !bluePromptSelected;
	  } else {
	    redPromptSelected = !redPromptSelected;
	  }

	} else if (e.key.keysym.sym == SDLK_TAB && greenPromptSelected) {

	  greenPromptSelected = !greenPromptSelected;
	  if (this->yellowPlaneActive){
	    yellowPromptSelected = !yellowPromptSelected;
	  } else if (this->bluePlaneActive){
	    bluePromptSelected = !bluePromptSelected;
	  } else if (this->redPlaneActive){
	    redPromptSelected = !redPromptSelected;
	  }else {
	    greenPromptSelected = !greenPromptSelected;
	  }

	} else 	if (e.key.keysym.sym == SDLK_TAB && yellowPromptSelected) {

	  yellowPromptSelected = !yellowPromptSelected;
	  if (this->bluePlaneActive){
	    bluePromptSelected = !bluePromptSelected;
	  } else if (this->redPlaneActive){
	    redPromptSelected = !redPromptSelected;
	  }else if (this->greenPlaneActive){
	    greenPromptSelected = !greenPromptSelected;
	  } else {
	    yellowPromptSelected = !yellowPromptSelected;
	  }
	}
	//Handle backspace
	else if (firstPromptSelected) {
	  if (e.key.keysym.sym == SDLK_BACKSPACE && this->clientId.length() > 0) {
	    //lop off character
	    this->clientId.pop_back();
	    renderText = true;
	  }
	} else if (e.key.keysym.sym == SDLK_BACKSPACE && this->planeId.length() > 0) {
	  //lop off character
	  this->planeId.pop_back();
	  renderText = true;
	} else if(e.key.keysym.sym == SDL_SCANCODE_KP_ENTER){
	  runningScreen = false;
	}
      }
      //Special text input event
      else if (e.type == SDL_TEXTINPUT) {
	//Not copy or pasting
	if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C')
	      && (e.text.text[0] == 'v' || e.text.text[0] == 'V')
	      && SDL_GetModState() & KMOD_CTRL)) {
	  //Append character
	  if (firstPromptSelected) {
	    // Character limit for IP
	    if (this->clientId.length() < 15)
	      this->clientId += e.text.text;
	  } else {
	    // Character limit for port
	    if (this->planeId.length() < 5)
	      this->planeId += e.text.text;
	  }
	  renderText = true;
	}
      }
      // Handle button events
      else if (e.button.type == SDL_MOUSEBUTTONDOWN) {
	if (e.button.button == SDL_BUTTON_LEFT) {
	  clicked = true;
	  // Get the mouse offsets
	  mouseX = e.button.x;
	  mouseY = e.button.y;
	}
      }
    }

    //Rerender text if needed
    if (renderText) {
      //Text is not empty
      if (this->clientId != "") {
	//Render new text
	initialScreen->loadText("clientId", this->clientId);
      }
    }

    // Set window background
    sdlHandler->setWindowBG(0, 0, 0);

    int inputBluePlaneIdPosX = 250;
    int inputRedPlaneIdPosX = 330;
    int inputGreenPlaneIdPosX = 410;
    int inputYellowPlaneIdPosX = 490;

    // Render logo
    initialScreen->renderTexture("logo", logoCenter, 90);
    if (bluePlaneActive){
      initialScreen->renderTexture("bluePlane", inputBluePlaneIdPosX, inputPlaneIdPosY);
    }
    if (redPlaneActive){
      initialScreen->renderTexture("redPlane", inputRedPlaneIdPosX, inputPlaneIdPosY);
    }
    if (greenPlaneActive){
      initialScreen->renderTexture("greenPlane", inputGreenPlaneIdPosX, inputPlaneIdPosY);
    }
    if (yellowPlaneActive){
      initialScreen->renderTexture("yellowPlane", inputYellowPlaneIdPosX, inputPlaneIdPosY);
    }

    // Set prompt color
    initialScreen->setRenderDrawColor(255, 255, 255, 255);
    // Render prompts
    initialScreen->renderRectangle("promptClientId");

    initialScreen->setRenderDrawColor(160, 160, 160, 255);

    if (clicked) {
      clicked = false;
      if ((mouseX > buttonCenter) && (mouseX < (buttonCenter + 230))	&& (mouseY > 475) && (mouseY < (475 + 50))) {
	if(bluePromptSelected){
	  this->planeId = "azul";
	} else if(redPromptSelected){
	  this->planeId = "rojo";
	} else if(greenPromptSelected){
	  this->planeId = "verde";
	} else if(yellowPromptSelected){
	  this->planeId = "amarillo";
	}
	if(this->clientId != "" && this->planeId != ""){
	  this->sendDataPlayer();
	  runningScreen = false;
	}
      } else if ((mouseX > inputBluePlaneIdPosX ) && (mouseX < (inputBluePlaneIdPosX + 60))
	  && (mouseY > inputPlaneIdPosY) && (mouseY < (inputPlaneIdPosY+60)) && this->bluePlaneActive) {
	bluePromptSelected = true;
	redPromptSelected = false;
	greenPromptSelected = false;
	yellowPromptSelected = false;
      } else if ((mouseX > inputRedPlaneIdPosX ) && (mouseX < (inputRedPlaneIdPosX + 60))
	  && (mouseY > inputPlaneIdPosY) && (mouseY < (inputPlaneIdPosY+60)) && this->redPlaneActive) {
	bluePromptSelected = false;
	redPromptSelected = true;
	greenPromptSelected = false;
	yellowPromptSelected = false;
      } else if ((mouseX > inputGreenPlaneIdPosX ) && (mouseX < (inputGreenPlaneIdPosX + 60))
	  && (mouseY > inputPlaneIdPosY) && (mouseY < (inputPlaneIdPosY+60)) && this->greenPlaneActive) {
	bluePromptSelected = false;
	redPromptSelected = false;
	greenPromptSelected = true;
	yellowPromptSelected = false;
      } else if ((mouseX > inputYellowPlaneIdPosX ) && (mouseX < (inputYellowPlaneIdPosX + 60))
	  && (mouseY > inputPlaneIdPosY) && (mouseY < (inputPlaneIdPosY+60)) && this->yellowPlaneActive) {
	bluePromptSelected = false;
	redPromptSelected = false;
	greenPromptSelected = false;
	yellowPromptSelected = true;
      }
    }
    initialScreen->renderRectangle("button");

    // Set outline color
    initialScreen->setRenderDrawColor(19, 144, 27, 255);
    initialScreen->loadRectangle("outline", promptCenter,clientIdPromptOutline, 260, 50);
    initialScreen->loadRectangle("outline2", promptCenter + 1,clientIdPromptOutline2, 258, 48);
    initialScreen->loadRectangle("outline3", promptCenter + 2,clientIdPromptOutline3, 256, 46);

    if (bluePromptSelected){
      initialScreen->loadRectangle("planeOutline", inputBluePlaneIdPosX,bluePlanePromptOutline, 60, 60);
      initialScreen->loadRectangle("planeOutline2", inputBluePlaneIdPosX + 1,bluePlanePromptOutline2, 58, 58);
      initialScreen->loadRectangle("planeOutline3", inputBluePlaneIdPosX + 2,bluePlanePromptOutline3, 56, 56);
    } else if (redPromptSelected){
      initialScreen->loadRectangle("planeOutline", inputRedPlaneIdPosX,redPlanePromptOutline, 60, 60);
      initialScreen->loadRectangle("planeOutline2", inputRedPlaneIdPosX + 1,redPlanePromptOutline2, 58, 58);
      initialScreen->loadRectangle("planeOutline3", inputRedPlaneIdPosX + 2,redPlanePromptOutline3, 56, 56);
    } else if (greenPromptSelected){
      initialScreen->loadRectangle("planeOutline", inputGreenPlaneIdPosX,greenPlanePromptOutline, 60, 60);
      initialScreen->loadRectangle("planeOutline2", inputGreenPlaneIdPosX + 1,greenPlanePromptOutline2, 58, 58);
      initialScreen->loadRectangle("planeOutline3", inputGreenPlaneIdPosX + 2,greenPlanePromptOutline3, 56, 56);
    } else if (yellowPromptSelected){
      initialScreen->loadRectangle("planeOutline", inputYellowPlaneIdPosX,yellowPlanePromptOutline, 60, 60);
      initialScreen->loadRectangle("planeOutline2", inputYellowPlaneIdPosX + 1,yellowPlanePromptOutline2, 58, 58);
      initialScreen->loadRectangle("planeOutline3", inputYellowPlaneIdPosX + 2,yellowPlanePromptOutline3, 56, 56);
    }

    // Render outlines
    initialScreen->renderRectangle("outline", true);
    initialScreen->renderRectangle("outline2", true);
    initialScreen->renderRectangle("outline3", true);
    if (bluePromptSelected || redPromptSelected || greenPromptSelected || yellowPromptSelected){
      initialScreen->renderRectangle("planeOutline", true);
      initialScreen->renderRectangle("planeOutline2", true);
      initialScreen->renderRectangle("planeOutline3", true);

    }

    // Render text textures
    if (this->clientId != ""){
      initialScreen->renderTexture("clientId", textCenter, inputClientIdPosY);
    }

    initialScreen->renderTexture("accept", buttonTextCenter, 475);
    initialScreen->renderTexture("clientText", textoIzquierda,	inputClientIdPosY);
    initialScreen->renderTexture("planeText", textoIzquierda, inputPlaneIdPosY);
    if (!selectedOk){
    	initialScreen->renderTexture("rechazo", 0, inputClientIdPosY+160 );
    }
    //Update screen
    sdlHandler->updateWindow();

    if (timer.tiempoActual() < 1000 / this->fps) {
      SDL_Delay((1000 / this->fps) - timer.tiempoActual());
    }
  }
  //Disable text input
  SDL_StopTextInput();
  delete initialScreen;
}


void Game::sendDataPlayer(){
  this->jugador = new PlayerData;
  strcpy( jugador->name, this->clientId.c_str() );
  strcpy( jugador->color, this->planeId.c_str() );
  this->unCliente->sendData(jugador);
}


void Game::loadWaitingGame() {
  SDL_Event e;
  string connectingText = "Esperando Jugadores...";
  Screen* waitingScreen = new Screen( this->sdlHandler );
  waitingScreen->loadTexture( "logo", "windowImages/1942logoPrincipal.bmp" );
  waitingScreen->loadText( "connecting", connectingText, { 255, 255, 255, 255 } );

  int logoCenter = ( this->windowWidth - waitingScreen->getTextureWidth( "logo" ) ) / 2;
  int promptCenter = logoCenter - 20;

  // Enable text input
  SDL_StartTextInput();
  // Set window background
  this->sdlHandler->setWindowBG(0, 0, 0);
  // Render logo
  waitingScreen->renderTexture( "logo", logoCenter, 90 );
  waitingScreen->renderTexture("connecting", logoCenter, 300 );
  //Update screen
  this->sdlHandler->updateWindow();
  while( !unCliente->allPlayersReady()){

    while (this->sdlHandler->nextEvent(&e)) {
      if (e.type == SDL_QUIT) {
	this->running = false;
	exit(1);
      } //Special key input
    }


  }

  //Disable text input
  SDL_StopTextInput();
  delete waitingScreen;
}
