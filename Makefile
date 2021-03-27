# barebones makefile, used by phoenix because phoenix knows how to use it better than batch
# also ethan, I'm not gonna heavily document this one because you're probably not gonna use it
# made for windows

# name and compiler
NAME=HistoryProject
CC=g++

# set this to console if you want the console window, or windows if not
SUBSYSTEM=console
# set this to -static-libgcc -static-libstdc++ if you want to statically link std (for release)
STATICSTD=
# set this to -g for debugging info (when profiling)
DEBUG=

# directories
INSTALL_DIR=./bin/
INCLUDE_DIR=./include/
FT_INCLUDE_DIR=./freetype/
LIB_DIR=./lib/
SRC_DIR=./src/

# src and libs
_SRC=*.cpp poor/*.cpp poor/glad/glad.c
SRC=$(patsubst %,$(SRC_DIR)%,$(_SRC))

LIBS=-lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -lsfml-network-s -lws2_32 -lsfml-audio-s -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lsfml-system-s  -lwinmm

# cflags
CFLAGS=-I$(INCLUDE_DIR) -I$(INCLUDE_DIR)openal/ -L$(LIB_DIR) $(DEBUG) -Wall -Wno-write-strings -Wl,-subsystem,$(SUBSYSTEM) $(STATICSTD)

# rules
all: $(SRC)
	$(CC) $(SRC) -o $(INSTALL_DIR)$(NAME) $(CFLAGS) $(LIBS)
