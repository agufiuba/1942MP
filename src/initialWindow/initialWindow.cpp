#include "../libs/xm_sdl/XM_SDL.h"
#include "../game/view/Screen.h"
#include "../game/controller/Timer.h"
#include <string>
#include <iostream>
using namespace std;

int main( int argc, char* argv[] ) {
  const int WINDOW_WIDTH = 800; //TODO: aca hay que cambiar a lo del escenario
  const int WINDOW_HEIGHT = 600;
  const char* WINDOW_TITLE = "1942MP Arcade";
  const int FRAMES_PER_SECOND = 10;

  bool quit = false;

  XM_SDL* sdlHandler = new XM_SDL( SDL_INIT_EVERYTHING );

  // Create window
  if( sdlHandler->createWindow( WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT ) ) {
    SDL_Event e;
    SDL_Renderer* renderer = sdlHandler->getRenderer();
    Timer fps;
    
    string serverIP = "127.0.0.1";
    string serverPort = "8080";
    string acceptButtonText = "ACEPTAR";

    Screen* initialScreen = new Screen( sdlHandler );
    initialScreen->loadTexture( "logo", "windowImages/1942logoPrincipal.bmp" );
    initialScreen->loadText( "serverIP", serverIP );
    initialScreen->loadText( "serverPort", serverPort );
    initialScreen->loadText( "accept", "ACEPTAR" );

    int logoCenter = ( WINDOW_WIDTH - initialScreen->getTextureWidth( "logo" ) ) / 2;
    int promptCenter = logoCenter - 20;
    int buttonCenter = promptCenter + 15;
    int textCenter = promptCenter + 20;
    int buttonTextCenter = buttonCenter + ( ( 230 - initialScreen->getTextureWidth( "accept" ) ) / 2 );
    int IPPromptOutline = 300, IPPromptOutline2 = 301, IPPromptOutline3 = 302;
    int portPromptOutline = 375, portPromptOutline2 = 376, portPromptOutline3 = 377;
    int mouseX, mouseY;

    bool firstPromptSelected = true;
    bool clicked = false;
    bool accept = false;

    // Create prompts
    initialScreen->loadRectangle( "promptIP", promptCenter, 300, 260, 50 );
    initialScreen->loadRectangle( "promptPort", promptCenter, 375, 260, 50 );
    initialScreen->loadRectangle( "button", promptCenter + 15, 475, 230, 50 );

    // Enable text input
    SDL_StartTextInput();

    while (!quit) {
      fps.correr();
      bool renderText = false;

      // Get events
      while (sdlHandler->nextEvent(&e)) {
	if (e.type == SDL_QUIT) {
	  quit = true;
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
		serverIP += e.text.text;
	      } else {
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
	    cout << "Aceptar" << endl;
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

	if( fps.tiempoActual() < 1000 / FRAMES_PER_SECOND ){
	  SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.tiempoActual() );
	}
      }
      //Disable text input
      SDL_StopTextInput();
    }

    delete sdlHandler;

    return 0;
  }


