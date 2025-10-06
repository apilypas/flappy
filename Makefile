# Project settings
TARGET = flappy
SRC_DIR = src
OBJ_DIR = build

# Find all .cpp files in src/
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference

# Raylib (assuming installed system-wide)
LIBS = -lraylib -lm -ldl -lpthread -lGL -lX11

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LIBS)

# Compile each .cpp into .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Run the game
run: $(TARGET)
	./$(TARGET)
