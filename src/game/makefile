####################   MODO  DE  USO   ####################
# g++ -std=c++11 ./model/Vivible.h ./view/Avionview.cpp ./model/Avion.cpp ./controller/Controller.cpp ./Main.cpp -lSDL -lSDL_image -o main
############################################################

PARAM = ./model/Vivible.h ./view/AvionView.cpp ./view/MisilView.cpp ./model/CompositeVivibles.cpp ./model/Avion.cpp ./model/Misil.cpp ./controller/Controller.cpp ./controller/ControllerMissiles.cpp ./controller/Timer.cpp ./view/Texture.cpp ../libs/xm_sdl/XM_SDL.cpp
PROGRAM = Main

OBJS = $(PARAM) ./$(PROGRAM).cpp

CC = g++

COMPILER = -std=c++11

LINKER = -pthread -lSDL2 

WARN = -Wno-write-strings

all : $(OBJS) 
	$(CC) $(OBJS)  $(COMPILER) $(LINKER) $(WARN) -o $(PROGRAM).exe

run:
	./$(PROGRAM).exe $(PARAM)
	
