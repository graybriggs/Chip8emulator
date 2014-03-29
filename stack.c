

#include "stack.h"

void stack_init(stack* s, chip8cpu* c8cpu) {
	/*
  for (int i = 0; i < 16; i++)
    s->callstack[i] = 0;
  */
  //memset(s->callstack, 0, 16);

  s->s_SP = c8cpu->main_memory[0x100];

  s->SP = 0;
}

void stack_push(stack* s, chip8cpu* c8cpu, unsigned char val) {
  /*
  s->callstack[s->SP] = val;
  s->SP++;

  if (s->SP >= STACK_LEVELS)
    s->SP = 0; // stack wraparound
  */

  *(s->p_SP) = val--;
}

unsigned char stack_top(stack* s) {
  //return s->callstack[s->SP];
  return *(s->p_SP);
}

void stack_pop(stack* s) {
  s->SP--;
}
