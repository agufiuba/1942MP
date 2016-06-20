####################   MODO  DE  USO   ####################

#modo: client o server

#Compile  : make p=modo

#Run      : make run p=modo

#Valgrind : make valgrind p=modo

############################################################

CC = g++
GAME_DIR = ./src/game
MODEL_DIR = $(GAME_DIR)/model
VIEW_DIR = $(GAME_DIR)/view
CONTROLLER_DIR = $(GAME_DIR)/controller

WINDOWINITIAL = $(GAME_DIR)/Game.cpp

# colision
COLISION = ./src/colision/Colision.cpp

# xml
XML = ./src/libs/tinyxml2.cpp

XM_SDL = ./src/libs/xm_sdl/XM_SDL.cpp

# conf
CONF = ./src/xml/conf/* ./src/xml/parser/GameParser.cpp

# logger
LOGGER = ./src/libs/logger/Logger.cpp

# palette
PALETTE = ./src/libs/palette/palette.cpp

# transmitter
TMT = src/libs/transmitter/Transmitter.cpp

# compiler
COMPILER = -std=c++11 -Wno-write-strings

# libraries to link

LINKER = -lSDL2 -lX11 -lSDL2_ttf -lSDL2_mixer -pthread

# event
EVENT = $(GAME_DIR)/events/Events.cpp $(GAME_DIR)/events/CompanionEvent.cpp

# model
RESOLUCION = $(MODEL_DIR)/Resolucion.cpp
POSICION = $(MODEL_DIR)/Posicion.cpp
VIVIBLE = $(MODEL_DIR)/Vivible.h
COMPOSITE = $(MODEL_DIR)/CompositeVivibles.cpp
AVION = $(MODEL_DIR)/Avion.cpp
SERVERAVIONENEMIGO = $(MODEL_DIR)/ServerAvionEnemigo.cpp
SERVERAVIONENEMIGORANDOM = $(MODEL_DIR)/ServerAvionEnemigoRandom.cpp
SERVERAVIONENEMIGOMEDIO = $(MODEL_DIR)/ServerAvionEnemigoMedio.cpp
SERVERAVIONENEMIGOGRANDE = $(MODEL_DIR)/ServerAvionEnemyGrande.cpp
SERVERAVIONENEMIGOFLOTA = $(MODEL_DIR)/ServerAvionEnemigoFlota.cpp
MISIL = $(MODEL_DIR)/Misil.cpp
MISILDIRIGIDO = $(MODEL_DIR)/MisilDirigido.cpp
PLAYER = $(MODEL_DIR)/Player.cpp
POWERUP = $(MODEL_DIR)/PowerUp.cpp
SOUND = $(MODEL_DIR)/Sound.cpp
MUSIC = $(MODEL_DIR)/Music.cpp
ENEMY = $(MODEL_DIR)/Enemy.cpp
ENEMYMEDIANO = $(MODEL_DIR)/EnemyMediano.cpp
ENEMYGRANDE = $(MODEL_DIR)/EnemyGrande.cpp

# view
AVION_VIEW = $(VIEW_DIR)/AvionView.cpp
ENEMY_VIEW = $(VIEW_DIR)/EnemyView.cpp
TEXTURE = $(VIEW_DIR)/Texture.cpp
BACKGROUND = $(VIEW_DIR)/Escenario.cpp
ISLA = $(VIEW_DIR)/Isla.cpp
MISIL_VIEW = $(VIEW_DIR)/MisilView.cpp
SCREEN = $(VIEW_DIR)/Screen.cpp
POWERUP_VIEW = $(VIEW_DIR)/PowerUpView.cpp
HEALTH_VIEW = $(VIEW_DIR)/HealthView.cpp
SCORE_VIEW = $(VIEW_DIR)/ScoreView.cpp
EXPLOSION_VIEW = $(VIEW_DIR)/ExplosionView.cpp

# controller
CONTROLLER_CONTROLLER = $(CONTROLLER_DIR)/Controller.cpp
PLAYERS_CONTROLLERS = $(CONTROLLER_DIR)/PlayersController.cpp 
MISILES_CONTROLLERS = $(CONTROLLER_DIR)/ControllerMissiles.cpp
MISILES_CONTROLLERS_ENEMY = $(CONTROLLER_DIR)/ControllerMissilesEnemy.cpp
TIMER_CONTROLLER = $(CONTROLLER_DIR)/Timer.cpp 
HANDLER_CONTROLLER = $(CONTROLLER_DIR)/HandlerPlayersControllers.cpp
HANDLER_POWERUP = $(CONTROLLER_DIR)/HandlerPowerUp.cpp
HANDLER_ENEMIGO = $(CONTROLLER_DIR)/HandlerEnemigos.cpp

# M V C
MODEL = $(SERVERAVIONENEMIGOMEDIO) $(SERVERAVIONENEMIGO) $(SERVERAVIONENEMIGORANDOM) $(SERVERAVIONENEMIGOGRANDE) $(SERVERAVIONENEMIGOFLOTA) $(RESOLUCION) $(POSICION) $(VIVIBLE) $(COMPOSITE) $(AVION) $(MISIL) $(MISILDIRIGIDO) $(PLAYER) $(POWERUP) $(SOUND) $(MUSIC) $(ENEMY) $(ENEMYMEDIANO) $(ENEMYGRANDE)
VIEW = $(AVION_VIEW) $(ENEMY_VIEW) $(TEXTURE) $(BACKGROUND) $(ISLA) $(MISIL_VIEW) $(SCREEN) $(HEALTH_VIEW) $(SCORE_VIEW) $(POWERUP_VIEW) $(EXPLOSION_VIEW)
CONTROLLER = $(CONTROLLER_CONTROLLER) $(PLAYERS_CONTROLLERS) $(MISILES_CONTROLLERS) $(MISILES_CONTROLLERS_ENEMY) $(TIMER_CONTROLLER) $(HANDLER_CONTROLLER) $(HANDLER_POWERUP) $(HANDLER_ENEMIGO)
# RED
# client
CLIENT = $(GAME_DIR)/client/GameClient.cpp ./src/libs/socket/Client.cpp

# server
SERVER = ./src/libs/socket/Server.cpp

RED = $(CLIENT) $(SERVER)

# Otros
XML_PARSER = ./src/xml/parser/XMLParser.cpp
K = ./src/utils/K.cpp
DEFAULTS = ./src/utils/Defaults.cpp
MENU = ./src/libs/menu/Menu.cpp
FLOTAOBSERVER = $(MODEL_DIR)/FlotaObserver.cpp

UTILS = $(K) $(DEFAULTS) $(XML_PARSER) $(MENU) $(FLOTAOBSERVER)

# compile

COMPILE =  $(GAME_DIR)/1942MultiPlayer.cpp
ifeq ($(p), client)
	COMPILE = $(GAME_DIR)/1942MultiPlayer.cpp
endif
ifeq ($(p), server)
	COMPILE = ./src/libs/socket/examples/server/server.cpp
endif

# executable name
EXE = 
ifeq ($(p), client)
	EXE = 1942MultiPlayer.exe
endif
ifeq ($(p), server)
	EXE = ./src/libs/socket/examples/server/server.exe
endif


OBJS = $(COMPILE) $(MODEL) $(VIEW) $(CONTROLLER) $(WINDOWINITIAL) $(XM_SDL) $(CONF) $(XML) $(LOGGER) $(PALETTE) $(TMT) $(RED) $(EVENT) $(UTILS) $(COLISION)

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER) $(LINKER) -o $(EXE) 
#-g3 -DDEBUG_ALL -gdwarf-2 -g
	
run:
	./$(EXE) $(p)
	
valgrind:
	valgrind --leak-check=full ./$(EXE) $(p)
