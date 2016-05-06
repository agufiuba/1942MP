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
  const char* FONT_PATH = "fonts/open_sans/OpenSans-Regular.ttf";

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

    // Enable text input
    SDL_StartTextInput();

    int logoCenter = ( WINDOW_WIDTH - initialScreen->getTextureWidth( "logo" ) ) / 2;
    int promptCenter = logoCenter - 20;
    int buttonCenter = promptCenter + 15;
    int textCenter = promptCenter + 20;
    int buttonTextCenter = buttonCenter + ( ( 230 - initialScreen->getTextureWidth( "accept" ) ) / 2 );
    bool firstPromptSelected = true;
    int mouseX, mouseY;
    bool clicked = false;
    bool accept = false;

    // Create prompt
    SDL_Rect prompt1 = { promptCenter, 300, 260, 50 };
    SDL_Rect prompt2 = { promptCenter, 375, 260, 50 };
    SDL_Rect button = { promptCenter + 15, 475, 230, 50 };
    SDL_Rect outline = { promptCenter, 0, 260, 50 };
    SDL_Rect outline2 = { promptCenter + 1, 0, 258, 48 };
    SDL_Rect outline3 = { promptCenter + 2, 0, 256, 46 };
    
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
	      //serverIPInput->loadFromRenderedText( serverIP.c_str(), fontFamily, textColor );
	    }
	    //Text is empty
	    else
	    {
	      //Render space texture
	      initialScreen->loadText( "serverIP", " " );
	      //serverIPInput->loadFromRenderedText( " ", fontFamily, textColor );
	    }
	  } else {

	    //Text is not empty
	    if( serverPort != "" )
	    {
	      //Render new text
	      initialScreen->loadText( "serverPort", serverPort );
	      //serverPortInput->loadFromRenderedText( serverPort.c_str(), fontFamily, textColor );
	    }
	    //Text is empty
	    else
	    {
	      //Render space texture
	      initialScreen->loadText( "serverPort", " " );
	      //serverPortInput->loadFromRenderedText( " ", fontFamily, textColor );
	    }
	  }
	}


	// Set window background
	sdlHandler->setWindowBG(0, 0, 0);

	initialScreen->renderTexture( "logo", logoCenter, 90 );

	if( !accept ) {
	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderFillRect( renderer, &prompt1 );
	SDL_RenderFillRect( renderer, &prompt2 );
	
	SDL_SetRenderDrawColor( renderer, 160, 160, 160, 255 );
	if( clicked ) {
	  clicked = false;
	  if( ( mouseX > buttonCenter ) && ( mouseX < ( buttonCenter + 230 ) ) 
	      && ( mouseY > 475 ) && ( mouseY < ( 475 + 50 ) ) ) {
	    SDL_SetRenderDrawColor( renderer, 86, 86, 86, 255 );
	    accept = true;
	    cout << "Aceptar" << endl;
	  }
	}
	SDL_RenderFillRect( renderer, &button );

	// Set outline color
	SDL_SetRenderDrawColor( renderer, 19, 144, 27, 255 );
	if( firstPromptSelected ) {
	  outline.y = 300;
	  outline2.y = 301;
	  outline3.y = 302;
	} else {
	  outline.y = 375;
	  outline2.y = 376;
	  outline3.y = 377;
	}
	SDL_RenderDrawRect( renderer, &outline );
	SDL_RenderDrawRect( renderer, &outline2 );
	SDL_RenderDrawRect( renderer, &outline3 );

	//Render text textures
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


