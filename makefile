####################   MODO  DE  USO   ####################

#modo: client o server

#Compile  : make

#Run      : make run p=modo

#Valgrind : make valgrind p=modo

############################################################

CC = g++
GAME_DIR = ./src/game
MODEL_DIR = $(GAME_DIR)/model
VIEW_DIR = $(GAME_DIR)/view
CONTROLLER_DIR = $(GAME_DIR)/controller

WINDOWINITIAL = $(GAME_DIR)/Game.cpp

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
COMPILER = -std=c++11

# libraries to link

LINKER = -lSDL2 -lX11 -lSDL2_ttf -pthread

# event
EVENT = $(GAME_DIR)/events/Events.cpp $(GAME_DIR)/events/CompanionEvent.cpp

# client
CLIENT = $(GAME_DIR)/client/GameClient.cpp ./src/libs/socket/Client.cpp

# server
SERVER = ./src/libs/socket/Server.cpp


# game
GAME = $(GAME_DIR)/1942MultiPlayer.cpp

# model
RESOLUCION = $(MODEL_DIR)/Resolucion.cpp
POSICION = $(MODEL_DIR)/Posicion.cpp
VIVIBLE = $(MODEL_DIR)/Vivible.h
COMPOSITE = $(MODEL_DIR)/CompositeVivibles.cpp
AVION = $(MODEL_DIR)/Avion.cpp
MISIL = $(MODEL_DIR)/Misil.cpp
PLAYER = $(MODEL_DIR)/Player.cpp

# view
AVION_VIEW = $(VIEW_DIR)/AvionView.cpp
TEXTURE = $(VIEW_DIR)/Texture.cpp
BACKGROUND = $(VIEW_DIR)/Escenario.cpp
ISLA = $(VIEW_DIR)/Isla.cpp
MISIL_VIEW = $(VIEW_DIR)/MisilView.cpp
SCREEN = $(VIEW_DIR)/Screen.cpp


# controller
CONTROLLER_CONTROLLER = $(CONTROLLER_DIR)/Controller.cpp
PLAYERS_CONTROLLERS = $(CONTROLLER_DIR)/PlayersController.cpp 
MISILES_CONTROLLERS = $(CONTROLLER_DIR)/ControllerMissiles.cpp
TIMER_CONTROLLER = $(CONTROLLER_DIR)/Timer.cpp 
HANDLER_CONTROLLER = $(CONTROLLER_DIR)/HandlerPlayersControllers.cpp

# M V C
MODEL = $(RESOLUCION) $(POSICION) $(VIVIBLE) $(COMPOSITE) $(AVION) $(MISIL) $(PLAYER)
VIEW = $(AVION_VIEW) $(TEXTURE) $(BACKGROUND) $(ISLA) $(MISIL_VIEW) $(SCREEN)
CONTROLLER = $(CONTROLLER_CONTROLLER) $(PLAYERS_CONTROLLERS) $(MISILES_CONTROLLERS) $(TIMER_CONTROLLER) $(HANDLER_CONTROLLER)

# executable name
EXE = 1942MultiPlayer.exe

OBJS = $(MODEL) $(VIEW) $(CONTROLLER) $(WINDOWINITIAL) $(XM_SDL) $(CONF) $(XML) $(LOGGER) $(PALETTE) $(TMT) $(CLIENT) $(SERVER) $(EVENT) $(GAME)

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER) $(LINKER) -o $(EXE)
	
run:
	./$(EXE) $(p)
	
valgrind:
	valgrind --leak-check=full ./$(EXE) $(p)
