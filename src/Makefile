# Variables
CC = g++
LANG_STD = -std=c++17
COMPILER_FLAGS = -Wall -Wfatal-errors
#INCLUDE_PATH = -I"../external/SDL2-2.0.22/lib/"
SRC_FILES = ./SoftwareRenderer/*.cpp
LINKER_FLAGS = -lSDL2
OBJ_NAME = softwarerenderer.bin

build:
	mkdir -p ./bin
	$(CC) $(COMPILER_FLAGS) $(LANG_STD) $(SRC_FILES) $(LINKER_FLAGS) -o ./bin/$(OBJ_NAME)

run:
	./bin/$(OBJ_NAME)

clean:
	rm $(OBJ_NAME)
