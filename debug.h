
#ifndef DEBUG_H
#define DEBUG_H

#include "chip8.h"

extern void print_chip8_state(chip8cpu*);
extern void main_memomry_dump(chip8cpu*);
extern void print_stack_trace(chip8cpu*);


#endif
