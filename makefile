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

# compiler
COMPILER = -std=c++11

# libraries to link
LINKER = -lSDL2 -lX11

# game
GAME = $(GAME_DIR)/1942MultiPlayer.cpp

# model
RESOLUCION = $(MODEL_DIR)/Resolucion.cpp
POSICION = $(MODEL_DIR)/Posicion.cpp
VIVIBLE = $(MODEL_DIR)/Vivible.h
COMPOSITE = $(MODEL_DIR)/CompositeVivibles.cpp
AVION = $(MODEL_DIR)/Avion.cpp
MISIL = $(MODEL_DIR)/Misil.cpp

# view
AVION_VIEW = $(VIEW_DIR)/AvionView.cpp
TEXTURE = $(VIEW_DIR)/Texture.cpp
BACKGROUND = $(VIEW_DIR)/Escenario.cpp
ISLA = $(VIEW_DIR)/Isla.cpp
MISIL_VIEW = $(VIEW_DIR)/MisilView.cpp

# controller
CONTROLLER_CONTROLLER = $(CONTROLLER_DIR)/Controller.cpp
PLAYERS_CONTROLLERS = $(CONTROLLER_DIR)/PlayersController.cpp 
MISILES_CONTROLLERS = $(CONTROLLER_DIR)/ControllerMissiles.cpp
TIMER_CONTROLLER = $(CONTROLLER_DIR)/Timer.cpp 

# M V C
MODEL = $(RESOLUCION) $(POSICION) $(VIVIBLE) $(COMPOSITE) $(AVION) $(MISIL)
VIEW = $(AVION_VIEW) $(TEXTURE) $(BACKGROUND) $(ISLA) $(MISIL_VIEW)
CONTROLLER = $(CONTROLLER_CONTROLLER) $(PLAYERS_CONTROLLERS) $(MISILES_CONTROLLERS) $(TIMER_CONTROLLER)

# executable name
EXE = 1942MultiPlayer.exe

OBJS = $(MODEL) $(VIEW) $(CONTROLLER) ./$(GAME)

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER) $(LINKER) -o $(EXE)
	
run:
	./$(EXE) $(p)
	
valgrind:
	valgrind --leak-check=full ./$(EXE) $(p)
