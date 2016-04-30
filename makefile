####################   MODO  DE  USO   ####################

#modo: client o server

#Compile  : make

#Run      : make run p=modo

#Valgrind : make valgrind p=modo

############################################################

CC = g++
GAME_DIR = ./src/game
MODEL_DIR = $(GAME_DIR)/model

# compiler
COMPILER = -std=c++11

# libraries to link
LINKER = -lSDL2 -lX11

#game
GAME = $(GAME_DIR)/1942Multiplayer.cpp
TEXTURE = $(GAME_DIR)/examples/libs/Texture.cpp
BACKGROUND = $(MODEL_DIR)/FondoDePantalla.cpp
RESOLUCION = $(MODEL_DIR)/Resolucion.cpp

# executable name
EXE = 1942Multiplayer.exe

OBJS = $(BACKGROUND) $(TEXTURE) $(RESOLUCION) ./$(GAME)

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER) $(LINKER) -o $(EXE)
	
run:
	./$(EXE) $(p)
	
valgrind:
	valgrind --leak-check=full ./$(EXE) $(p)
