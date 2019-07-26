CC       = gcc
CXX      = g++
CFLAGS   = -std=c99 -Wall -g -O2
CXXFLAGS = $(CFLAGS)
LIBS     = -lgdi32 -lgdiplus

OBJS     = window.o paint.o io.o misc.o

all : libCanvas demo

demo : libCanvas demo.o
	$(CC) -I. -L./Release/ demo.o -llibCanvas -o ./Release/demo.exe

libCanvas : $(OBJS)
	$(CC) -shared $(OBJS) $(LIBS) -o ./Release/libCanvas.dll -Wl,--out-implib,./Release/libCanvas.a

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	rm -f *.o ./Release/*

