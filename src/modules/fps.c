#include "fps.h"
#include <SDL2/SDL.h>
// fn declarations -----------------------------------------------------------------------------



// var definitions -----------------------------------------------------------------------------

FPS fps;

//constructor ----------------------------------------------------------------------------------
extern FPS new_FPS() {
  FPS fps_s;
  fps_s.now = 0;
  fps_s.last = 0;
  fps_s.delta = 0;
  return fps_s;
}


// fn definitions -----------------------------------------------------------------------------

void setDelta(FPS *Fps) {
    Fps->now = SDL_GetPerformanceCounter();
    Fps->delta = (double) (Fps->now - Fps->last) / (double) SDL_GetPerformanceFrequency();
    Fps->last = Fps->now;
}
