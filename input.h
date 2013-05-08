
#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"

struct _input {
  SDL_Event event;
};

typedef struct _input input;

extern char wait_key_press(input*);


#endif // INPUT_H
