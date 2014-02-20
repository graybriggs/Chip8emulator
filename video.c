
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

void draw_chip8_pixel(video* v, unsigned char on_off)
{
  SDL_Surface* scr = v->screen;

  for (int h = 0; h < 8; ++h) {
    for (int w = 0; w < 8; ++w) {
      if (on_off)
	     (unsigned int*)scr->pixels[h * (scr->pitch / 4) + w] = 0xFF;
      else
	     (unsigned int*)scr->pixels[h * (scr->pitch / 4) + w] = 0x00;
    }
  }

}

int draw_screen(video* v)
{
  SDL_Surface* scr = v->screen;

  if (SDL_MUSTLOCK(scr)) {
    if (SDL_LockSurface(scr) < 0) {
      fprintf(stderr, "Failed locking screen surface\n");
      return -1;
    }
  }

  for (int y = 0; y < VIDEO_H;  ++y) {
    for (int x = 0; x < VIDEO_W; ++x) {
      draw_chip8_pixel(v, v->virtual_screen[y * VIDEO_H + x]);
    }
  }

  if (SDL_MUSTLOCK(scr)) {
    SDL_UnlockSurface(scr);
  }
}
