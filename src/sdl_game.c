#include "sdl_game.h"

#include "sdl_context.h"
#include "bunk.h"
// fn declarations -------------------------------------------------------------------------------
/*
 * Event Handler "constructor".
 * Sets the function pointers.
 */
EventHandler new_handler();

/*
 * Handles game events. This is the main event handling function.
 */
static void handler_handle_events(EventHandler* event_handler);

/*
 * Sets the "is running" to FALSE.
 * Breaks the main while loop in the main.c file.
 */
static void quit_game(void);


static void generate_new_round(void);

/*
 * Erases everything in the score-list.txt.
 * Start farming the highest score all over again.
 */
static void nuke_score_list(void);

// ---------
#define SHOOTING_COOLDOWN 0.5 
double last_shot_time = 0.0;

// fn implementation -------------------------------------------------------------------
EventHandler new_handler() {
    EventHandler new_handler;
    new_handler.handle_events = &handler_handle_events;
    new_handler.quit = &quit_game;
    new_handler.generate = &generate_new_round;
    new_handler.event.key.keysym.sym = 'a';
    return new_handler;
}
static void handler_handle_events(EventHandler* event_handler) {
    static double since_last_ = 0.0;
    while(SDL_PollEvent(&event_handler->event)) {
        if(event_handler->event.type == SDL_QUIT) {
            event_handler->quit();
        } else if(event_handler->event.type == SDL_KEYDOWN 
               && game.started == FALSE) {
            game.started = TRUE;
            printf("Game started!\n");
        } else if(event_handler->event.type == SDL_KEYDOWN
                &&event_handler->event.key.keysym.sym == SDLK_ESCAPE
                &&game.started == TRUE && game.player.lives && !game.end) {
                    game.paused = !game.paused;
        } else if(event_handler->event.type == SDL_KEYDOWN
                   &&event_handler->event.key.keysym.sym == SDLK_n)
                      nuke_score_list();
    }
    event_handler->key_states = SDL_GetKeyboardState(NULL);
    since_last_ += game.fps.delta;
    if(since_last_ >= 0.01) {

        if(((game.paused || !game.player.lives)
         || (game.player.lives && !Aliens.size))
         && event_handler->key_states[SDL_SCANCODE_Q])
              event_handler->quit();
        if(game.paused)
          if(event_handler->key_states[SDL_SCANCODE_M])
                game.sound_man.sound_on = !game.sound_man.sound_on;

        if(game.started == TRUE && game.paused == FALSE) {
          if(event_handler->key_states[SDL_SCANCODE_LEFT])
            game.player.move_left(&game.player);
          if(event_handler->key_states[SDL_SCANCODE_RIGHT])
            game.player.move_right(&game.player);
          if(event_handler->key_states[SDL_SCANCODE_SPACE]) {
              if((SDL_GetTicks() - last_shot_time) >= (SHOOTING_COOLDOWN * 1000.0)) {
                  game.player.shoot(&game.player);
                  last_shot_time = SDL_GetTicks();
              }
          }
          if(event_handler->key_states[SDL_SCANCODE_R]) {
            if(game.end)
              generate_new_round();
          }

        }
        since_last_ = 0.0;
    }
}
static void generate_new_round(void) {
    printf("Generating the next round.\n");
    game.end = FALSE;
    int temp = game.player.projectiles.size;
    for(int i = 0; i < temp; i++) {
        Projectile* t_projectile = (Projectile*) game.player.projectiles.items[i];
        t_projectile->removal = TRUE;
    }
    remove_projectiles(&game.player.projectiles);
    game.player.position.x = (SCREEN_WIDTH / 2) - (PLAYER_WIDTH / 2);
    if(!(game.player.lives)) game.player.score = 0.0;
    if(RESTORE_HEALTH || !(game.player.lives))
        game.player.lives = PLAYER_LIVES;
    for(int i = 0; i < Aliens.size; i++) {
        Alien* t_alien = (Alien*) Aliens.items[i];
        t_alien->alive = FALSE;
    }
    remove_aliens(&Aliens);
    init_aliens(&Aliens);
    remove_bunkers(&Bunkers, TRUE);
    init_bunkers(&Bunkers);
}
static void quit_game(void) {
    printf("Quitting the game!\n");
    game.running = FALSE;
    game.end = FALSE;
    score_appender();
}
static void nuke_score_list(void) {
  FILE* score_file = fopen(SCORE_FILE_PATH, "w");
  printf("Score file erased.\n");
  fclose(score_file);
}
extern void score_appender(void) {
    static BOOL write = FALSE;
    static BOOL locker = FALSE;
    if (game.end) write = TRUE;
    else locker = FALSE;
    if ((write && !locker)) {
        FILE* score_file = fopen(SCORE_FILE_PATH, "a");
        printf("Appending score.\n");
        fprintf(score_file, "%d\n", game.player.score);
        fclose(score_file);
        write = FALSE;
        if(!locker) locker = TRUE;
    }
}
