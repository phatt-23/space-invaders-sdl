//
// Created by phatt on 12/6/23.
//

#include "alien.h"
#include "config.h"
#include "projectile.h"

// fn declarations --------------------------------------------------------------------------

static void alien_move_right(Alien* alien);
static void alien_move_left(Alien* alien);
static void alien_move_down(Alien* alien);
static void alien_shoot(Alien* alien);
static void alien_render(Alien* alien);
static void alien_clean(Alien* alien);
static void alien_projs_update(void);
static void alien_objects_update(DynArray* aliens);

extern void update_aliens(DynArray* aliens);
extern void render_aliens(DynArray* aliens);
extern void remove_aliens(DynArray* aliens);
extern void init_aliens(DynArray* aliens);

// variables definition ---------------------------------------------------------------------
DynArray Aliens;
Projectile alien_projs[ALIEN_PROJECTILE_COUNT];     // allocated on stack, no freeing required
static int now_using = 0;
// fn definition ----------------------------------------------------------------------------
extern Alien* new_alien(void) {
    Alien* new_alien = (Alien*)malloc(sizeof(Alien));
    new_alien->position.x = 0;
    new_alien->position.y = TOP_MARGIN;
    new_alien->dst_rect.x = new_alien->position.x;
    new_alien->dst_rect.y = new_alien->position.y;
    new_alien->dst_rect.w = ALIEN_SIZE;
    new_alien->dst_rect.h = ALIEN_SIZE;
    new_alien->src_rect.x = 0;
    new_alien->src_rect.y = 0;
    new_alien->src_rect.w = 0;
    new_alien->src_rect.h = 0;
    new_alien->alive = TRUE;
    new_alien->direction = TRUE;
    new_alien->move_right = &alien_move_right;
    new_alien->move_left = &alien_move_left;
    new_alien->move_down = &alien_move_down;
    new_alien->shoot = &alien_shoot;
    new_alien->render = &alien_render;
    new_alien->clean = &alien_clean;
    new_alien->projectile.position.x = OUT_X;     // out of bounds
    new_alien->projectile.position.y = OUT_Y;
    new_alien->projectile.removal = TRUE;         // here the removal will take on the role of hiding in the OUT_BOUNDS place
    return new_alien;
}
static void alien_render(Alien* alien) {
    SDL_RenderCopy(game.renderer, alien->texture, &alien->src_rect, &alien->dst_rect);
    for(int i = 0; i < ALIEN_PROJECTILE_COUNT; i++) {
        if(alien_projs[i].removal == FALSE)
            SDL_RenderCopyEx( game.renderer,
                              alien_projs[i].texture,
                               &alien_projs[i].src_rect,
                              &alien_projs[i].dst_rect,
                              180,
                              NULL,
                              SDL_FLIP_NONE );
    }
}
static void alien_move_right(Alien* alien) {
    alien->position.x += ALIEN_SPEED * game.fps.delta;
}
static void alien_move_left(Alien* alien) {
  alien->position.x -= ALIEN_SPEED * game.fps.delta;
}
static void alien_move_down(Alien* alien) {
    alien->position.y += ALIEN_SIZE;
}
static void alien_clean(Alien* alien) {
    dynarray_remove(&Aliens ,alien);
}
static void alien_shoot(Alien* alien) {
    alien->projectile.removal = FALSE;  // now won't be forced into the OUT_BOUNDS place
    alien->projectile.position.x = alien->position.x + (ALIEN_SIZE/2) - (PROJECTILE_WIDTH/2);
    alien->projectile.position.y = alien->position.y + ALIEN_SIZE;
    alien->projectile.dst_rect.x = alien->projectile.position.x;
    alien->projectile.dst_rect.y = alien->projectile.position.y;
    alien->projectile.dst_rect.w = PROJECTILE_WIDTH;
    alien->projectile.dst_rect.h = PROJECTILE_HEIGHT;

    alien_projs[now_using].removal = FALSE;
    alien_projs[now_using].position.x = alien->projectile.position.x;
    alien_projs[now_using].position.y = alien->projectile.position.y;
    alien_projs[now_using].dst_rect.x = alien->projectile.dst_rect.x;
    alien_projs[now_using].dst_rect.y = alien->projectile.dst_rect.y;
    alien_projs[now_using].dst_rect.w = PROJECTILE_WIDTH;
    alien_projs[now_using].dst_rect.h = PROJECTILE_HEIGHT;

    now_using++;
    if(now_using >= ALIEN_PROJECTILE_COUNT - 1) now_using = 0.0;
    game.sound_man.play_sound(game.sound_man.sound5);
}
extern void update_aliens(DynArray* aliens) {
  static double since_last = 0.0;
  static const double buffer = 1.0;
  since_last += game.fps.delta;
  if(game.end == FALSE) {
    if(game.player.lives) {
      alien_projs_update();
    }
    alien_objects_update(aliens);
    if(since_last > buffer) {
      for (int i = 0; i < aliens->size; i++) {
        Alien *t_alien = (Alien *)aliens->items[i];
        if (t_alien->src_rect.x)
          t_alien->src_rect.x -= t_alien->src_rect.w;
        else
          t_alien->src_rect.x += t_alien->src_rect.w;
      }
      since_last = 0.0;
    }
  }
}
extern void render_aliens(DynArray* aliens) {
    for(int i = 0; i < aliens->size; i++) {
        Alien* temp_alien = (Alien*)aliens->items[i];
        temp_alien->render(temp_alien);
    }
}
extern void remove_aliens(DynArray* aliens) {
    for(int i = 0; i < aliens->size; i++) {
        Alien* temp_alien = (Alien*)aliens->items[i];
        if(temp_alien->alive == FALSE)
            temp_alien->clean(temp_alien);
    }
}
extern void init_aliens(DynArray* aliens) {
    int initial_position_x = SIDE_MARGIN;
    int initial_position_y = TOP_MARGIN;
    dynarray_init(aliens, sizeof(Alien) * ALIEN_COUNT);
    for(int i = 0; i < ALIEN_ROWS; i++) {
        for(int j = 0; j < ALIEN_COLUMNS; j++) {
            Alien* temp_alien = (Alien*)aliens->items[i + j];
            temp_alien = new_alien();
            temp_alien->position.x = initial_position_x;
            temp_alien->position.y = initial_position_y;
            temp_alien->looted = FALSE;
            if(i == 0) {
              temp_alien->loot = 150;
              temp_alien->texture = game.textures.alien_3_texture;
              SDL_QueryTexture(game.textures.alien_3_texture,
                               NULL, NULL,
                               &temp_alien->src_rect.w,
                               &temp_alien->src_rect.h);
            } else if(i > 0 && i <= ALIEN_ROWS / 2) {
              temp_alien->loot = 100;
              temp_alien->texture = game.textures.alien_1_texture;
              SDL_QueryTexture(game.textures.alien_1_texture,
                               NULL, NULL,
                               &temp_alien->src_rect.w,
                               &temp_alien->src_rect.h);
            } else if(i > ALIEN_ROWS / 2) {
              temp_alien->loot = 50;
              temp_alien->texture = game.textures.alien_2_texture;
              SDL_QueryTexture(game.textures.alien_2_texture,
                               NULL, NULL,
                               &temp_alien->src_rect.w,
                               &temp_alien->src_rect.h);
            }
            temp_alien->src_rect.w /= 2;
            initial_position_x += ALIEN_SIZE + ALIEN_GAP;
            dynarray_push(aliens, temp_alien);
        }
        initial_position_x = SIDE_MARGIN;
        initial_position_y += ALIEN_SIZE + ALIEN_GAP;
    }
    for(int i = 0; i < ALIEN_PROJECTILE_COUNT; i++) {
        alien_projs[i] = new_projectile(FALSE);
        alien_projs[i].position.x = OUT_X;
        alien_projs[i].position.y = OUT_Y;
        alien_projs[i].dst_rect.x = alien_projs[i].position.x;
        alien_projs[i].dst_rect.y = alien_projs[i].position.y;
        alien_projs[i].removal = TRUE;
    }
}
static void alien_projs_update(void) {
    static const double buffer = 0.3;
    static double since_last = 0.0;
  since_last += game.fps.delta;
  for(int i = 0; i < ALIEN_PROJECTILE_COUNT; i++) {
      if(since_last > buffer) {
        if(alien_projs[i].src_rect.x > 0)
            alien_projs[i].src_rect.x -= alien_projs[i].src_rect.w;
          else
            alien_projs[i].src_rect.x += alien_projs[i].src_rect.w;
      }
    if(alien_projs[i].position.y > SCREEN_HEIGHT) {
      alien_projs[i].removal = TRUE;
    }
    if(alien_projs[i].removal == FALSE) {
      alien_projs[i].position.y += alien_projs[i].speed * game.fps.delta;
      alien_projs[i].dst_rect.x = alien_projs[i].position.x;
      alien_projs[i].dst_rect.y = alien_projs[i].position.y;
    } else if(alien_projs[i].removal == TRUE) {
      alien_projs[i].position.x = OUT_X;
      alien_projs[i].position.y = OUT_Y;
    }
    if(since_last > buffer && i == ALIEN_PROJECTILE_COUNT - 1)
      since_last = 0.0;
  }
}
static void alien_objects_update(DynArray* aliens) {
  static double since_last = 0.0;
  static BOOL bool_down = FALSE;
  static BOOL wall_hit = FALSE;
  since_last += game.fps.delta;
  //printf("Since last: %lf\n", since_last);
  if(since_last >= ALIEN_MOVE_DOWN_BUFFER) {
    bool_down = TRUE;
  }
  for(int j = 0; j < aliens->size; j++) {
      Alien* temp_alien = (Alien*)aliens->items[j];
      if(temp_alien->position.x >= SCREEN_WIDTH - SIDE_MARGIN - ALIEN_SIZE) {
          for(int i = 0; i < aliens->size; i++) {
              Alien* tmp_alien = (Alien*)aliens->items[i];
              tmp_alien->direction = FALSE;
          }
          wall_hit = TRUE;
      }
      if(temp_alien->position.x <= SIDE_MARGIN) {
          for(int i = 0; i < aliens->size; i++) {
              Alien* tmp_alien = (Alien*)aliens->items[i];
              tmp_alien->direction = TRUE;
          }
          wall_hit = TRUE;
      }
      if(temp_alien->projectile.position.y > SCREEN_HEIGHT) {
          temp_alien->projectile.removal = TRUE;
      }
      if(temp_alien->position.y + ALIEN_SIZE > SCREEN_HEIGHT - (SCREEN_HEIGHT / 4) - ALIEN_SIZE) {
          bool_down = FALSE;
      }
    }
    for(int i = 0; i < aliens->size; i++) {
        Alien* tmp_alien = (Alien*)aliens->items[i];
        if(tmp_alien->direction) tmp_alien->move_right(tmp_alien);
        else tmp_alien->move_left(tmp_alien);
        if(bool_down && wall_hit)
          tmp_alien->move_down(tmp_alien);
        tmp_alien->dst_rect.x = tmp_alien->position.x;
        tmp_alien->dst_rect.y = tmp_alien->position.y;
        if(tmp_alien->projectile.removal) {
            tmp_alien->projectile.position.x = OUT_X;
            tmp_alien->projectile.position.y = OUT_Y;
        } else {
            tmp_alien->projectile.position.y += PROJECTILE_SPEED;
            tmp_alien->projectile.dst_rect.x = tmp_alien->projectile.position.x;
            tmp_alien->projectile.dst_rect.y = tmp_alien->projectile.position.y;
        }
    }
    if(bool_down && wall_hit) {
      //printf("Bool down: %d   Bool wall hit: %d\n", bool_down, wall_hit);
      bool_down = FALSE;
      since_last = 0.0;
    }
  wall_hit = FALSE;

}