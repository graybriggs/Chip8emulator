
#ifndef VIDEO_H
#define VIDEO_H

#include "SDL/SDL.h"

#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32
#define VIDEO_BPP 8

struct _video {
  SDL_Surface* screen;
  unsigned char virtual_screen[VIDEO_WIDTH * VIDEO_HEIGHT];
};

typedef struct _video video;

extern void init_video(video*);

extern void clear_screen(video*);

// draws sprite at coordiantes x, y.
extern void draw_sprite(video*, const unsigned char, const unsigned char);

extern unsigned char get_sprite_at(unsigned char); // for instruction FX29

static void draw_chip8_pixel(video*, unsigned char); // called from draw_screen
extern int draw_screen(video*);

#endif // VIDEO_H
