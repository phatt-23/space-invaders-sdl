//
// Created by phatt on 12/8/23.
//

#include "bunk.h"
#include "sdl_context.h"

// var definitions -------------------------------------------------------------

DynArray Bunkers;

// fn declarations -------------------------------------------------------------

static Block new_block(void);
static void block_update(Block* block);
static void block_render(Block * block);
static void bunker_clean_blocks(DynArray* blocks);
static Bunker* new_bunker(int x_block);


// fn definitions --------------------------------------------------------------

static Block new_block(void) {
  Block new_b;
  new_b.update = &block_update;
  new_b.render = &block_render;
  new_b.src_rect.x = 0;
  new_b.src_rect.y = 0;
  int x, y;
  SDL_QueryTexture(game.textures.block_type_0,
                   NULL, NULL, &x, &y);
  new_b.src_rect.w = y;
  new_b.src_rect.h = y;
  new_b.dst_rect.x = 0;
  new_b.dst_rect.y = 0;
  new_b.dst_rect.w = BLOCK_SIZE;
  new_b.dst_rect.h = BLOCK_SIZE;
  new_b.type = 0;
  new_b.alive = TRUE;
  new_b.hit_count = 0;
  return new_b;
}

static Bunker* new_bunker(int x_block) {
  static const int y_block = SCREEN_HEIGHT - (SCREEN_HEIGHT / 4);
  Bunker* new_bun = (Bunker*)malloc(sizeof(Bunker));
  new_bun->position.x = x_block;
  new_bun->position.y = SCREEN_HEIGHT / 3;
  new_bun->destroyed = FALSE;
  new_bun->clean_blocks = &bunker_clean_blocks;
  dynarray_init(&new_bun->blocks, sizeof(Block));
  for(int i = 0; i < 10; i++) {
    Block* n_block = (Block*)malloc(sizeof(Block));
    *n_block = new_block();
    if(i < 4) {
      n_block->dst_rect.x = x_block + (i * BLOCK_SIZE);
      n_block->dst_rect.y = y_block;
      if(i == 0) n_block->type = 1;
      else if(i == 3) n_block->type = 2;
    } else if(i < 8) {
      n_block->dst_rect.x = x_block + ((i - 4) * BLOCK_SIZE);
      n_block->dst_rect.y = y_block + BLOCK_SIZE;
      if(i == 5) n_block->type = 3;
      else if(i == 6) n_block->type = 4;
    } else {
      n_block->dst_rect.x = x_block;
      n_block->dst_rect.y = y_block + (2 * BLOCK_SIZE);
      if(i == 9) n_block->dst_rect.x = x_block + (3 * BLOCK_SIZE);
    }
    dynarray_push(&new_bun->blocks, n_block);
    //printf("New Block (%d) was allocated\n", i);
  }
  //printf("New Bunker pointer with blocks was created.\n");
  return new_bun;
}
extern void init_bunkers(DynArray* bunkers) {
  //printf("Beginning initialising four Bunkers.\n");
  dynarray_init(bunkers, sizeof(Bunker));
  int x_pos = BUNKER_1_POS;
  for(int i = 0; i < 4; i++) {
    //printf("Initialisation of Bunker no. %d started.\n", i);
    Bunker* t_bunker_ptr = new_bunker(x_pos);
    dynarray_push(bunkers, t_bunker_ptr);
    //printf("Bunker stats: .blocks.size = %d, .position.x = %d\n", t_bunker_ptr->blocks.size, t_bunker_ptr->position.x);
    x_pos += CONTAINER_WIDTH;
    //printf("Initialisation of Bunker no. %d finished.\n", i);
  }
}
extern void remove_bunkers(DynArray* bunkers, BOOL leaving) {
  //printf("Removing Bunkers started.\n");
  //printf("Amount of Bunkers: %d\n", bunkers->size);
  const int iterate = bunkers->size;
  for(int i = 0; i < iterate; i++) {
    Bunker* t_bun = (Bunker*)bunkers->items[i];
    if(leaving) {
      //printf("Now removing Bunker no. %d\n", i);
      // printf("Stats: blocks.size = %d\n", t_bun->blocks.size);
      for(int j = 0; j < t_bun->blocks.size; j++) {
        Block* t_block = (Block*)t_bun->blocks.items[j];
        t_block->alive = FALSE;
        //printf("Block no. %d of Bunker no. %d was set for removal\n", j, i);
      }
      //printf("Bunker no. %d was removed from memory.\n", i);
    }
    t_bun->clean_blocks(&t_bun->blocks);
    if(leaving) dynarray_free(&t_bun->blocks);
  }
  if(leaving) {
    dynarray_free(bunkers);
    //printf("Finished removing Bunkers.\n");
  }
}
extern void update_bunkers(DynArray* bunkers) {
  for(int i = 0; i < bunkers->size; i++) {
    Bunker* t_bunker = (Bunker*)bunkers->items[i];
    for(int j = 0; j < t_bunker->blocks.size; j++) {
      Block* t_block = (Block*) t_bunker->blocks.items[j];
      t_block->update(t_block);
    }
    if(t_bunker->blocks.size == 0)
        t_bunker->destroyed = TRUE;
  }
}
extern void render_bunkers(DynArray* bunkers) {
  for(int i = 0; i < bunkers->size; i++) {
    Bunker* t_bunker = (Bunker*)bunkers->items[i];
    for(int j = 0; j < t_bunker->blocks.size; j++) {
      Block* t_block = (Block*) t_bunker->blocks.items[j];
      t_block->render(t_block);
    }
  }
}
static void block_update(Block* block) {
  if(block->hit_count == BLOCK_DURABILITY) {
    block->alive = FALSE;
  }
}
static void block_render(Block* block) {
  static SDL_Rect src_ren;
  static SDL_Texture* ren_txtr;
  src_ren.x = block->src_rect.x;
  src_ren.y = block->src_rect.y;
  src_ren.w = block->src_rect.w;
  src_ren.h = block->src_rect.h;
  for(int i = 0; i < block->hit_count; i++) {
    src_ren.x += block->src_rect.w;
  }
  if(block->type == 0) ren_txtr = game.textures.block_type_0;
  if(block->type == 1) ren_txtr = game.textures.block_type_1;
  if(block->type == 2) ren_txtr = game.textures.block_type_2;
  if(block->type == 3) ren_txtr = game.textures.block_type_3;
  if(block->type == 4) ren_txtr = game.textures.block_type_4;
  SDL_RenderCopy(game.renderer,
                 ren_txtr,
                 &src_ren,
                 &block->dst_rect);
}
static void bunker_clean_blocks(DynArray* blocks) {
  const int iterate = blocks->size;
  for(int i = 0; i < iterate; i++) {
    Block* t_block = (Block*) blocks->items[i];
    if(t_block->alive == FALSE) {
      dynarray_remove(blocks, t_block);
      //printf("Block (%d) was removed from DynArray blocks\n", i);
    }
  }
}


