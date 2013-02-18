
/*
 * Chip-8 emulator
 */

#ifndef STACK_H
#define STACK_H

#define STACK_LEVELS 16

/*
 * The Chip-8 the stack is only used to store return addresses when
 * subroutines are called. That is, the memory address that the program ycounter is holding.
 */

struct _callstack {

  unsigned short callstack[STACK_LEVELS];   // callstack is only used to store return addresses when subroutines are called
  unsigned short SP;              // stack pointer;
};

typedef struct _callstack stack;

extern void stack_init(stack*);
extern void stack_push(stack*, unsigned short); // program counter - unsigned short
extern unsigned short stack_top();
extern void stack_pop();


#endif
