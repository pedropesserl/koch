CC = g++
PROGRAM = koch
CFLAGS = -Iinclude -Wall -Wextra -Wshadow -Wno-missing-field-initializers
LFLAGS = -lm -lraylib 

_OBJ = main.o
_DEPS = raylib.h raymath.h

SDIR = src
IDIR = include

OBJ = $(_OBJ:%=$(SDIR)/%)
DEPS = $(_DEPS:%=$(IDIR)/%)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(PROGRAM)
all: clean

$(PROGRAM): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	rm -f $(OBJ)

purge:
	rm -f $(OBJ) $(PROGRAM)
