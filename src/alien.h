//
// Created by phatt on 12/6/23.
//

#ifndef SDL_GAME_ALIEN_H
#define SDL_GAME_ALIEN_H

#include "sdl_context.h"
#include "projectile.h"

typedef struct alien_struct Alien;
struct alien_struct {
    /* methods */
    void (*move_right)(Alien* self);
    void (*move_left)(Alien* self);
    void (*move_down)(Alien* self);
    void (*shoot)(Alien* self);
    void (*render)(Alien* self);
    void (*clean)(Alien* self);
    /* attributes */
    int loot;                   // the amount of points the player will get for killing this alien
    BOOL looted;                // has this alien been looted yet? if yes, no more points for the player
    BOOL direction;             // true = move_right(), false = move_left()
    BOOL alive;                 // also acts as a removal flag
    Position position;
    Projectile projectile;      // each alien will have only one projectile (should be enough)
    SDL_Rect dst_rect;
    SDL_Rect src_rect;
    SDL_Texture* texture;
};

/*
 * An array (dynamic) of aliens.
 */
extern DynArray Aliens;

extern Projectile alien_projs[ALIEN_PROJECTILE_COUNT];

/*
 * Alien constructor.
 */
extern Alien* new_alien(void);

/*
 * Initialises the aliens. This function must be called first
 * in order to be able to work with this array.
 * Preferably in the   sdl_context_init()   function.
 */
extern void init_aliens(DynArray* aliens);

/*
 * Updates all the aliens in the dynamic array.
 */
extern void update_aliens(DynArray* aliens);

/*
 * Renders all the allocated aliens.
 */
extern void render_aliens(DynArray* aliens);

/*
 * Frees those aliens who where flagged for removal.
 */
extern void remove_aliens(DynArray* aliens);

#endif //SDL_GAME_ALIEN_H
