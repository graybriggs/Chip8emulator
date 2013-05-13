
#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>a
#include <time.h>

#include "stack.h"


struct _chip8cpu {

  unsigned short program_counter;
  unsigned short I;               // memory address index pointer

  unsigned char main_memory[4096];// first 512 bytes reserved. Addressable range is 0x0200 - 0x0FFF
  unsigned char reg[16];          // 16 8 bit registers. Register 0xF doubles up as a carry flag

  struct _callstack stack;
};

typedef struct _chip8cpu chip8cpu;

extern void chip8_init(chip8cpu*);

extern void parse_instruction(unsigned short);

static void init_sprite_data(chip8emu*);
/* add_to_memory helper function */
static void add_to_memory(chip8emu*, char*, unsigned int);

extern int load_program();


#endif // CHIP8_H
