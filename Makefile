CC=g++

#parametri pentru compiler si pentru linker
CFLAGS=-c $(shell pkg-config --cflags sdl2) $(shell pkg-config --cflags CEGUI-0-OPENGL3) $(shell pkg-config --cflags assimp)
LDFLAGS=$(shell pkg-config --libs sdl2) -lGL -lGLU -lGLEW $(shell pkg-config --libs CEGUI-0-OPENGL3) $(shell pkg-config --libs assimp) -v -da -Q

#fisierele sursa si fisierele object
SOURCES=$(shell find . -name '*.cpp')
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=pr_practica

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo Linking
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

# '.c.o' the same as '%.o : %.c'
.cpp.o:
	@echo Compiling
	$(CC) $(CFLAGS) $< -o $@


