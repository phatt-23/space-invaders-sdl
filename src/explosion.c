//
// Created by phatt on 12/8/23.
//

#include "explosion.h"
#include "config.h"
#include "alien.h"

// fn declarations ------------------------------------------------------------------------------
static void explosion_update(Explosion* self);
static void explosion_render(Explosion* self);
static void explosion_clean(Explosion* self);
// var definitions ------------------------------------------------------------------------------
DynArray Explosions;
// fn definitions -------------------------------------------------------------------------------
extern void init_Explosions(DynArray* explosions) {
    dynarray_init(explosions, sizeof(Explosion));
    //printf("DynArray for Explosions has been initialised!\n");
}
extern void alien_explode(Alien* alien_ptr) {
    Explosion* new_exp = (Explosion*)malloc(sizeof(Explosion));
    new_exp->alive = TRUE;
    new_exp->iteration = 0;
    new_exp->src_rect.x = 0;
    new_exp->src_rect.y = 0;

    int x, y;
    SDL_QueryTexture(game.textures.explosion_texture,
                     NULL, NULL,&x,&y);
    new_exp->src_rect.w = y / 2;
    new_exp->src_rect.h = y / 2;

    new_exp->dst_rect.x = alien_ptr->dst_rect.x - ALIEN_SIZE/2;
    new_exp->dst_rect.y = alien_ptr->dst_rect.y - ALIEN_SIZE/2;
    new_exp->dst_rect.w = alien_ptr->dst_rect.w * 2;
    new_exp->dst_rect.h = alien_ptr->dst_rect.h * 2;
    new_exp->update = &explosion_update;
    new_exp->render = &explosion_render;
    new_exp->clean = &explosion_clean;
    dynarray_push(&Explosions, new_exp);
    //printf("Malloced new_exp in the DynArray Explosions\n");
}
extern void player_explode(Player* player) {
    Explosion* new_exp = (Explosion*)malloc(sizeof(Explosion));
    new_exp->alive = TRUE;
    new_exp->iteration = 0;
    new_exp->src_rect.x = 0;
    new_exp->src_rect.y = 0;

    int x, y;
    SDL_QueryTexture(game.textures.explosion_texture,
                     NULL, NULL,&x,&y);
    new_exp->src_rect.w = y / 2;
    new_exp->src_rect.h = y / 2;

    if(player->lives) {
        new_exp->dst_rect.x = player->dst_rect.x;
        new_exp->dst_rect.y = player->dst_rect.y - (player->dst_rect.h / 3);
        new_exp->dst_rect.w = player->dst_rect.w;
        new_exp->dst_rect.h = player->dst_rect.h;
    } else {
        new_exp->dst_rect.x = player->dst_rect.x - (player->dst_rect.w / 2);
        new_exp->dst_rect.y = player->dst_rect.y - (player->dst_rect.h / 2);
        new_exp->dst_rect.w = player->dst_rect.w * 2;
        new_exp->dst_rect.h = player->dst_rect.h * 2;
    }
    new_exp->update = &explosion_update;
    new_exp->render = &explosion_render;
    new_exp->clean = &explosion_clean;
    dynarray_push(&Explosions, new_exp);
}
extern void proj_explode(Projectile* projectile) {
    static uint8_t scale = 4;
    Explosion* new_exp = (Explosion*)malloc(sizeof(Explosion));
    new_exp->alive = TRUE;
    new_exp->iteration = 0;
    new_exp->src_rect.x = 0;
    new_exp->src_rect.y = 0;

    int x, y;
    SDL_QueryTexture(game.textures.explosion_texture,
                     NULL, NULL,&x,&y);
    new_exp->src_rect.w = y / 2;
    new_exp->src_rect.h = y / 2;

    new_exp->dst_rect.x = projectile->dst_rect.x - (PROJECTILE_WIDTH / scale) * 3;
    new_exp->dst_rect.y = projectile->dst_rect.y - (PROJECTILE_HEIGHT / scale) * 3;
    new_exp->dst_rect.w = projectile->dst_rect.w * scale;
    new_exp->dst_rect.h = projectile->dst_rect.w * scale;
    new_exp->update = &explosion_update;
    new_exp->render = &explosion_render;
    new_exp->clean = &explosion_clean;
    dynarray_push(&Explosions, new_exp);
}
extern void explosions_update(DynArray* explosions) {
    for(int i = 0; i < explosions->size; i++) {
        Explosion* tmp_exp = (Explosion*)explosions->items[i];
        tmp_exp->update(tmp_exp);
    }
}
extern void explosions_render(DynArray* explosions) {
    for(int i = 0; i < explosions->size; i++) {
        Explosion* tmp_exp = (Explosion*)explosions->items[i];
        tmp_exp->render(tmp_exp);
    }
}
extern void explosions_remove(DynArray* explosions) {
    for(int i = 0; i < explosions->size; i++) {
        Explosion* tmp_exp = (Explosion*)explosions->items[i];
        tmp_exp->clean(tmp_exp);
    }
}
static void explosion_update(Explosion* self) {
    int x, y;
    SDL_QueryTexture(game.textures.explosion_texture,
                     NULL, NULL, &x, &y);
    if(self->iteration == 5) {
        self->src_rect.x = 0;
        self->src_rect.y += y/2;
    }
    self->src_rect.x += y/2;
    self->iteration++;
    if(self->iteration > 6)
        self->alive = FALSE;
}
static void explosion_render(Explosion* self) {
    if(self->alive) {
        SDL_RenderCopy(game.renderer,
                       game.textures.explosion_texture,
                       &self->src_rect,
                       &self->dst_rect);
    }
}
static void explosion_clean(Explosion* self) {
    if(self->alive == FALSE) {
        dynarray_remove(&Explosions, self);
        //printf("An Explosion Object has been removed from the DynArray.\n");
    }
}