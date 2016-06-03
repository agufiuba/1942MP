#include "Game.h"
#include "controller/Timer.h"
#include "view/Screen.h"
#include "../xml/parser/GameParser.h"

/**
 * Clase de Pantallas de configuraciones iniciales
 * Circuito:
 * 			loadConnectionScreen
 * 			loadValidationScreen
 * 			loadModeGameScreen
 * 			loadselectionPlane
 * 			loadWaitingGame
 */

Game::Game( uint32_t sdlFlags ) {
  this->sdlHandler = new XM_SDL( sdlFlags );
  this->running = false;
  gc = GameParser::parse("gameconf.xml");
  Resolucion::INSTANCE(gc->escenario->ancho, gc->escenario->alto);
  this->windowWidth = Resolucion::INSTANCE()->getWidthScreen();
  this->windowHeight = Resolucion::INSTANCE()->getHeightScreen();
  this->windowTitle = "1942MP Arcade";
  this->fps = 10;
  this->serverIP = "127.0.0.1";
  this->serverPort = "8000";
  this->clientId = "";
  this->planeId = "";
  this->player = NULL;
  this->team = 0;
}

Game::~Game() {
  delete sdlHandler;
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
    escenario = new Escenario(this->unCliente->getConfig(), sdlHandler);
    escenario->setClient(unCliente);

    for( int i = 0; i < this->unCliente->getPlayers().size(); i++) {
      PlayerData* pData = this->unCliente->getPlayers()[i];
      if( this->clientId == pData->name ) {
	  this->player = new Player( pData->name, pData->color, pData->x, pData->y, pData->team);
    	  escenario->configurarMiAvion( pData );
      } else {
    	  escenario->configurarAvionAmigo( pData );
      }
    }

    escenario->setPlayer( this->player );
    this->unCliente->setPlayer( this->player );
    unCliente->setHandler(escenario->getHandler());
    this->unCliente->requestPlayerScore();
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

  Screen* initialScreen = new Screen( this->sdlHandler );

  // Load logo and text
  initialScreen->loadTexture( "logo", "windowImages/1942logoPrincipal.bmp" );
  initialScreen->loadText( "serverIP", this->serverIP );
  initialScreen->loadText( "serverPort", this->serverPort );
  initialScreen->loadText( "accept", "ACEPTAR" );

  int promptWidth = initialScreen->getTextWidth( "999.999.999.999" ) + 40;
  int promptCenter = initialScreen->getRectCenter( promptWidth ); 
  int buttonWidth = 250;
  int buttonCenter = initialScreen->getRectCenter( buttonWidth ); 

  // Load prompts
  initialScreen->loadRectangle( "promptIP", promptCenter, 300, promptWidth, 50 );
  initialScreen->loadRectangle( "promptPort", promptCenter, 375, promptWidth, 50 );
  initialScreen->loadRectangle( "button", buttonCenter, 475, buttonWidth, 50 );

  int logoCenter = initialScreen->getTextureCenter( "logo" ); 
  int textCenter = promptCenter + 20;
  int buttonTextCenter = initialScreen->getTextCenter( "ACEPTAR" ); 
  int IPPromptOutline = 300, IPPromptOutline2 = 301, IPPromptOutline3 = 302;
  int portPromptOutline = 375, portPromptOutline2 = 376, portPromptOutline3 = 377;
  int mouseX, mouseY;

  bool runningScreen = true;
  bool firstPromptSelected = true;
  bool clicked = false;

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
      if( ( mouseX > buttonCenter ) && ( mouseX < ( buttonCenter + 250 ) )
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
      initialScreen->loadRectangle( "outline", promptCenter, IPPromptOutline, promptWidth, 50 );
      initialScreen->loadRectangle( "outline2", promptCenter + 1, IPPromptOutline2, promptWidth - 2, 48 );
      initialScreen->loadRectangle( "outline3", promptCenter + 2, IPPromptOutline3, promptWidth - 4, 46 );
    } else {
      initialScreen->loadRectangle( "outline", promptCenter, portPromptOutline, promptWidth, 50 );
      initialScreen->loadRectangle( "outline2", promptCenter + 1, portPromptOutline2, promptWidth - 2, 48 );
      initialScreen->loadRectangle( "outline3", promptCenter + 2, portPromptOutline3, promptWidth - 4, 46 );
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
  string continueButtonText = "CONTINUAR";
  string backButtonText = "VOLVER";

  Screen* validationScreen = new Screen( this->sdlHandler );

  // Load logo and text
  validationScreen->loadTexture( "logo", "windowImages/1942logoPrincipal.bmp" );
  validationScreen->loadText( "connecting", connectingText, { 255, 255, 255, 255 } );
  validationScreen->loadText( "connected", connectedText, { 255, 255, 255, 255 } );
  validationScreen->loadText( "failure", failureText, { 255, 255, 255, 255 } );
  validationScreen->loadText( "continue", continueButtonText, {255,255,255,255} );
  validationScreen->loadText( "back", backButtonText, {255,255,255,255} );

  int buttonWidth = 250;
  int buttonCenter = validationScreen->getRectCenter( buttonWidth );
  validationScreen->loadRectangle( "button", buttonCenter, 475, buttonWidth, 50 );

  // Get center positions
  int logoCenter = validationScreen->getTextureCenter( "logo" ); 
  int connectingTextCenter = validationScreen->getTextCenter( connectingText );
  int connectedTextCenter = validationScreen->getTextCenter( connectedText );
  int failureTextCenter = validationScreen->getTextCenter( failureText );
  int continueTextCenter = validationScreen->getTextCenter( continueButtonText ); 
  int backTextCenter = validationScreen->getTextCenter( backButtonText ); 

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
	this->unCliente->disconnectFromServer();
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
      if ((mouseX > buttonCenter) && (mouseX < (buttonCenter + 250))
	  && (mouseY > 475) && (mouseY < (475 + 50))) {
	runningScreen = false;
	//Disable text input
	SDL_StopTextInput();
	delete validationScreen;
	if (connectionFailed){
	  this->loadConnectionScreen();
	} else {
		if (this->unCliente->getGameData()->cooperativeMode || this->unCliente->getGameData()->teamMode){
			this->checkGameMode();
		} else {
			this->loadModeGameScreen();
		}
	}
	break;
      }
    }

    // Render text textures
    if (!connected){
      validationScreen->renderTexture( "connecting", connectingTextCenter, 305 );
    } else {
      if (connectionFailed) {
	validationScreen->renderTexture( "failure", failureTextCenter, 305 );
	validationScreen->setRenderDrawColor( 234, 25 ,25 , 255 );
	validationScreen->renderRectangle( "button" );
	validationScreen->renderTexture( "back", backTextCenter, 480 );
      } else {
	validationScreen->renderTexture( "connected", connectedTextCenter, 305 );
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
  initialScreen->loadText("clientText", "Jugador:", { 0, 255, 100 });
  initialScreen->loadText("planeText", "Avion:", { 0, 255, 100 });
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
  int textoIzquierda = 220;
  int buttonTextCenter = buttonCenter	+ ((230 - initialScreen->getTextureWidth("accept")) / 2);
  int inputClientIdPosY = 250;
  int inputPlaneIdPosY = 350;
  int clientIdPromptOutline = inputClientIdPosY, clientIdPromptOutline2 =	inputClientIdPosY + 1, clientIdPromptOutline3 = inputClientIdPosY + 2;
  int bluePlanePromptOutline = inputPlaneIdPosY, bluePlanePromptOutline2 = inputPlaneIdPosY + 1, bluePlanePromptOutline3 = inputPlaneIdPosY + 2;
  int redPlanePromptOutline = inputPlaneIdPosY, redPlanePromptOutline2 = inputPlaneIdPosY + 1, redPlanePromptOutline3 = inputPlaneIdPosY + 2;
  int greenPlanePromptOutline = inputPlaneIdPosY, greenPlanePromptOutline2 = inputPlaneIdPosY + 1, greenPlanePromptOutline3 = inputPlaneIdPosY + 2;
  int yellowPlanePromptOutline = inputPlaneIdPosY, yellowPlanePromptOutline2 = inputPlaneIdPosY + 1, yellowPlanePromptOutline3 = inputPlaneIdPosY + 2;

  int mouseX, mouseY;

  int inputBluePlaneIdPosX = textoIzquierda + 300;
  int inputRedPlaneIdPosX = textoIzquierda + 380;
  int inputGreenPlaneIdPosX = textoIzquierda + 460;
  int inputYellowPlaneIdPosX = textoIzquierda + 540;
  int promptClientIdPosX = textoIzquierda + 310;

  bool runningScreen = true;
  bool firstPromptSelected = true;
  bool bluePromptSelected = false;
  bool redPromptSelected = false;
  bool greenPromptSelected = false;
  bool yellowPromptSelected = false;
  bool clicked = false;

  // Create prompts
  initialScreen->loadRectangle("promptClientId",promptClientIdPosX ,inputClientIdPosY, 260, 50);
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
	this->unCliente->disconnectFromServer();
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
    initialScreen->loadRectangle("outline", promptClientIdPosX,clientIdPromptOutline, 260, 50);
    initialScreen->loadRectangle("outline2", promptClientIdPosX + 1,clientIdPromptOutline2, 258, 48);
    initialScreen->loadRectangle("outline3", promptClientIdPosX + 2,clientIdPromptOutline3, 256, 46);

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
      initialScreen->renderTexture("clientId", promptClientIdPosX, inputClientIdPosY);
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
  jugador->team = this->team;
  this->unCliente->sendData(jugador);
}


void Game::loadWaitingGame() {
  SDL_Event e;
  string connectingText = "Esperando Jugadores...";
  Screen* waitingScreen = new Screen( this->sdlHandler );
  waitingScreen->loadTexture( "logo", "windowImages/1942logoPrincipal.bmp" );
  waitingScreen->loadText( "connecting", connectingText, { 255, 255, 255, 255 } );

  int logoCenter = ( this->windowWidth - waitingScreen->getTextureWidth( "logo" ) ) / 2;
  int promptCenter = logoCenter - 100;

  // Enable text input
  SDL_StartTextInput();
  // Set window background
  this->sdlHandler->setWindowBG(0, 0, 0);
  // Render logo
  waitingScreen->renderTexture( "logo", logoCenter, 90 );
  waitingScreen->renderTexture("connecting", promptCenter, 300 );
  //Update screen
  this->sdlHandler->updateWindow();
  while( !unCliente->allPlayersReady()){

    while (this->sdlHandler->nextEvent(&e)) {
      if (e.type == SDL_QUIT) {
	this->running = false;
	this->unCliente->disconnectFromServer();
	exit(1);
      } //Special key input
    }
  }

  //Disable text input
  SDL_StopTextInput();
  delete waitingScreen;
}

void Game::loadSinglePlayerScoreScreen( int stage ) {
  if( !( this->running ) ) return;
  bool runningScreen = true;
  SDL_Event e;
  Timer timer;
  string stageCompleteText = "Stage " + to_string( stage ) + " Complete !!";
  string scoreHeaderText = "Score Ranking";
  string nameText = "Name";
  string scoreText = "Score";

  Screen* scoreScreen= new Screen( this->sdlHandler );

  // TODO: REMOVE, only for test purpouses
  this->player = new Player( "sousuke", "rojo", 20, 200, 1);
  Player* player2 = new Player( "kaname", "azul", 20, 200, 1);
  Player* player3 = new Player( "tessa", "amarillo", 20, 200, 2);
  Player* player4 = new Player( "melissa", "verde", 20, 200, 2);

  this->player->addScore( 17816229 );
  player3->addScore( 5287 );
  player4->addScore( 98723 );

  // Load text
  scoreScreen->loadText( "stageComplete", stageCompleteText, { 53, 167, 84, 255 } );
  scoreScreen->loadText( "scoreText", scoreHeaderText, { 255, 0, 0, 255 } );
  scoreScreen->loadText( "nameHeader", nameText, { 191, 189, 37, 255 } );
  scoreScreen->loadText( "scoreHeader", scoreText, { 191, 189, 37, 255 } );
  scoreScreen->loadText( player->getName(), player->getName(), { 255, 255, 255, 255 } );
  scoreScreen->loadText( player->getName() + "score", to_string( player->getScore() ), { 255, 255, 255, 255 } );
  scoreScreen->loadText( player2->getName(), player2->getName(), { 255, 255, 255, 255 } );
  scoreScreen->loadText( player2->getName() + "score", to_string( player2->getScore() ), { 255, 255, 255, 255 } );
  scoreScreen->loadText( player3->getName(), player3->getName(), { 255, 255, 255, 255 } );
  scoreScreen->loadText( player3->getName() + "score", to_string( player3->getScore() ), { 255, 255, 255, 255 } );
  scoreScreen->loadText( player4->getName(), player4->getName(), { 255, 255, 255, 255 } );
  scoreScreen->loadText( player4->getName() + "score", to_string( player4->getScore() ), { 255, 255, 255, 255 } );
  scoreScreen->loadText( "continueText", "Continue", { 0, 0, 0, 255 } );

  int buttonWidth = 250;
  int buttonCenter = scoreScreen->getRectCenter( buttonWidth ); 
  int continueTextCenter = scoreScreen->getTextCenter( "continue" );

  // Load prompts
  scoreScreen->loadRectangle( "continue", buttonCenter, 600, buttonWidth, 50 );

  // Load planes
  scoreScreen->loadTexture( player->getColor(), "score/avion_" + player->getColor() + ".bmp" );
  scoreScreen->loadTexture( player2->getColor(), "score/avion_" + player2->getColor() + ".bmp" );
  scoreScreen->loadTexture( player3->getColor(), "score/avion_" + player3->getColor() + ".bmp" );
  scoreScreen->loadTexture( player4->getColor(), "score/avion_" + player4->getColor() + ".bmp" );

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

  HealthView* remainingHealth = new HealthView( scoreScreen, this->player->getHealth() );

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

    remainingHealth->render();
    // Render text textures
    scoreScreen->renderTexture( "stageComplete", stageCompleteTextCenter, topPadding );
    scoreScreen->renderTexture( "scoreText", scoreTextCenter, topPadding + gap * 2 );
    scoreScreen->renderTexture( "nameHeader", stageCompleteTextCenter, topPadding + gap * 4 );
    scoreScreen->renderTexture( "scoreHeader", scoreHeaderCenter, topPadding + gap * 4 );
    scoreScreen->renderTexture( player->getName(), stageCompleteTextCenter, topPadding + gap * 5.5 );
    scoreScreen->renderTexture( player->getName() + "score", 
				scoreRightLimit - scoreScreen->getTextWidth( to_string( player->getScore() ) ), topPadding + gap * 5.5 );
    scoreScreen->renderTexture( player->getColor(), imageCenter, topPadding + gap * 5.5 );
    scoreScreen->renderTexture( player2->getName(), stageCompleteTextCenter, topPadding + gap * 7 );
    scoreScreen->renderTexture( player2->getName() + "score", 
				scoreRightLimit - scoreScreen->getTextWidth( to_string(
				player2->getScore() ) ), topPadding + gap * 7 );
    scoreScreen->renderTexture( player2->getColor(), imageCenter, topPadding + gap * 7 );
    scoreScreen->renderTexture( player3->getName(), stageCompleteTextCenter, topPadding + gap * 8.5 );
    scoreScreen->renderTexture( player3->getName() + "score", 
				scoreRightLimit - scoreScreen->getTextWidth( to_string( player3->getScore() ) ), topPadding + gap * 8.5 );
    scoreScreen->renderTexture( player3->getColor(), imageCenter, topPadding + gap * 8.5 );
    scoreScreen->renderTexture( player4->getName(), stageCompleteTextCenter, topPadding + gap * 10 );
    scoreScreen->renderTexture( player4->getName() + "score", 
				scoreRightLimit - scoreScreen->getTextWidth( to_string(
				player4->getScore() ) ), topPadding + gap * 10 );
    scoreScreen->renderTexture( player4->getColor(), imageCenter, topPadding + gap * 10 );
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
    this->sdlHandler->updateWindow();

    if( timer.tiempoActual() < 1000 / this->fps ){
      SDL_Delay( ( 1000 / this->fps ) - timer.tiempoActual() );
    }
  }

  //Disable text input
  SDL_StopTextInput();
  delete scoreScreen;
}

void Game::loadModeGameScreen(){
  SDL_Event e;
  Timer timer;

  Screen* initialScreen = new Screen( this->sdlHandler );
  initialScreen->loadTexture( "logo", "windowImages/1942logoPrincipal.bmp" );
  initialScreen->loadText( "gameModeText", "Modo de Juego:", { 0, 200, 100 } );
  initialScreen->loadText( "practiceText", "Modo Practica:", { 0, 200, 100 } );
  initialScreen->loadText( "accept", "ACEPTAR");
  initialScreen->loadText( "practice", "PRACTICA");
  initialScreen->loadText( "cooperative", "COOPERATIVO");
  initialScreen->loadText( "team", "EQUIPOS");

  int logoCenter = ( this->windowWidth - initialScreen->getTextureWidth( "logo" ) ) / 2;
  int promptCenter = logoCenter - 20;
  int buttonCenter = promptCenter + 15;
  int textCenter = promptCenter + 20;
  int buttonAcceptTextCenter = buttonCenter + ( ( 230 - initialScreen->getTextureWidth( "accept" ) ) / 2 );
  int buttonTeamTextCenter = buttonCenter+150 + ( ( 230 - initialScreen->getTextureWidth( "team" ) ) / 2 );
  int buttonCooperativeTextCenter = buttonCenter+150 + ( ( 230 - initialScreen->getTextureWidth( "cooperative" ) ) / 2 );
  int buttonPracticeTextCenter = buttonCenter+150 + ( ( 230 - initialScreen->getTextureWidth( "practice" ) ) / 2 );
  int teamPromptOutline = 300, teamPromptOutline2 = 301, teamPromptOutline3 = 302;
  int cooperativePromptOutline = 375, cooperativePromptOutline2 = 376, cooperativePromptOutline3 = 377;
  int practicePromptOutline = 450, practicePromptOutline2 = 451, practicePromptOutline3 = 452;
  int mouseX, mouseY;

  bool runningScreen = true;
  bool teamPromptSelected = true;
  bool cooperativePromptSelected = false;
  bool practicePromptSelected = false;
  bool clicked = false;

  // Create prompts
  initialScreen->loadRectangle( "promptTeam", promptCenter+150, 300, 260, 50 );
  initialScreen->loadRectangle( "promptCooperative", promptCenter+150, 375, 260, 50 );
  initialScreen->loadRectangle( "promptPractice", promptCenter+150, 450, 260, 50 );
  initialScreen->loadRectangle( "button", promptCenter + 15, 525, 230, 50 );

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
		  this->unCliente->disconnectFromServer();
	  } else if( e.button.type == SDL_MOUSEBUTTONDOWN ) {
		  if( e.button.button == SDL_BUTTON_LEFT ) {
			  clicked = true;
			  // Get the mouse offsets
			  mouseX = e.button.x;
			  mouseY = e.button.y;
			}
	  }
	}

	// Set window background
	sdlHandler->setWindowBG(0, 0, 0);
	// Render logo
	initialScreen->renderTexture( "logo", logoCenter, 90 );
	// Set prompt color
	initialScreen->setRenderDrawColor( 160, 160, 160, 255 );
	// Render prompts
	initialScreen->renderRectangle( "promptTeam" );
	initialScreen->renderRectangle( "promptCooperative" );
	initialScreen->renderRectangle( "promptPractice" );

	if( clicked ) {
	  clicked = false;
	  if( ( mouseX > buttonCenter ) && ( mouseX < ( buttonCenter + 230 ) ) && ( mouseY > 525 ) && ( mouseY < ( 525 + 50 ) ) ) {
			runningScreen = false;
			//Disable text input
			SDL_StopTextInput();
			delete initialScreen;
			this->unCliente->getGameData()->teamMode = teamPromptSelected;
			this->unCliente->getGameData()->cooperativeMode = cooperativePromptSelected;
			this->unCliente->getGameData()->practiceMode = practicePromptSelected;
			this->checkGameMode();

	  } else if( ( mouseX > promptCenter+150 ) && ( mouseX < ( promptCenter+150 + 230 ) ) && ( mouseY > 300 ) && ( mouseY < ( 300 + 50 ) ) ) {
			  teamPromptSelected = true;
			  cooperativePromptSelected = false;
	  } else if( ( mouseX > promptCenter+150 ) && ( mouseX < ( promptCenter+150 + 230 ) ) && ( mouseY > 375 ) && ( mouseY < ( 375 + 50 ) ) ) {
			  cooperativePromptSelected = true;
			  teamPromptSelected = false;
	  } else if( ( mouseX > promptCenter+150 ) && ( mouseX < ( promptCenter+150 + 230 ) ) && ( mouseY > 450 ) && ( mouseY < ( 450 + 50 ) ) ) {
		  	  practicePromptSelected = !practicePromptSelected;
	  }
	}

	initialScreen->renderRectangle( "button" );

	// Set outline color
	initialScreen->setRenderDrawColor( 19, 144, 27, 255 );

	if( teamPromptSelected ) {
	  initialScreen->loadRectangle( "outline", promptCenter+150, teamPromptOutline, 260, 50 );
	  initialScreen->loadRectangle( "outline2", promptCenter+150 + 1, teamPromptOutline2, 258, 48 );
	  initialScreen->loadRectangle( "outline3", promptCenter+150 + 2, teamPromptOutline3, 256, 46 );
	} else if( cooperativePromptSelected ) {
	  initialScreen->loadRectangle( "outline", promptCenter+150, cooperativePromptOutline, 260, 50 );
	  initialScreen->loadRectangle( "outline2", promptCenter+150 + 1, cooperativePromptOutline2, 258, 48 );
	  initialScreen->loadRectangle( "outline3", promptCenter+150 + 2, cooperativePromptOutline3, 256, 46 );
	}

	if( practicePromptSelected ) {
		initialScreen->loadRectangle( "outlinePractice", promptCenter+150, practicePromptOutline, 260, 50 );
		initialScreen->loadRectangle( "outlinePractice2", promptCenter+150 + 1, practicePromptOutline2, 258, 48 );
		initialScreen->loadRectangle( "outlinePractice3", promptCenter+150 + 2, practicePromptOutline3, 256, 46 );
		// Render outlines
		initialScreen->renderRectangle( "outlinePractice", true );
		initialScreen->renderRectangle( "outlinePractice2", true );
		initialScreen->renderRectangle( "outlinePractice3", true );
	}

	// Render outlines
	initialScreen->renderRectangle( "outline", true );
	initialScreen->renderRectangle( "outline2", true );
	initialScreen->renderRectangle( "outline3", true );

	// Render text textures
	initialScreen->renderTexture( "gameModeText", 120, teamPromptOutline );
	initialScreen->renderTexture( "practiceText", 120, practicePromptOutline );
	initialScreen->renderTexture( "cooperative", buttonCooperativeTextCenter, cooperativePromptOutline );
	initialScreen->renderTexture( "team", buttonTeamTextCenter, teamPromptOutline );
	initialScreen->renderTexture( "practice", buttonPracticeTextCenter, practicePromptOutline );
	initialScreen->renderTexture( "accept", buttonAcceptTextCenter, 525 );

	//Update screen
	sdlHandler->updateWindow();

	if( timer.tiempoActual() < 1000 / this->fps ){
	  SDL_Delay( ( 1000 / this->fps ) - timer.tiempoActual() );
	}
  }
}


