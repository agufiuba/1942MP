LOGGER = ../../../../logger/Logger.cpp
PALETTE = ../../../../palette/palette.cpp
CLIENT = ../../../Client.cpp
TMT = ../../../../transmitter/Transmitter.cpp
OBJS = sendNameColor.cpp $(CLIENT) $(LOGGER) $(PALETTE) $(TMT)
CC = g++
COMPILER_FLAGS = -std=c++11
LINKER_FLAGS = -pthread
OBJ_NAME = sendNameColor.exe
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)


