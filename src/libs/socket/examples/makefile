LOGGER = ../../logger/Logger.cpp
SERVER_CONF = ../../../xml/conf/ServerConf.cpp
CLIENT_CONF = ../../../xml/conf/ClientConf.cpp
XML_PARSER = ../../../xml/parser/XMLParser.cpp
XML = ../../../libs/tinyxml2.cpp
K = ../../../utils/K.cpp
DEFAULTS = ../../../utils/Defaults.cpp
CLIENT = ../Client.cpp
OBJS = client.cpp $(CLIENT) $(LOGGER) $(CLIENT_CONF) $(XML_PARSER) $(XML) $(SERVER_CONF) $(K) $(DEFAULTS)
CC = g++
COMPILER_FLAGS = -std=c++11 -Wno-write-strings
OBJ_NAME = client.exe
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME)


