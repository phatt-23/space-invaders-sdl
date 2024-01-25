#ifndef SDL_COLLISION_SDL_GAME_H
#define SDL_COLLISION_SDL_GAME_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "config.h"
#include "sdl_game.h"
#include "modules/fps.h"
#include "modules/dynamic_array.h"
#include "player.h"
#include "texture_manager.h"
#include "alien.h"
#include "sound_manager.h"

typedef struct Game_Context Context;
struct Game_Context {
    /*  functions  */
    void (*sdl_init)(Context* self, const char* title);
    void (*sdl_update)(Context* self);
    void (*sdl_render)(Context* self);
    void (*sdl_clean)(Context* self);
    void (*fps_set_delta)(FPS* fps);
    BOOL (*fullscreen)(Context* self);
    /* variables */
    BOOL running;
    BOOL started;           // the player has pressed some key and began to play
    BOOL end;               // the game ended - either by winning or losing
    BOOL paused;            // stops all the updating processes, much like the 'started' flag, but can be called mid-game
    BOOL sound;
    FPS fps;
    SDL_Window *window;
    SDL_Renderer *renderer;
    EventHandler event_handler;
    TextureManager textures;
    SoundManager sound_man;
    Player player;
    int arg_count;
    char** arguments;       // command line arguments
};

/*
 * You can use this as a Game_Context structure.
 * This Game_Context must be initialised by the
 * function new_context().
 */
extern Context game;


/*
 * Game Context constructor.
 */
extern Context new_context(int argc, char** argv);



#endif //SDL_COLLISION_SDL_GAME_H
