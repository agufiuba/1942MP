/*
 * Escenario.h
 *
 */

#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "Texture.h"
#include "Isla.h"
#include "../../libs/data/player_data.h"
#include "../model/Resolucion.h"
#include "../model/Posicion.h"
#include "../model/Avion.h"
#include "../model/Enemy.h"
#include "../model/Vivible.h"
#include "../controller/IController.h"
#include "../controller/Controller.h"
#include "../controller/PlayersController.h"
#include "../controller/Timer.h"
#include "../controller/HandlerPlayersControllers.h"
#include "../controller/ControllerMissiles.h"
#include "../controller/HandlerEnemigos.h"
#include "../../xml/conf/GameConf.h"
#include "../../xml/parser/GameParser.h"
#include "../../xml/conf/EscenarioConf.h"
#include "../../libs/socket/Client.h"
#include "../../libs/xm_sdl/XM_SDL.h"
#include "Screen.h"
#include "HealthView.h"
#include "ScoreView.h"
#include "../model/Player.h"
#include "../model/PowerUp.h"
#include "../model/Music.h"
#include "../controller/HandlerPowerUp.h"

class Escenario {

private:
	XM_SDL* sdl;
	Screen* escenarioScreen;
	HealthView* healthView;
	ScoreView* scoreView;
	ScoreView* teamScoreView;
	ScoreView* teamAlphaScoreView;
	ScoreView* teamBetaScoreView;
	Player* player;
	SDL_Renderer* gRenderer = NULL;
	SDL_Event evento;
	Resolucion* resolucion;

	bool isFullScreen = false;

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FRAMES_PER_SECOND = 30;
	int VELOCIDAD_SCREEN = -3;
	int LONGITUD_NIVEL;
	int CANTIDAD_NIVELES;
	int LIMITE_IMAGEN = 4000;
	int pixelesRecorridos = 0;
	int desfasajeConexion = 70; //Mas desfasaje, mas abajo se ponen las islas

	string rojo = "rojo";
	string azul = "azul";
	string verde = "verde";
	string amarillo = "amarillo";

	string DIR_FONDO_PANTALLA;

	Controller* myControl;
	HandlerPlayersControllers* controllers;
	HandlerPowerUp* hPowerUp;
	HandlerEnemigos* hEnemigos;

	vector<Isla*> fondosVivibles;

	GameConf* gc;
	Client* unCliente;
	Music* musica;
	Sound* youWin;
	Sound* gameOver;

	void actualizarEscenario(Posicion* posicion);
	void actualizarPosicionEscenario(Posicion* posicion);
	void aplicarFPS(Uint32 start);
	void setFondosVivibles(int x, int y);
	void configurarFondosVivibles();
	void configurarPowerUps();
	void limpiarFondosVivibles();
	bool isFinNivel(int numeroNivel);
	void loadScoreScreen( int stage = 0 );
	void loadCoopModeScoreScreen( int stage );
	void loadTeamModeScoreScreen( int stage );
	void loadWaitForPlayersScreen();
	void verificarEstacionamiento(int numeroNivel);
	void getPowerUp();
	void loadScoreData();
	void hitEnemy(vector<Vivible*>* disparos);
	bool escenarioCreado;
	void planesColision();
	void enemyOtherPlayerColision();
	void hitEnemyMissileByOtherPlayer(ControllerMissiles* controller);
	void enemyOtherPlayerMissileColision();
	void hitPlanes(vector<Vivible*>* disparos, Vivible* avion);
	void arrancarAviones();

public:
	Escenario(GameConf* configuracion, XM_SDL* sdl, Client* client);
	~Escenario();
	SDL_Event* run();
	HandlerPlayersControllers* getHandler();
	void setClient(Client* cliente);
	void setPlayer(Player* player);
	void configurarAvionAmigo(PlayerData* playerData);
	void configurarMiAvion(PlayerData* playerData);
    void crearFlota(int, int);
    int flota;
};

#endif /* ESCENARIO_H_ */
