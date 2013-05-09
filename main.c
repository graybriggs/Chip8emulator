

/*
 * Chip-8 emulator implementation
 *
 * Graham Briggs, February 2013
 */

#include <stdio.h>

#include "video.h"
#include "input.h"

int main()
{
  chip8emulator c8emu;
  video video;
  input input;

  unsigned char screen[64 * 32];

  chip8_init(c8emu);

  for (;;) {

  }

  return 0;
}
