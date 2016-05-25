#ifndef GAME_H
#define GAME_H

#include <string>
#include <thread>

#include "../libs/xm_sdl/XM_SDL.h"
#include "../libs/socket/Client.h"
#include "../libs/data/player_data.h"
#include "../xml/conf/GameConf.h"
#include "../game/model/Resolucion.h"
#include "view/Escenario.h"
#include "model/Player.h"
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
    Player* player;
  
    bool bluePlaneActive;
    bool redPlaneActive;
    bool greenPlaneActive;
    bool yellowPlaneActive;

    void loadConnectionScreen();
    void loadValidationScreen();
    void loadTimeoutScreen();
    void loadselectionPlane(bool selectedOk);
    void loadWaitingGame();
    void sendDataPlayer();
    void setPlanesActives(bool blue,bool red,bool green,bool yellow);

    void cargarEscenario();

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
