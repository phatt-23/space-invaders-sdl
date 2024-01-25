//
// Created by phatt on 12/6/23.
//

#ifndef SDL_GAME_PROJECTILE_H
#define SDL_GAME_PROJECTILE_H

#include "player.h"
#include "config.h"
#include "SDL2/SDL.h"

typedef struct projectile_struct Projectile;

struct projectile_struct {
    void (*update)(Projectile* self);
    void (*render)(Projectile* self);
    /* variables */
    Position position;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    SDL_Texture* texture;
    int speed;
    BOOL removal;
    /* methods */
};

/*
 * The projectile "constructor"
 */
extern Projectile new_projectile(BOOL player);

/*
 * Takes the dynamic array of projectiles and updates them all.
 */
extern void update_projectiles(DynArray* projectiles);

/*
 * Removes the flagged projectiles (removal = true)
 */
extern void remove_projectiles(DynArray* projectiles);

/*
 * Takes the dynamic array of the projectiles and renders them all.
 */
extern void render_projectiles(DynArray* projectiles);

#endif //SDL_GAME_PROJECTILE_H
