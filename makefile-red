####################   MODO  DE  USO   ####################

#modo: client o server

#Compile         : make p=modo

#Run default     : make p=modo run
#Run con archivo : make p=modo arch=nameArch run

#Valgrind default: make p=modo valgrind
#Valgrind archivo: make p=modo arch=nameArch valgrind

############################################################

MENU = ./src/libs/menu/Menu.cpp
PALETTE = ./src/libs/palette/palette.cpp
XML = ./src/libs/tinyxml2.cpp
XMLPARSER = ./src/xml/parser/XMLParser.cpp
LOGGER = ./src/libs/logger/Logger.cpp
DEFAULTS = ./src/utils/Defaults.cpp
CCONF = ./src/xml/conf/ClientConf.cpp
SCONF = ./src/xml/conf/ServerConf.cpp
PARAM = ./src/config/$(arch)
CONST = ./src/utils/K.cpp

ifeq ($(p), client)
	PROGRAM = src/client/client
endif
ifeq ($(p), server)
	PROGRAM = src/server/server
endif

ifeq ($(arch), )
	PARAM = 
endif

OBJS = $(MENU) $(PALETTE) $(CCONF) $(SCONF) $(XML) $(XMLPARSER) $(LOGGER) $(DEFAULTS) $(CONST) ./$(PROGRAM).cpp

CC = g++

COMPILER = -std=c++11

LINKER = -pthread

WARN = -Wno-write-strings

all : $(OBJS) 
	$(CC) $(COMPILER) $(LINKER) $(WARN) $(OBJS) -o $(PROGRAM).exe

run:
	./$(PROGRAM).exe $(PARAM)
	
valgrind:
	valgrind --leak-check=full ./$(PROGRAM).exe $(PARAM)
