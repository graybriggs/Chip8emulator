
#include "video.h"


void init_video(video* v)
{
	v->screen = SDL_SetVideoMode(VIDEO_W, VIDEO_H, VIDEO_BPP, SDL_SWSURFACE);
	clear_screen(v);
}

void clear_screen(video* v)
{	
	for (int i = 0; i < SCREEN_H; ++i) {
		for (int j = 0; j < SCREEN_W; ++j) {
			virtual_screen[i * SCREEN_H + j] = 0;
		}
	}
}

void draw_sprite(video* v, unsigned char x, unsigned char y)
{
	SDL_Surface* scr = v->screen;
	
	for (int i = 0; i < y; ++i) {
		for (int j = 0; j < x; j++) {
			
		}
	}
}
