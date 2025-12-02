CC = g++
PROGRAM = koch
SRC_PATH = src
INC_PATH = include
RAYLIB_PATH = /home/pedro/software/raylib/src

CFLAGS = -Iinclude -I$(RAYLIB_PATH) -Wall -Wextra -Wshadow -Wno-missing-field-initializers -g
LFLAGS = -L$(RAYLIB_PATH) -lm -lraylib

_SRC = main geometry koch
_DEPS = geometry.hpp koch.hpp

SRC = $(_SRC:%=$(SRC_PATH)/%.cpp)
DEPS = $(_DEPS:%=$(INC_PATH)/%) $(RAYLIB_PATH)/raylib.h $(RAYLIB_PATH)/raymath.h

$(PROGRAM): $(SRC) $(DEPS)
	$(CC) -o $@ $(SRC) $(CFLAGS) $(LFLAGS)

all: $(PROGRAM)

purge:
	rm -f $(PROGRAM)
