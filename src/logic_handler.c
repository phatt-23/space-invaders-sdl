//
// Created by phatt on 12/6/23.
//

#include "logic_handler.h"

#include "sdl_context.h"
#include "projectile.h"
#include "modules/random_interval.h"
#include "explosion.h"
#include "bunk.h"
// fn declarations --------------------------------------------------------------------

/*
 * Checks for collision between the player's projectiles and the aliens.
 */
static void player_projs_hitting_aliens(Player* player, DynArray* aliens);

/*
 * Checks for collisions between the alien's projectile and the player.
 */
static void alien_projs_hitting_player(Player* player);

/*
 * A random alien will shoot.
 */
static void rand_shooting(DynArray* aliens);

/*
 * Checks whether the player had died (player.lives == 0)
 */
static void check_player(Player* player);

/*
 * Checks whether all the aliens died (aliens.size == 0)
 */
static void check_aliens(DynArray* aliens);

/*
 * Checks if the player's projectiles hit an alien's projectiles and vice versa
 */
static void check_projectiles_collision(Player* player);

/*
 * Checks whether the bunker's blocks were hit by the player or the aliens.
 */
static void check_hitting_bunker(DynArray* bunkers, Projectile* a_projs, Player* player);
// var --------------------------------------------------------------------------------
#define SHOOTING_COOLDOWN_AL 1
double last_shot_time_al = 0.0;

