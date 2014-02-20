

/*
 * Chip-8 emulator implementation
 *
 * Graham Briggs, February 2013
 */

#include <stdio.h>

#include "video.h"
#include "input.h"

int main() {

	video video;
  	input input;
	chip8cpu c8cpu;

	chip8_init(&c8emu);
	init_video(&video);
	chip8_load_resources(&c8cpu, &video, &input);

	


	return 0;
}
