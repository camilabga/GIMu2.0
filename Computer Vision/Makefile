.SUFFIXES:
.SUFFIXES: .c .cpp

CC = gcc
GCC = g++

OBJS = Cow.o 
LIBS = -laruco

.c:
	$(CC) -I$(INCDIR) $(CFLAGS) $< $(GL_LIBS) -o $@ $(LIBS)

.cpp:
	$(GCC) -Wall -Wunused -std=c++11 -O2 `pkg-config --cflags opencv` $< -o $@ `pkg-config --libs opencv` $(LIBS)


