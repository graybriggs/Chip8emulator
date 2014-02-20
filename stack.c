

#include "stack.h"

void stack_init(stack* s)
{
	/*
  for (int i = 0; i < 16; i++)
    s->callstack[i] = 0;
  */
  memset(s->callstack, 0, 16);

  s->SP = 0;
}

void stack_push(stack* s, unsigned short val)
{
  s->callstack[s->SP] = val;
  s->SP++;

  if (s->SP >= STACK_LEVELS)
    s->SP = 0; // stack wraparound
}

unsigned short stack_top(stack* s)
{
  return s->callstack[s->SP];
}

void stack_pop(stack* s)
{
  s->SP--;
}
