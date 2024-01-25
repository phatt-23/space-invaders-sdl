CXX = g++
CXXFLAGS = -g -Wpedantic -Wall -Wno-unused-result -Wextra -Werror=uninitialized -Werror=vla -Werror=return-type
VAL = valgrind -q --tool=memcheck --track-origins=no --error-exitcode=1 --track-origins=yes
SDL = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
SRC_MAIN = main.c
SRC_DIR = src/*.c
MODULES = src/modules/*.c
TARGET = program
GAMEMODE = -gamemode0
FULLSCREEN = -f

all:
	$(CXX) $(SRC_DIR) $(MODULES) $(SDL) -o$(TARGET) $(CXXFLAGS)
	./$(TARGET)
rm:
	rm $(TARGET)


