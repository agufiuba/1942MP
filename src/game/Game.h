#ifndef GAME_H
#define GAME_H
#include <string>
#include "../libs/xm_sdl/XM_SDL.h"
using namespace std;
class Game {
  private:
    XM_SDL* sdlHandler; 
    bool running;
    int fps;
    int windowWidth;
    int windowHeight;
    string windowTitle;
    void loadConnectionScreen();

  public:
    Game( uint32_t sdlFlags = SDL_INIT_EVERYTHING );
    ~Game();
    void start();
    void setWindowWidth( int width );
    void setWindowHeight( int height );
    void setWindowTitle( string title );
};
#endif
