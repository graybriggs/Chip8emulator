
#ifndef CHIP8_H
#define CHIP8_H

#include "types.h"

//#include "stack.h"
#include "timer.h"

#include "video.h"
#include "input.h"

#define PROGRAM_MEMORY_START 0x0200
#define CHIP_8_MEMORY_MAX 0x1000 // 4096 memory locations
#define NUM_REGISTERS 16
#define STACK_LEVELS 16
#define STACK_START_ADDR 0x100

struct _chip8cpu {

  WORD program_counter;
  BYTE I;               // 16bit memory address index pointer

  BYTE main_memory[CHIP_8_MEMORY_MAX];// first 512 bytes reserved. Addressable range is 0x0200 - 0x0FFF
  BYTE reg[NUM_REGISTERS];          // 16 8 bit registers. Register 0xF doubles up as a carry flag

  WORD SP; // stack pointer;

  //
  timer timer;
  sound_timer sound_timer;

  // pointers to video and input resources

  video* p_video;
  input* p_input;
};

typedef struct _chip8cpu chip8cpu;


extern void chip8_init(chip8cpu*);

extern void advance_pc(chip8cpu*);
extern WORD read_instruction(chip8cpu* c8cpu);
extern void fetch_decode_execute_cycle(chip8cpu* c8cpu);

// assigns video and input to chip8 internal pointers
extern void chip8_load_resources(chip8cpu*, video*, input*);
extern void parse_instruction(chip8cpu*, WORD);

// chip-8 stack
extern void stack_init(chip8cpu*);
extern void stack_push(chip8cpu*, BYTE); // program counter - unsigned short
extern BYTE stack_top(chip8cpu*);
extern void stack_pop(chip8cpu*);


static void init_sprite_data(chip8cpu*);
/* add_to_memory helper function */
static void add_to_memory(chip8cpu*, BYTE*, WORD);

extern int load_program(chip8cpu*);


#endif // CHIP8_H
