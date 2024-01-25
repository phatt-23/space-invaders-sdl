#ifndef SDL_GAME_FPS_H
#define SDL_GAME_FPS_H

#include <stdint.h>

typedef struct fps_struct FPS;
struct fps_struct {
    uint64_t last;      // outside gameloop call .last = SDL_GetPerformanceCounter()
    uint64_t now;       // inside gameloop call .now = SDL_GetPerformanceCounter() then .last = .now;
    double delta;       // delta = (double) (fps->now - fps->last) / (double) SDL_GetPerformanceFrequency();
};

extern FPS fps;

extern FPS new_FPS(void);

/*
 * Calculates the numbers in FPS struct (delta time)
 */
extern void setDelta(FPS *fps);

#endif //SDL_GAME_FPS_H
