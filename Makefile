###############################################################################
# Declare Makefile variables
###############################################################################
CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
LINKER_FLAGS = -lm -lSDL2 -lSDL2_image
SRC_FILES = ./src/*.cpp ./src/Game/*.cpp ./src/ECS/*.cpp ./src/AssetStore/*.cpp
INCLUDE_PATHS = -I "./libs"
OBJ_NAME = game

###############################################################################
# Declare Makefile rules
###############################################################################
build:
	$(CC) $(LANG_STD) $(COMPILER_FLAGS) $(SRC_FILES) $(INCLUDE_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME);

debug:
	$(CC) $(LANG_STD) $(COMPILER_FLAGS) $(SRC_FILES) $(INCLUDE_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME) -g;

clean:
	rm ./$(OBJ_NAME);

run:
	./$(OBJ_NAME);