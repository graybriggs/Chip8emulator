
#ifndef CHIP8_H
#define CHIP8_H


struct _callstack {

  unsigned short callstack[16];   // callstack is only used to store return addresses when subroutines are called
  unsigned short SP;              // stack pointer;
};

struct _chip8cpu {

  unsigned short program_counter;
  unsigned short I;               // memory address index pointer

  unsigned char main_memory[4096];// first 512 bytes reserved. Addressable range is 0x0200 - 0x0FFF
  unsigned char reg[16];          // 16 8 bit registers. Register 0xF doubles up as a carry flag

  struct _callstack stack;
};





#endif // CHIP8_H
