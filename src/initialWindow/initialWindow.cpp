#include "../libs/xm_sdl/XM_SDL.h"
#include "../game/view/Texture.h"
#include "../game/controller/Timer.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char **argv) {

  const int WINDOW_WIDTH = 800; //TODO: aca hay que cambiar a lo del escenario
  const int WINDOW_HEIGHT = 600;
  const char* WINDOW_TITLE = "1942MP Arcade";
  const int FRAMES_PER_SECOND = 10;
  const char* FONT_PATH = "fonts/open_sans/OpenSans-Regular.ttf";

  bool quit = false;

  XM_SDL* sdlHandler = new XM_SDL( SDL_INIT_EVERYTHING );

  // Create window
  if (sdlHandler->createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT)) {
    SDL_Event e;
    SDL_Renderer* renderer = sdlHandler->getRenderer();
    Timer fps;
    Texture* logoPrincipal = new Texture();

    // Create text inputs
    string serverIP = "127.0.0.1";
    string serverPort = "8080";
    TTF_Font* fontFamily = sdlHandler->loadFont( FONT_PATH );
    // White text color
    SDL_Color textColor = { 0, 0, 0, 255 };
    Texture* serverIPInput = new Texture();
    Texture* serverPortInput = new Texture();
    // Create textures from text 
    serverIPInput->loadFromRenderedText( serverIP.c_str(), fontFamily, textColor, renderer );
    serverPortInput->loadFromRenderedText( serverPort.c_str(), fontFamily, textColor, renderer );
    // Enable text input
    SDL_StartTextInput();


    if (!logoPrincipal->loadFromFile("windowImages/1942logoPrincipal.bmp", renderer)) {
      printf("Failed to load logoPrincipal texture image!\n");
      return false;
    }

    int logoCenter = ( WINDOW_WIDTH - logoPrincipal->getWidth() ) / 2;
    int promptCenter = logoCenter - 20;
    int textCenter = promptCenter + 20;
    bool firstPromptSelected = true;

    // Create prompt
    SDL_Rect prompt1 = { promptCenter, 300, 260, 50 };
    SDL_Rect prompt2 = { promptCenter, 375, 260, 50 };

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
	}

	//Rerender text if needed
	if( renderText )
	{
	  if( firstPromptSelected ) {
	    //Text is not empty
	    if( serverIP != "" )
	    {
	      //Render new text
	      serverIPInput->loadFromRenderedText( serverIP.c_str(), fontFamily, textColor, renderer );
	    }
	    //Text is empty
	    else
	    {
	      //Render space texture
	      serverIPInput->loadFromRenderedText( " ", fontFamily, textColor, renderer );
	    }
	  } else {

	    //Text is not empty
	    if( serverPort != "" )
	    {
	      //Render new text
	      serverPortInput->loadFromRenderedText( serverPort.c_str(), fontFamily, textColor, renderer );
	    }
	    //Text is empty
	    else
	    {
	      //Render space texture
	      serverPortInput->loadFromRenderedText( " ", fontFamily, textColor, renderer );
	    }
	  }
	}


	// Set window background
	sdlHandler->setWindowBG(0, 0, 0);

	logoPrincipal->render( logoCenter, 90, renderer, NULL );

	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderFillRect( renderer, &prompt1 );
	SDL_RenderFillRect( renderer, &prompt2 );
	//Render text textures
	serverIPInput->render( textCenter, 305, renderer );
	serverPortInput->render( textCenter, 380, renderer );

	//Update screen
	sdlHandler->updateWindow();

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


