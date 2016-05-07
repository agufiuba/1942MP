OBJ = initialWindow.cpp
CC = g++
LINKER_FLAGS = -std=c++11 -lSDL2 -lSDL2_ttf -pthread
OBJS = ../libs/xm_sdl/XM_SDL.cpp ../game/view/Texture.cpp ../game/view/Screen.cpp ../game/controller/Timer.cpp ../game/Game.cpp ../libs/socket/Client.cpp ../libs/logger/Logger.cpp ../libs/palette/palette.cpp
OBJ_NAME = initialWindow.exe
all: $(OBJS)
	$(CC) $(OBJ) $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)
