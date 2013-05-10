
#include "debug.h"

void print_chip8_state(chip8cpu* c8cpu)
{
  printf("Dump state of CPU registers\n");
  printf("Program Counter -        %X\n", c8cpu->program_counter);
  printf("Memory address pointer - %X\n", c8cpu->I);
  printf("Registers :- \n");
  
  for (int i = 0; i < 16; ++i) {
    printf("Register[%d] - %X\n", i, c8cpu->reg[i]);
  }
}

void main_memory_dump(chip8cpu* c8cpu)
{
  for (int i = 0; i < 4096; i += 16) {
    for (int j = 0; j < 16; ++j) {
      printf("%X\t%X", i, j);
    }
    printf("\n");
  }
}

void print_stack_trace(stack* stack)
{
  printf("Stack Pointer - %X\n\n", stack->SP);
  
  for (int i = 0; i < STACK_LEVELS; i++) {
    printf("%d - %X\n", stack->callstack[i]);
  }
}
