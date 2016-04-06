#Compile client: make p=client
#Compile server: make p=server
#Run client: make p=client run
#Run server: make p=server run

MENU = ./src/libs/menu/Menu.cpp
PALETTE = ./src/libs/palette/palette.cpp
XML = ./src/libs/tinyxml2.cpp
XMLPARSER = ./src/xml/parser/XMLParser.cpp
LOGGER = ./src/logger/Logger.cpp
DEFAULTS = ./src/utils/Defaults.cpp
CCONF = ./src/xml/conf/ClientConf.cpp
SCONF = ./src/xml/conf/ServerConf.cpp

ifeq ($(p), client)
	PROGRAM = src/client/client
endif
ifeq ($(p), server)
	PROGRAM = src/server/server
endif

OBJS = $(MENU) $(PALETTE) $(CCONF) $(SCONF) $(XML) $(XMLPARSER) $(LOGGER) $(DEFAULTS) ./$(PROGRAM).cpp

CC = g++

COMPILER = -std=c++11

LINKER = -pthread

WARN = -Wno-write-strings

all : $(OBJS) 
	$(CC) $(COMPILER) $(LINKER) $(WARN) $(OBJS) -o $(PROGRAM)

run:
	./$(PROGRAM)
