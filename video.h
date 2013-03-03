
#ifndef VIDEO_H
#define VIDEO_H

#include "SDL/SDL.h"

#define VIDEO_W 64
#define VIDEO_H 32
#define VIDEO_BPP 8

struct _video {
	SDL_Surface* screen;
	unsigned char virtual_screen[VIDEO_W * VIDEO_H];
};

typedef struct _video video;

extern void init_video(video*);

extern void clear_screen(video*);

// draws sprite at coordiantes x, y.
extern void draw_sprite(video*, const unsigned char, const unsigned char, const chip8cpu);

extern char get_sprite_at(char); // for FX29

#endif // VIDEO_H

