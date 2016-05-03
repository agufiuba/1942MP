####################   MODO  DE  USO   ####################
# g++ -std=c++11 ./Model/Vivible.h ./View/AvionView.cpp ./Model/Avion.cpp ./Control/Controller.cpp ./Main.cpp -lSDL -lSDL_image -o main
############################################################

PARAM = ./Model/Vivible.h ./View/AvionView.cpp ./View/MisilView.cpp ./Model/Avion.cpp ./Model/CompositeVivibles.cpp ./Model/Misil.cpp ./Control/ControllerMissiles.cpp ./Control/Controller.cpp ./Control/Timer.cpp ./View/Texture.cpp
PROGRAM = Main

OBJS = $(PARAM) ./$(PROGRAM).cpp

CC = g++

COMPILER = -std=c++11

LINKER = -pthread -lSDL2  -lSDL2_image

WARN = -Wno-write-strings

all : $(OBJS) 
	$(CC) $(OBJS)  $(COMPILER) $(LINKER) $(WARN) -o $(PROGRAM).exe

run:
	./$(PROGRAM).exe $(PARAM)
	