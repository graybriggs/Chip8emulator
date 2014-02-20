
CC     = gcc
CFLAGS = -std=c99 -Wall
LFLAGS = -Wall
DEBUG  = -g
LIB    = -lm

OBJS = chip8.o debug.o input.o stack.o timer.o video.o

c8emu : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -c -o c8emu

%.o : %.c
	$(CC) $(CFLAGS) $<

clean :
	rm -f *.o