void Game::loadTeamSelectedScreen(){
  SDL_Event e;
  Timer timer;
  GameData* gameData = this->unCliente->getGameData();

  bool team1Complete = (gameData->maxPlayersTeams == gameData->countPlayersTeam1);
  bool team2Complete = (gameData->maxPlayersTeams == gameData->countPlayersTeam2);

  Screen* initialScreen = new Screen( this->sdlHandler );
  initialScreen->loadTexture( "logo", "windowImages/1942logoPrincipal.bmp" );
  initialScreen->loadText( "teamText", "Elija equipo:", { 0, 200, 100 } );
  initialScreen->loadText( "accept", "ACEPTAR");
  initialScreen->loadText( "alphaTeam", "EQUIPO ALFA",{255,50,0});
  initialScreen->loadText( "betaTeam", "EQUIPO BETA",{0,50,255});

  int logoCenter = ( this->windowWidth - initialScreen->getTextureWidth( "logo" ) ) / 2;
  int promptCenter = logoCenter - 20;
  int buttonCenter = promptCenter + 15;
  int textCenter = promptCenter + 20;
  int buttonAcceptTextCenter = buttonCenter + ( ( 230 - initialScreen->getTextureWidth( "accept" ) ) / 2 );
  int buttonAlphaTeamTextCenter = buttonCenter+150 + ( ( 230 - initialScreen->getTextureWidth( "alphaTeam" ) ) / 2 );
  int buttonBetaTeamTextCenter = buttonCenter+150 + ( ( 230 - initialScreen->getTextureWidth( "betaTeam" ) ) / 2 );
  int alphaTeamPromptOutline = 300, alphaTeamPromptOutline2 = 301, alphaTeamPromptOutline3 = 302;
  int betaTeamPromptOutline = 375, betaTeamPromptOutline2 = 376, betaTeamPromptOutline3 = 377;
  int mouseX, mouseY;

  bool runningScreen = true;
  bool alphaTeamSelected = true;
  bool betaTeamSelected = false;
  bool clicked = false;

  // Create prompts
  initialScreen->loadRectangle( "promptAlphaTeam", promptCenter+150, 300, 260, 50 );
  initialScreen->loadRectangle( "promptBetaTeam", promptCenter+150, 375, 260, 50 );
  initialScreen->loadRectangle( "button", promptCenter + 15, 525, 230, 50 );

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
		  this->unCliente->disconnectFromServer();
		  exit(1);
	  } else if( e.button.type == SDL_MOUSEBUTTONDOWN ) {
		  if( e.button.button == SDL_BUTTON_LEFT ) {
			  clicked = true;
			  // Get the mouse offsets
			  mouseX = e.button.x;
			  mouseY = e.button.y;
			}
	  }
	}

	// Set window background
	sdlHandler->setWindowBG(0, 0, 0);
	// Render logo
	initialScreen->renderTexture( "logo", logoCenter, 90 );
	// Set prompt color
	initialScreen->setRenderDrawColor( 160, 160, 160, 255 );
	// Render prompts
	if( !team1Complete ){
		initialScreen->renderRectangle( "promptAlphaTeam" );
	}
	if( !team2Complete ){
		initialScreen->renderRectangle( "promptBetaTeam" );
	}

	if( clicked ) {
	  clicked = false;
	  if( ( mouseX > buttonCenter ) && ( mouseX < ( buttonCenter + 230 ) ) && ( mouseY > 525 ) && ( mouseY < ( 525 + 50 ) ) ) {
		runningScreen = false;
		if ( alphaTeamSelected ){
			this->team = 1;
			this->unCliente->getGameData()->countPlayersTeam1++;
		} else if (betaTeamSelected ){
			this->team = 2;
			this->unCliente->getGameData()->countPlayersTeam2++;
		}
		//Disable text input
		SDL_StopTextInput();
		delete initialScreen;
		break;
	  } else if( ( mouseX > promptCenter+150 ) && ( mouseX < ( promptCenter+150 + 230 ) ) && ( mouseY > 300 ) && ( mouseY < ( 300 + 50 ) ) ) {
		  alphaTeamSelected = true;
		  betaTeamSelected = false;
	  } else if( ( mouseX > promptCenter+150 ) && ( mouseX < ( promptCenter+150 + 230 ) ) && ( mouseY > 375 ) && ( mouseY < ( 375 + 50 ) ) ) {
		  alphaTeamSelected = false;
  		  betaTeamSelected = true;
	  }
	}

	initialScreen->renderRectangle( "button" );

	// Set outline color
	initialScreen->setRenderDrawColor( 19, 144, 27, 255 );

	if( alphaTeamSelected && !team1Complete ) {
	  initialScreen->loadRectangle( "outline", promptCenter+150, alphaTeamPromptOutline, 260, 50 );
	  initialScreen->loadRectangle( "outline2", promptCenter+150 + 1, alphaTeamPromptOutline2, 258, 48 );
	  initialScreen->loadRectangle( "outline3", promptCenter+150 + 2, alphaTeamPromptOutline3, 256, 46 );
	} else if( betaTeamSelected && !team2Complete ) {
	  initialScreen->loadRectangle( "outline", promptCenter+150, betaTeamPromptOutline, 260, 50 );
	  initialScreen->loadRectangle( "outline2", promptCenter+150 + 1, betaTeamPromptOutline2, 258, 48 );
	  initialScreen->loadRectangle( "outline3", promptCenter+150 + 2, betaTeamPromptOutline3, 256, 46 );
	}

	// Render outlines
	initialScreen->renderRectangle( "outline", true );
	initialScreen->renderRectangle( "outline2", true );
	initialScreen->renderRectangle( "outline3", true );

	// Render text textures
	initialScreen->renderTexture( "teamText", 120, alphaTeamPromptOutline );
	if( !team1Complete ){
		initialScreen->renderTexture( "alphaTeam", buttonAlphaTeamTextCenter, alphaTeamPromptOutline );
	}
	if( !team2Complete ){
		initialScreen->renderTexture( "betaTeam", buttonBetaTeamTextCenter, betaTeamPromptOutline );
	}
	initialScreen->renderTexture( "accept", buttonAcceptTextCenter, 525 );

	//Update screen
	sdlHandler->updateWindow();

	if( timer.tiempoActual() < 1000 / this->fps ){
	  SDL_Delay( ( 1000 / this->fps ) - timer.tiempoActual() );
	}
  }
}

void Game::checkGameMode(){
	if ( this->unCliente->getGameData()->teamMode ){
		cout<<"Modo Team desde otro"<<endl;
		this->loadTeamSelectedScreen();
	}
	cout<<"Envio el gameData al server"<<endl;
	this->unCliente->sendGameData();

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

