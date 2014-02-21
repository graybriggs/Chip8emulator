
#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "stack.h"
#include "timer.h"

#include "video.h"
#include "input.h"

struct _chip8cpu {

  unsigned short program_counter;
  unsigned short I;               // memory address index pointer

  unsigned char main_memory[4096];// first 512 bytes reserved. Addressable range is 0x0200 - 0x0FFF
  unsigned char reg[16];          // 16 8 bit registers. Register 0xF doubles up as a carry flag


  stack stack;

  //
  timer timer;
  sound_timer sound_timer;


  // pointers to video and input resources

  video* p_video;
  input* p_input;

};

typedef struct _chip8cpu chip8cpu;


extern void chip8_init(chip8cpu*);

// assigns video and input to chip8 internal pointers
extern void chip8_load_resources(chip8cpu*, video*, input*);

extern void parse_instruction(chip8cpu*, unsigned short);

static void init_sprite_data(chip8cpu*);
/* add_to_memory helper function */
static void add_to_memory(chip8cpu*, char*, unsigned short);

extern int load_program(chip8cpu*);


#endif // CHIP8_H
