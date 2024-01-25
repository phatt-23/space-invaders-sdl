#ifndef SDL_GAME_SDL_GAME_H
#define SDL_GAME_SDL_GAME_H

#include <SDL2/SDL.h>
#include "config.h"
#include "modules/fps.h"

typedef struct eventHandler EventHandler;

struct eventHandler {
    void (*handle_events)(EventHandler* event_handler);
    void (*quit)(void);
    void (*generate)(void);
    SDL_Event event;                    // can only handle one input -- used for quitting the game
    const Uint8* key_states;            // for keyboard input (this way it can process multiple keyboard inputs)
};

/*
 * Construct the new EventHandler structure.
 */
extern EventHandler new_handler();


extern void score_appender(void);


#endif //SDL_GAME_SDL_GAME_H
