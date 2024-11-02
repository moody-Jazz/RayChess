# -*- MakeFile -*-

# Compiler and flags
CXX = g++

CXXFLAGS = -O1 -Wall -std=c++11
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm

# Variables
RAYLIB_DIR = C:/raylib
RAYLIB_SRC = $(RAYLIB_DIR)/src
RAYLIB_INC = include
RAYLIB_LIB = lib

# Source Files
SRC = $(wildcard src/*.cpp)

#Object files in the same directory
OBJ = $(patsubst src/%.cpp, obj/%.o,$(SRC))

TARGET = raychess.exe

# Default target
default: check_raylib build_raylib $(TARGET)

# Check if Raylib is installed or not
check_raylib:
	@if [ ! -d "$(RAYLIB_DIR)" ]; then \
		echo "Raylib is not installed in C drive"; \
	fi

# build the static libraries if it doesn' exist
build_raylib:
	@if [ ! -f "$(RAYLIB_SRC)\\libraylib.a" ]; then \
		echo "Static file libraylib.a not found"; \
		echo "Building Static file ....."; \
		cd $(RAYLIB_SRC) && mingw32-make PLATFORM=PLATFORM_DESKTOP; \
	fi

	@if [ ! -d "include" ]; then \
		mkdir include; \
	fi

	@if [ ! -d "lib" ]; then \
		echo "lib directory not found"; \
		echo "Creating lib directory...."; \
		mkdir lib; \
	fi

	@if [ ! -f "include\\raylib.h" ]; then \
		echo "Copying raylib.h in current project..."; \
		cp $(RAYLIB_SRC)\\raylib.h include\\raylib.h; \
	fi

	@if [ ! -f "lib\\libraylib.a" ]; then \
		echo "Copying libraylib.a in current project..."; \
		cp $(RAYLIB_SRC)\\libraylib.a lib\\libraylib.a; \
	fi

# Linking
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(CXXFLAGS) -I $(RAYLIB_INC) -L $(RAYLIB_LIB) $(LDFLAGS)

# Compiling
obj/%.o: src/%.cpp
	@if [ ! -d "obj" ]; then \
		echo "Creating obj directory..."; \
		mkdir obj; \
	fi
	$(CXX) -c $< -o $@ $(CXXFLAGS) -I $(RAYLIB_INC)

# Clean target
clean:
	rm -r obj -f $(TARGET)