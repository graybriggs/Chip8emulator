

/*
 * Chip-8 emulator implementation
 *
 * Graham Briggs, February 2013
 */

#include <stdio.h>

#include "SDL/SDL.h"

#include "chip8.h"
#include "video.h"
#include "input.h"
#include "timer.h"
#include "stack.h"
#include "debug.h"


int main() {

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	video v;
	
  	input in;
	chip8cpu c8cpu;

	init_video(&v);
	chip8_init(&c8cpu);
	chip8_load_resources(&c8cpu, &v, &in);
	
	load_program(&c8cpu);


	for (;;) {
		/*
		parse_instruction(&c8cpu, 0x620A);  // set r2 to 0A
		parse_instruction(&c8cpu, 0x6308);  // set r3 to 09
		parse_instruction(&c8cpu, 0x8235);  // subtract r3 from r2
		parse_instruction(&c8cpu, 0xA003);  // set I to 123
		parse_instruction(&c8cpu, 0xF555);  // move r4 to [I]
		*/

		if (check_for_exit(c8cpu.p_input))
			break;

		SDL_Flip(c8cpu.p_video->screen);
		SDL_Delay(250);
		break;
	}
	print_chip8_state(&c8cpu);
	main_memory_dump(&c8cpu);
	
	SDL_Quit();

	return 0;
}
