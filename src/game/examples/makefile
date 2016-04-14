# USAGE: make file=FILENAME

# files to compile
OBJS = $(file).cpp
# compiler
CC = g++
# compilation options
#COMPILER_FLAGS = 
# libraries to link
LINKER_FLAGS = -lSDL2
# executable name
OBJ_NAME = $(file).exe

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
