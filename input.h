
#ifndef INPUT_H
#define INPUT_H

#include "SDL/SDL.h"

struct _input {
  SDL_Event event;
};

typedef struct _input input;

extern char wait_key_press(input*);

extern int get_key_pressed(input*);

extern int check_for_exit(input*);

#endif // INPUT_H
