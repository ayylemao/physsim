# Compiler
CC := g++
CFLAGS := -Wall -Wextra -pedantic -std=c++17

# Directories
SRC_DIR := src
INCLUDE_DIR := src/include
BUILD_DIR := build
BIN_DIR := bin

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Include directories
INCLUDES := -I$(INCLUDE_DIR)

# Executable
EXECUTABLE := $(BIN_DIR)/sim

# SFML Libraries
SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(SFML_LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/* $(BIN_DIR)/*

.PHONY: all clean