#Compile client: make p=client
#Compile server: make p=server
#Run client: make p=client run
#Run server: make p=server run

MENU = ./src/libs/menu/Menu.cpp
PALETTE = ./src/libs/palette/palette.cpp
LOGGER = ./src/logger/Logger.cpp

ifeq ($(p), client)
	PROGRAM = src/client/client
endif
ifeq ($(p), server)
	PROGRAM = src/server/server
endif

OBJS = $(MENU) $(PALETTE) $(LOGGER) ./$(PROGRAM).cpp

CC = g++

COMPILER = -std=c++11

LINKER = -pthread

all : $(OBJS) 
	$(CC) $(COMPILER) $(LINKER) $(OBJS) -o $(PROGRAM)

run:
	./$(PROGRAM)
