#include "sdl_context.h"
#include "projectile.h"
#include "alien.h"
#include "logic_handler.h"
#include "modules/random_interval.h"
#include "explosion.h"
#include "bunk.h"
// declarations --------------------------------------------------------------------------------------------------------
/*
 * Takes in a pointer to the game context, initialises it and returns it.
 */
static void context_sdl_init(Context* context, const char* title);
/*
 * Destroys the passed Context.
 */
static void context_sdl_clean(Context* context);

/*
 * The main update function.
 * Handles all the updating of the game objects.
 * (consist of all the update function of each element in the game)
 */
static void context_update_function(Context* context);

/*
 * The main render function.
 * Handles all the rendering of the game.
 */
static void context_render_function(Context* context);

/* Checks the arguments from the command line
 * This one checks whether the user wants to play on fullscreen-mode
 */
static BOOL context_fullscreen(Context* context);
// utils fn ------------------------------------------------------------------------------------------------------------


// variables -----------------------------------------------------------------------------------------------------------
Context game;
// function definitions ------------------------------------------------------------------------------------------------
Context new_context(int argc, char** argv) {
    Context newContext;
    newContext.sdl_init = &context_sdl_init;
    newContext.sdl_clean = &context_sdl_clean;
    newContext.fps_set_delta = &setDelta;
    newContext.sdl_update = &context_update_function;
    newContext.sdl_render = &context_render_function;
    newContext.fullscreen = &context_fullscreen;
    newContext.arguments = argv;
    newContext.arg_count = argc;
    newContext.started = FALSE;
    newContext.end = FALSE;
    newContext.paused = FALSE;
    newContext.sound = TRUE;
    return newContext;
}
static BOOL context_fullscreen(Context* context) {
    for(int i = 0; i < context->arg_count; i++)
        if(strcmp(context->arguments[i], "-f") == 0)
            return TRUE;
    return FALSE;
}
extern void context_sdl_init(Context* context, const char* title) {
    rand_init();
    int flag = SDL_WINDOW_SHOWN;
    if(context->fullscreen(context))
      flag = SDL_WINDOW_FULLSCREEN;
    if (SDL_Init(SDL_INIT_VIDEO)) {
      fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError()); exit(1);
    }
    if (!(context->window = SDL_CreateWindow( title,
                                              SDL_WINDOWPOS_CENTERED,
                                              SDL_WINDOWPOS_CENTERED,
                                              SCREEN_WIDTH,
                                              SCREEN_HEIGHT,
                                              flag ))) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();exit(1);
    }
    if (!(context->renderer = SDL_CreateRenderer( context->window,
                                                 -1,
                                                 SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ))) {
        SDL_DestroyWindow(context->window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();exit(1);
    }

    context->fps = new_FPS();
    context->running = TRUE;
    context->event_handler = new_handler();
    context->player = new_player();
    context->textures = new_texture_manager(context->renderer);
    context->sound_man = new_sound_manager();
    context->fps.last = SDL_GetPerformanceCounter();
    init_aliens(&Aliens);
    init_Explosions(&Explosions);
    init_bunkers(&Bunkers);
    format_hi_score();
    init_startup_text(&context->textures);
    context->sound_man.init_audio();
    context->sound_man.load_audio(&context->sound_man);
}
extern void context_sdl_clean(Context* context) {
    remove_bunkers(&Bunkers, TRUE);
    dynarray_free(&Aliens);
    dynarray_free(&Explosions);
    context->player.clean(&game.player);
    context->textures.destroy(&context->textures);
    context->sound_man.clean_audio(&context->sound_man);
    SDL_DestroyRenderer(context->renderer);
    SDL_DestroyWindow(context->window);
    SDL_Quit();
}
extern void context_update_function(Context* context) {
  setDelta(&game.fps);
  if(!game.paused && game.started) {
      score_appender();
      free_all_text(&context->textures);
      update_all_text(&context->textures);
      context->player.update(&context->player);
      update_projectiles(&game.player.projectiles);
      remove_projectiles(&game.player.projectiles);
      update_aliens(&Aliens);
      remove_aliens(&Aliens);
      explosions_update(&Explosions);
      explosions_remove(&Explosions);
      update_bunkers(&Bunkers);
      remove_bunkers(&Bunkers, FALSE);
      handle_logic();
  }
}
extern void context_render_function(Context* context) {
    SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 255);
    SDL_RenderClear(context->renderer);
    if(!game.started) {
      render_startup_text(&context->textures);
    } else {
        context->textures.score.render(&context->textures.score);
        context->textures.lives.render(&context->textures.lives);
        render_bunkers(&Bunkers);
        render_projectiles(&game.player.projectiles);
        context->player.render();
        render_aliens(&Aliens);
        explosions_render(&Explosions);
        dim_screen(&context->textures);
    }
    SDL_RenderPresent(context->renderer);
}
