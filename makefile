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

#game
GAME = $(GAME_DIR)/1942Multiplayer.cpp
TEXTURE = $(GAME_DIR)/examples/libs/Texture.cpp
BACKGROUND = $(VIEW_DIR)/Escenario.cpp
RESOLUCION = $(MODEL_DIR)/Resolucion.cpp
POSICION = $(MODEL_DIR)/Posicion.cpp

# executable name
EXE = 1942Multiplayer.exe

OBJS = $(BACKGROUND) $(TEXTURE) $(RESOLUCION) $(POSICION) ./$(GAME)

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER) $(LINKER) -o $(EXE)
	
run:
	./$(EXE) $(p)
	
valgrind:
	valgrind --leak-check=full ./$(EXE) $(p)
