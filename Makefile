
CC     = gcc
CFLAGS = -std=c99 -Wall
LFLAGS = -Wall
DEBUG  = -g
LIB    = -lSDL

OBJS = chip8.o debug.o input.o stack.o timer.o video.o main.o

c8emu : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o c8emu $(LIB)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f *.o
