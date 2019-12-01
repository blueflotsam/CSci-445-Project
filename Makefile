PROG=Harezini
CC=g++
EXT=.cpp
INCLUDE   = -I/usr/include/
INCLUDEGL = -I/usr/include/GL/
LIBDIR    = -L/usr/lib/x86_64-linux-gnu/ -L.
LIBS      = -lGL -lGLU -lglut -lm
RAYGL     = ./raygl/raygl
#PGMIO = ./pgmIO.o

CFLAGS = -Wall -DGL_GLEXT_PROTOTYPES -fPIC
LFLAGS = -static -c

$(PROG): $(PROG).o
	$(CC) $@$(EXT) $(CFLAGS) $(INCLUDE) $(INCLUDEGL) $(RAYGL).o $(LIBDIR) $(LIBS) -o $@
raygl: 
	$(CC) $(RAYGL).cpp $(INCLUDE) $(INCLUDEGL) $(LFLAGS) -o $(RAYGL).o
clean:
	rm -f $(PROG).o
	rm -f $(PROG)
clean-raygl:
	rm -f $(RAYGL).o
clean-frames:
	rm frames/*