// fn definitions ---------------------------------------------------------------------
extern void handle_logic(void) {
        rand_shooting(&Aliens);
        check_projectiles_collision(&game.player);
        player_projs_hitting_aliens(&game.player, &Aliens);
        alien_projs_hitting_player(&game.player);
        check_hitting_bunker(&Bunkers, alien_projs, &game.player);
        check_aliens(&Aliens);
        check_player(&game.player);
}
static void check_hitting_bunker(DynArray* bunkers, Projectile* a_projs, Player* player) {
  for(int i = 0; i < bunkers->size; i++) {
    Bunker* t_bun = (Bunker*)bunkers->items[i];
    for(int j = 0; j < t_bun->blocks.size; j++) {
      Block* t_block = (Block*)t_bun->blocks.items[j];
      //TODO check if the player has hit the block
      for(int k = 0; k < player->projectiles.size; k++) {
        Projectile* t_player_p = (Projectile*)player->projectiles.items[k];
          if(t_player_p->position.x + PROJECTILE_WIDTH > t_block->dst_rect.x   &&
            t_player_p->position.y + PROJECTILE_HEIGHT > t_block->dst_rect.y    &&
            t_block->dst_rect.x    + BLOCK_SIZE      > t_player_p->position.x &&
            t_block->dst_rect.y    + BLOCK_SIZE      > t_player_p->position.y) {
              t_player_p->removal = TRUE;
              t_block->hit_count += 1;
              proj_explode(t_player_p);
              game.sound_man.play_sound(game.sound_man.sound3);
              //printf("Player hit a block (%d) of Bunker (%d).\n", j, i);
            }
      }
      //TODO check if an alien has hit it
      for(int l = 0; l < ALIEN_PROJECTILE_COUNT; l++) {
          Projectile* t_alien_p = &a_projs[l];
          if(t_alien_p->position.x + PROJECTILE_WIDTH > t_block->dst_rect.x   &&
            t_alien_p->position.y  + PROJECTILE_HEIGHT > t_block->dst_rect.y   &&
            t_block->dst_rect.x    + BLOCK_SIZE      > t_alien_p->position.x &&
            t_block->dst_rect.y    + BLOCK_SIZE      > t_alien_p->position.y) {
              t_alien_p->removal = TRUE;
              t_block->hit_count += 1;
              proj_explode(t_alien_p);
              game.sound_man.play_sound(game.sound_man.sound3);
              //printf("Alien hit a block (%d) of Bunker (%d).\n", j, i);
          }
      }
    }
  }
}
static void player_projs_hitting_aliens(Player* player, DynArray* aliens) {
    for(int i = 0; i < player->projectiles.size; i++) {
        Projectile* playerProj = (Projectile*)player->projectiles.items[i];
        for(int j = 0; j < aliens->size; j++) {
            Alien* alien = (Alien*)aliens->items[j];
            if(playerProj->position.x + PROJECTILE_WIDTH  >  alien->position.x
             &&playerProj->position.y + PROJECTILE_HEIGHT  >  alien->position.y
             &&alien->position.x + ALIEN_SIZE   >  playerProj->position.x
             &&alien->position.y + ALIEN_SIZE   >  playerProj->position.y ) {
                playerProj->removal = TRUE;
                if(alien->alive == TRUE && alien->looted == FALSE) {
                    //printf("The alien (%d) has been hit for %d.\n", j, alien->loot);
                    alien_explode(alien);
                    proj_explode(playerProj);
                    alien->alive = FALSE;
                    alien->looted = TRUE;
                    player->score += alien->loot;
                    game.sound_man.play_sound(game.sound_man.sound2);
                }
             }
        }
    }
}
static void alien_projs_hitting_player(Player* player) {
    for(int i = 0; i < ALIEN_PROJECTILE_COUNT; i++) {
        if(alien_projs[i].position.x + PROJECTILE_WIDTH > player->position.x
         &&alien_projs[i].position.y + PROJECTILE_HEIGHT > player->position.y
         &&player->position.x + PLAYER_WIDTH > alien_projs[i].position.x
         &&player->position.y + PLAYER_HEIGHT > alien_projs[i].position.y ) {
            if(player->lives && !(game.end)) {
                player_explode(&game.player);
                proj_explode(&alien_projs[i]);
                if(!player->gamemode_0) player->lives -= 1;
                alien_projs[i].removal = TRUE;
                game.sound_man.play_sound(game.sound_man.sound1);

                    //printf("Player has been hit by an alien projectile. (%d)\n", i);
                    //printf("Remaining lives: %d\n", player->lives);
            }
        }
    }
}
static void check_projectiles_collision(Player* player) {
    for(int i = 0; i < player->projectiles.size; i++) {
        Projectile* playerProj = (Projectile*)player->projectiles.items[i];
        for(int k = 0; k < ALIEN_PROJECTILE_COUNT; k++) {
            if(alien_projs[k].position.y + PROJECTILE_HEIGHT > playerProj->position.y
               && playerProj->position.y > alien_projs[k].position.y
               && ((alien_projs[k].position.x + PROJECTILE_WIDTH > playerProj->position.x
               && playerProj->position.x > alien_projs[k].position.x)
               || (alien_projs[k].position.x + PROJECTILE_WIDTH > playerProj->position.x + PROJECTILE_WIDTH
               && playerProj->position.x + PROJECTILE_WIDTH > alien_projs[k].position.x))) {
                //printf("Two bullets collided.\n");
                proj_explode(playerProj);
                playerProj->removal = TRUE;
                alien_projs[k].removal = TRUE;
            }
        }
    }
}
static void rand_shooting(DynArray* aliens) {
    static int who_will_shoot = 0;
    if((SDL_GetTicks() - last_shot_time_al) >= (SHOOTING_COOLDOWN_AL * 1000.0) && aliens->size > 0 && !(game.end)) {
        who_will_shoot = rand_interval(1, aliens->size) - 1; // range < 0, ALIEN_COUNT - 1 >
        //printf("WHO WILL SHOOT %d\n", who_will_shoot);
        Alien *tmp_alien = (Alien*)aliens->items[who_will_shoot];
        if (tmp_alien->projectile.removal == TRUE) {
            tmp_alien->shoot(tmp_alien);
        }
        last_shot_time_al = SDL_GetTicks();
    }
}
static void check_player(Player* player) {
    if(!(game.end)) {
        if(player->lives == 0) {
            player_explode(&game.player);
            //printf("You died!\n");
            //printf("You scored %d points this game, good luck next time.\n", game.player.score);
            if(EXIT_AFTER) {
                game.event_handler.quit();
            }
            game.end = TRUE;
        }
    }
}
static void check_aliens(DynArray* aliens) {
    if(!(game.end)) {
        if (aliens->size == 0) {
            //printf("You won!\n");
            //printf("Congratulations, you scored %d points this game.\n", game.player.score);
            if (EXIT_AFTER) {
                game.event_handler.quit();
            }
            game.end = TRUE;
        }
    }
}

