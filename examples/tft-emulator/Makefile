# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Target executable
TARGET = tft-emulator

# Source files
SRCS = main.cpp

# SFML includes and libraries (adjust these paths to your environment)
SFML_LIB_DIR = /usr/local/Cellar/sfml/2.6.1/lib
SFML_INCLUDE_DIR = /usr/local/Cellar/sfml/2.6.1/include/SFML

# Link SFML libraries (Make sure to include the window and system components)
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Compiler flags and linker flags for SFML
INCLUDES = -I$(SFML_INCLUDE_DIR)
LDFLAGS = -L$(SFML_LIB_DIR) $(SFML_LIBS)

# Default target
all: $(TARGET)

# Build target executable
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRCS) -o $(TARGET) $(LDFLAGS)

# Clean up
clean:
	rm -f $(TARGET)

