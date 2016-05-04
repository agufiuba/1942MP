#include "../libs/xm_sdl/XM_SDL.h"
#include "../game/view/Texture.h"
#include "../game/controller/Timer.h"
#include <string>
#include <iostream>
using namespace std;

int main(int argc, char **argv) {

  const int WINDOW_WIDTH = 600; //TODO: aca hay que cambiar a lo del escenario
  const int WINDOW_HEIGHT = 600;
  const char* WINDOW_TITLE = "1942MP Arcade";
  const char* FONT_PATH = "fonts/open_sans/OpenSans-Regular.ttf";

  bool quit = false;

  XM_SDL* sdlHandler = new XM_SDL( SDL_INIT_EVERYTHING);

  // Create window
  if (sdlHandler->createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT)) {
    SDL_Event e;
    SDL_Renderer* renderer = sdlHandler->getRenderer();

    Texture * logoPrincipal = new Texture();
    SDL_Rect* exactImage = new SDL_Rect();
    exactImage->x = 0;
    exactImage->y = 0;
    exactImage->w = 250;
    exactImage->h = 100;
    
    // Create text inputs
    string serverIP = "127.0.0.1";
    string serverPort = "8080";
    TTF_Font* fontFamily = sdlHandler->loadFont( FONT_PATH );
    // White text color
    SDL_Color textColor = { 255, 255, 255, 255 };
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

    // Set window background
    sdlHandler->setWindowBG(0, 0, 0);

    logoPrincipal->render(175,75,renderer,exactImage);

    sdlHandler->updateWindow();

    while (!quit) {
      bool renderText = false;

      // Get events
      while (sdlHandler->nextEvent(&e)) {
	if (e.type == SDL_QUIT) {
	  quit = true;
	} //Special key input
	 else if( e.type == SDL_KEYDOWN ) {
	    //Handle backspace
	    if( e.key.keysym.sym == SDLK_BACKSPACE && serverIP.length() > 0 )
	    {
	      //lop off character
	      serverIP.pop_back();
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
	    if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
	    {
	      //Append character
	      serverIP += e.text.text;
	      renderText = true;
	    }
	  }
      }

	//Rerender text if needed
	if( renderText )
	{
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
	}

	//Render text textures
	serverIPInput->render( 175, 300, renderer );
	
	//Update screen
	sdlHandler->updateWindow();
    }
      	//Disable text input
      	SDL_StopTextInput();
  }

  delete sdlHandler;

  return 0;
}


