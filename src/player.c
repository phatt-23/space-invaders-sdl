//
// Created by phatt on 12/6/23.
//

#include "player.h"

#include <stdlib.h>
#include "sdl_context.h"
#include "config.h"
#include "projectile.h"



// fn declarations -----------------------------------------------------------------------------------------------------

static void player_move_right(Player* player);
static void player_move_left(Player* player);
static void player_shoot(Player* player);
static void player_update(Player* player);
static void player_render(void);
static void player_clean(Player* player);

static BOOL gamemode_check(int argc, char** argv);

// fn definitions ------------------------------------------------------------------------------------------------------

Player new_player() {
    Player newPlayer;
    newPlayer.lives = PLAYER_LIVES;
    newPlayer.score = 0;
    newPlayer.position.x = (SCREEN_WIDTH/2) - (PLAYER_WIDTH/2);
    newPlayer.position.y = SCREEN_HEIGHT - BOTTOM_MARGIN;
    newPlayer.move_right = &player_move_right;
    newPlayer.move_left = &player_move_left;
    newPlayer.shoot = &player_shoot;
    newPlayer.update = &player_update;
    newPlayer.render = &player_render;
    newPlayer.clean = &player_clean;
    newPlayer.gamemode_0 = gamemode_check(game.arg_count, game.arguments);
    newPlayer.dst_rect.x = newPlayer.position.x;
    newPlayer.dst_rect.y = newPlayer.position.y;
    newPlayer.dst_rect.w = PLAYER_WIDTH;
    newPlayer.dst_rect.h = PLAYER_HEIGHT;
    dynarray_init( &newPlayer.projectiles, sizeof(Projectile) );
    return newPlayer;
}
void player_move_right(Player* player) {
    if(player->position.x < SCREEN_WIDTH - (SIDE_MARGIN + PLAYER_WIDTH)
    && (player->lives || player->gamemode_0) )
        player->position.x += PLAYER_SPEED * game.fps.delta;
}
void player_move_left(Player* player) {
    if(player->position.x > SIDE_MARGIN
    && (player->lives || player->gamemode_0) )
        player->position.x -= PLAYER_SPEED * game.fps.delta;
}
void player_shoot(Player* player) {
    if(player->lives || player->gamemode_0) {  // locking shooting when you die
        Projectile *new_proj = (Projectile *) malloc(sizeof(Projectile));
        *new_proj = new_projectile(TRUE);
        new_proj->position.x = player->position.x + (PLAYER_WIDTH/2) - (PLAYER_PROJECTILE_W/2);
        new_proj->position.y = player->position.y;
        dynarray_push(&game.player.projectiles, new_proj);
        game.sound_man.play_sound(game.sound_man.sound4);
    }
}
void player_update(Player* player) {
    player->dst_rect.x = player->position.x;
    player->dst_rect.y = player->position.y;
}
void player_render(void) {
    if(game.player.lives)
        SDL_RenderCopy(game.renderer,
                       game.textures.player_texture,
                       NULL,
                       &game.player.dst_rect);
}
void player_clean(Player* player) {
    dynarray_free(&player->projectiles);
}
static BOOL gamemode_check(int argc, char** argv) {
    BOOL gamemode0 = FALSE;
    for(int i = 1; i < argc; i++) {
        printf("Command Line Argument(%d): %s\n", i, argv[i]);
        if(strcmp(argv[i], "-g") == 0) {
            gamemode0 = TRUE;
            printf("Cheats enabled!\n");
            break;
        }
    }
    return gamemode0;
}