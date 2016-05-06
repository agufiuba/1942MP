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

  string serverIP = "127.0.0.1";
  string serverPort = "8080";
  string acceptButtonText = "ACEPTAR";

  Screen* initialScreen = new Screen( sdlHandler );
  initialScreen->loadTexture( "logo", "windowImages/1942logoPrincipal.bmp" );
  initialScreen->loadText( "serverIP", serverIP );
  initialScreen->loadText( "serverPort", serverPort );
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
  bool accept = false;

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
	  if( e.key.keysym.sym == SDLK_BACKSPACE && serverIP.length() > 0 ) {
	    //lop off character
	    serverIP.pop_back();
	    renderText = true;
	  }
	} else if( e.key.keysym.sym == SDLK_BACKSPACE && serverPort.length() > 0 ) {
	  //lop off character
	  serverPort.pop_back();
	  renderText = true;
	}
	//Handle copy
	else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
	{
	  SDL_SetClipboardText( serverIP.c_str() );
	}
	//Handle paste
	else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
	{
	  serverIP = SDL_GetClipboardText();
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
	    if( serverIP.length() < 15 )
	      serverIP += e.text.text;
	  } else {
	    // Character limit for port 
	    if( serverPort.length() < 5 )
	      serverPort += e.text.text;
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
    if( renderText  && !accept )
    {
      if( firstPromptSelected ) {
	//Text is not empty
	if( serverIP != "" )
	{
	  //Render new text
	  initialScreen->loadText( "serverIP", serverIP );
	}
	//Text is empty
	else
	{
	  //Render space texture
	  initialScreen->loadText( "serverIP", " " );
	}
      } else {

	//Text is not empty
	if( serverPort != "" )
	{
	  //Render new text
	  initialScreen->loadText( "serverPort", serverPort );
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

    if( !accept ) {
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
	  initialScreen->setRenderDrawColor( 86, 86, 86, 255 );
	  accept = true;
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
    }
    //Update screen
    sdlHandler->updateWindow();

    // Accept button clicked
    if( accept ) {
    }

    if( timer.tiempoActual() < 1000 / this->fps ){
      SDL_Delay( ( 1000 / this->fps ) - timer.tiempoActual() );
    }
  }
  //Disable text input
  SDL_StopTextInput();
}
