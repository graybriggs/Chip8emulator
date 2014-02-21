
#include "input.h"

char wait_key_press(input* in)
{

  SDL_Event e = in->event;
  char k_press = 0x00;

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
      case SDLK_KP0:
	k_press = 0x00;
	break;
      case SDLK_KP1:
	k_press = 0x01;
	break;
      case SDLK_KP2:
	k_press = 0x02;
	break;
      case SDLK_KP3:
	k_press = 0x03;
	break;
      case SDLK_KP4:
	k_press = 0x04;
	break;
      case SDLK_KP5:
	k_press = 0x05;
	break;
      case SDLK_KP6:
	k_press = 0x06;
	break;
      case SDLK_KP7:
	k_press = 0x07;
	break;
      case SDLK_KP8:
	k_press = 0x08;
	break;
      case SDLK_KP9:
	k_press = 0x09;
	break;
      case SDLK_a:
	k_press = 0x0A;
	break;
      case SDLK_b:
	k_press = 0x0B;
	break;
      case SDLK_c:
	k_press = 0x0C;
	break;
      case SDLK_d:
	k_press = 0x0D;
	break;
      case SDLK_e:
	k_press = 0x0E;
	break;
      case SDLK_f:
	k_press = 0x0F;
	break;
      default:
	break;
      }
    }
  }
  return k_press;
}

/* 
 *returns 1 on key press; 0 otherwise
 */
int get_key_pressed(input* in) {
	SDL_Event e = in->event;
	while(SDL_PollEvent(&e)) {
		if (e.type == SDL_KEYDOWN) {
			return 1;
		}
	}
	return 0;
}

int check_for_exit(input* in) {

	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			return 1;
	}
	return 0;
}
