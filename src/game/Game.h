#ifndef GAME_H
#define GAME_H

#include <string>
#include <thread>

#include "../libs/xm_sdl/XM_SDL.h"
#include "../libs/socket/Client.h"
#include "../libs/data/player_data.h"
#include "../xml/conf/GameConf.h"
#include "view/Escenario.h"
#include <SDL2/SDL.h>

using namespace std;
class Game {
  private:
    GameConf* gc;
    Escenario* escenario;
    XM_SDL* sdlHandler; 
    bool running;
    int fps;
    int windowWidth;
    int windowHeight;
    string windowTitle;
    string serverIP;
    string serverPort;
    Client* unCliente;
    PlayerData* jugador;
    string clientId;
    string planeId;
  
    bool bluePlaneActive;
    bool redPlaneActive;
    bool greenPlaneActive;
    bool yellowPlaneActive;

    void loadConnectionScreen();
    void loadValidationScreen();
    void loadselectionPlane(bool selectedOk);
    void loadWaitingGame();
    void sendDataPlayer();
    void setPlanesActives(bool blue,bool red,bool green,bool yellow);

    void cargarEscenario();
    void recorroConfig(GameConf*);
    void crearGameConfHardcodeada();

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
