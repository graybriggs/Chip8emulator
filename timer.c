
#include "timer.h"


/*
 * Delay Timer
 *
 */

void init_timer(timer* timer)
{
  timer->DT = 0;
  timer->last = SDL_GetTicks();
}

void set_delay_timer(timer* timer, unsigned char delay)
{
  timer->DT = delay;
  timer->last = SDL_GetTicks();
}

unsigned char get_delay_timer(timer* timer)
{
  return timer->DT;
}

void update_timer(timer* timer)
{
  if (timer->DT > 0) {
    if ((SDL_GetTicks() - timer->last) > (1000.0f / 60.0f)) {
      timer->DT -= 1;
      timer->last = SDL_GetTicks();
    }
  }
}


/*
 * Sound Timer
 *
 */


void init_sound_timer(sound_timer* timer)
{
  timer->DT = 0;
  tiemr->last = SDL_GetTicks();
}

void set_sound_timer(sound_timer* timer, unsigned char sound_for)
{
  timer->ST = sound_for;
  timer->last = SDL_GetTicks();
}

void update_timer(sound_timeyr* timer)
{
  if (timer->ST > 0) {
    if ((SDL_GetTicks() - timer->last) > (1000.0f / 60.0f)) {
      timer->ST -= 1;
      timer->last = SDL_GetTicks();
    }
  }
}
