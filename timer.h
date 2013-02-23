
#ifndef TIMER_H
#define TIMER_H

#include "SDL/SDL.h"

struct _timer {
  Uint32 DT;  // delay timer register
  Uint32 last;
};

typedef struct _timer timer;


extern void init_timer(timer*);
extern void set_delay_timer(timer*, unsigned char);
extern unsigned char get_delay_timer(timer*);
extern void update_timer(timer*);

#endif // TIMER_H
