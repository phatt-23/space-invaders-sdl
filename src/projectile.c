//
// Created by phatt on 12/6/23.
//

#include "projectile.h"
#include "sdl_context.h"

// fn declarations -----------------------------------------------------------------------------------------------------

static void projectile_update(Projectile* proj);
static void projectile_render(Projectile* proj);
/*
 * Takes the dynamic array of projectiles and updates them all.
 */

/*
 * Takes the dynamic array of the projectiles and renders them all.
 *//*

static void update_projectiles(DynArray* projectiles);

*/
/*
 * Takes the dynamic array of the projectiles and renders them all.
 *//*

static void render_projectiles(DynArray* projectiles);
*/
// fn definitions ------------------------------------------------------------------------------------------------------

extern Projectile new_projectile(BOOL player) {
    Projectile new_proj;
    new_proj.position.x = 0;
    new_proj.position.y = 0;
    new_proj.speed = PROJECTILE_SPEED;
    new_proj.update = &projectile_update;
    new_proj.render = &projectile_render;
    new_proj.removal = FALSE;
    new_proj.dst_rect.x = 0;
    new_proj.dst_rect.y = 0;
    new_proj.src_rect.x = 0;
    new_proj.src_rect.y = 0;
  if(player) {
    new_proj.dst_rect.w = PLAYER_PROJECTILE_W;
    new_proj.dst_rect.h = PLAYER_PROJECTILE_H;
    SDL_QueryTexture(game.textures.player_projectile_texture,
                     NULL, NULL,
                     &new_proj.src_rect.w, &new_proj.src_rect.h);
    new_proj.src_rect.w /= 2;
    new_proj.texture = game.textures.player_projectile_texture;
  } else {
    new_proj.dst_rect.w = PROJECTILE_WIDTH;
    new_proj.dst_rect.h = PROJECTILE_HEIGHT;
    SDL_QueryTexture(game.textures.alien_projectile_texture,
                     NULL, NULL,
                     &new_proj.src_rect.w,
                     &new_proj.src_rect.h);
    new_proj.src_rect.w /= 2;
    new_proj.texture = game.textures.alien_projectile_texture;
  }
    return new_proj;
}

static void projectile_update(Projectile* proj) {
    proj->position.y -= proj->speed * game.fps.delta;
    if(proj->position.y < 0 - PROJECTILE_HEIGHT || proj->position.y > SCREEN_HEIGHT) {
        proj->removal = TRUE;
        //printf("Projectile ready for removal.\n");
    }
}
static void projectile_render(Projectile* proj) {
    proj->dst_rect.x = proj->position.x;
    proj->dst_rect.y = proj->position.y;
    //printf("Projectile .x = %d, .y = %d\n", proj->position.x, proj->position.y);
    //printf("\t\t dst_rect .x = %d, .y = %d\n", proj->dst_rect.x, proj->dst_rect.y);
    SDL_RenderCopy(game.renderer, proj->texture, &proj->src_rect, &proj->dst_rect);
}
extern void update_projectiles(DynArray* projectiles) {
    static double since_last = 0.0;
    static const double buffer = 0.1;
    since_last += game.fps.delta;
    int iter = projectiles->size;
    for(int i = 0; i < iter; i++) {
        Projectile* temp_proj = (Projectile*)projectiles->items[i];
        if(!game.end || game.player.lives)
            temp_proj->update(temp_proj);
        if(since_last > buffer) {
          if(temp_proj->src_rect.x)
            temp_proj->src_rect.x -= temp_proj->src_rect.w;
          else
            temp_proj->src_rect.x += temp_proj->src_rect.w;
        }
      if(since_last > buffer && i == iter - 1)
          since_last = 0.0;
    }
}
extern void render_projectiles(DynArray* projectiles) {
    for(int i = 0; i < projectiles->size; i++) {
        Projectile* temp_projectile = (Projectile*)projectiles->items[i];
        temp_projectile->render(temp_projectile);
    }
}
extern void remove_projectiles(DynArray* projectiles) {
    for(int i = 0; i < projectiles->size; i++) {
        Projectile* temp_proj = (Projectile*)projectiles->items[i];
        if(temp_proj->removal) {
            dynarray_remove(projectiles, temp_proj);
            //printf("Projectile removed.\n");
        }
    }
}
