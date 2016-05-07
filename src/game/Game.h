#ifndef GAME_H
#define GAME_H

#include <string>
#include <thread>

#include "../libs/xm_sdl/XM_SDL.h"
#include "../libs/socket/Client.h"

using namespace std;
class Game {
  private:
    XM_SDL* sdlHandler; 
    bool running;
    int fps;
    int windowWidth;
    int windowHeight;
    string windowTitle;
    string serverIP;
    string serverPort;
    Client* unCliente;
    
    string clientId;
    string planeId;
  
    void loadConnectionScreen();
    void loadValidationScreen();
    void loadselectionPlane();

  public:
    Game( uint32_t sdlFlags = SDL_INIT_EVERYTHING );
    ~Game();
    void start();
    bool connectingToServer();
    void setWindowWidth( int width );
    void setWindowHeight( int height );
    void setWindowTitle( string title );
};
#endif
