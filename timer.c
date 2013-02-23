
#include "timer.h"



void init_timer(timer* timer)
{
  timer->timer = 0;
}

void set_delay_timer(timer* timer, unsigned char delay)
{
  timer->time = delay;
  timer->last = SDL_GetTicks();
}

unsigned char get_delay_timer(timer* timer)
{
  return timer->DT;
}

void update_timer(timer* timer)
{
  if (timer->DT > 0) {
    if ((SDL_GetTicks() - timer->last) > (1000.0f / 60.0f))
      timer->DT -= 1;
    else
      timer->last = SDL_GetTicks();
  }

}
