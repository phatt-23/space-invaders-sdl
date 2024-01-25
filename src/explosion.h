#ifndef SDL_GAME_EXPLOSION_H
#define SDL_GAME_EXPLOSION_H

#include <SDL2/SDL.h>
#include "modules/dynamic_array.h"
#include "config.h"
#include "alien.h"

typedef struct explosion_class Explosion;
struct explosion_class {
    void (*update)(Explosion* self);
    void (*render)(Explosion* self);
    void (*clean)(Explosion* self);
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    BOOL alive;
    int iteration;          // this will be incremented until the final frame of the explosion-sprite
};

extern DynArray Explosions;

/* Call this function in the MAIN initialisation fn - sdl_context_init()
 * This will init the DynArray.
 * * */
extern void init_Explosions(DynArray* explosions);

/* call this fn when there's an alien that died, pass in the pointer to the alien
 * this will create a new element in the dynamic array Explosions on the heap
 * this new Explosion will have a limited life span, it will die off after some given time
 * we will know it died when the internal flag 'alive' is toggled to FALSE
 * * */
extern void alien_explode(Alien* alien_ptr);

/*
 * This fn works the same way as the alien_explosion for the Alien class
 * but for the player.
 */
extern void player_explode(Player* player);

/*
 * When two projectiles intersect.
 */
extern void proj_explode(Projectile* projectile);

// ****** these fns will be put inside the main update fn in the sdl_context.c file ******

/*  This fn takes care of the EXISTING explosions
 *  It updates their state i.e. move forward in the explosion-sprite
 * */
extern void explosions_update(DynArray* explosions);

/* Frees the memory in the dynamic array Explosions in places of those
 * explosions that have been flagged as 'alive == FALSE'
 * * */
extern void explosions_remove(DynArray* explosions);

// ****** this fn will be put inside the MAIN rendering fn - render()
/* This fn renders all EXISTING explosions
 * that are flagged as 'alive == TRUE'
 * * */
extern void explosions_render(DynArray* explosions);


#endif //SDL_GAME_EXPLOSION_H
