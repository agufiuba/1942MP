#include "Game.h"
#include "controller/Timer.h"
#include "view/Screen.h"

Game::Game( uint32_t sdlFlags ) {
  this->sdlHandler = new XM_SDL( sdlFlags );
  this->running = false;
  this->windowWidth = 800;
  this->windowHeight = 600;
  this->windowTitle = "1942MP Arcade";
  this->fps = 10;
  this->serverIP = "127.0.0.1";
  this->serverPort = "8080";
}

Game::~Game() {
  delete sdlHandler;
}

void Game::start() {
  if( this->sdlHandler->createWindow( this->windowTitle.c_str(), this->windowWidth, this->windowHeight ) ) {
    this->running = true;
    this->loadConnectionScreen();
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
						//TODO: siguiente pantalla RAMON
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
    	connectionFailed = !this->unCliente->connectToServer(); //TODO: Revisar la devolucion en false, devuelve true
    	connected = true;
    }

/*
    if (connectionFailed) {
    	cout << "failed" << endl;
    } else {
    	cout << "connected" << endl;
    } */

  }
}
