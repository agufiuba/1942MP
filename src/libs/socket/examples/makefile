LOGGER = ../../logger/Logger.cpp
SERVER_CONF = ../../../xml/conf/ServerConf.cpp
CLIENT_CONF = ../../../xml/conf/ClientConf.cpp
XML_PARSER = ../../../xml/parser/XMLParser.cpp
XML = ../../../libs/tinyxml2.cpp
K = ../../../utils/K.cpp
DEFAULTS = ../../../utils/Defaults.cpp
PALETTE = ../../palette/palette.cpp
CLIENT = ../Client.cpp
OBJS = client.cpp $(CLIENT) $(LOGGER) $(CLIENT_CONF) $(XML_PARSER) $(XML) $(SERVER_CONF) $(K) $(DEFAULTS) $(PALETTE)
CC = g++
COMPILER_FLAGS = -std=c++11 -Wno-write-strings
LINKER_FLAGS = -pthread
OBJ_NAME = client.exe
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)


