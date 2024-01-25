//
// Created by phatt on 12/8/23.
//

#ifndef SDL_GAME_BUNK_H
#define SDL_GAME_BUNK_H

#include "player.h"

typedef struct block_struct Block;
typedef struct bunker_struct Bunker;

struct block_struct {
  void (*update)(Block* self);
  void (*render)(Block* self);
  SDL_Rect src_rect;
  SDL_Rect dst_rect;
  BOOL alive;           // if FALSE then ready for removal
  int hit_count;        // if reaches 4 then 'alive' = FALSE
  uint8_t type;         // 0 -NORMAL ; 1 -BOTTOM_RIGHT ; 2 -BOTTOM_LEFT ; 3 -TOP_RIGHT ; 4 -TOP_LEFT
};

struct bunker_struct {
  void (*clean_blocks)(DynArray* blocks);
  void (*remove_blocks)(DynArray* blocks);
  BOOL destroyed;
  DynArray blocks;
  Position position;    // the position of the bunker type 1
};

extern DynArray Bunkers;
/*
 *     1 0 0 2
 *     0 3 4 0
 *     0     0
 *
 *    This is how one bunker looks like
 */


/*
 * This function will initialise all four bunkers.
 */
extern void init_bunkers(DynArray* bunkers);

/*
 * This function updates all bunkers
 */
extern void update_bunkers(DynArray* bunkers);

/*
 * This function renders all the bunkers on the screen.
 */
extern void render_bunkers(DynArray* bunkers);

/*
 * This function will remove (free) the DynArray of Bunkers
 */
extern void remove_bunkers(DynArray* bunkers, BOOL leaving);




#endif //SDL_GAME_BUNK_H
