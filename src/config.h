#ifndef SDL_GAME_MACROS_H
#define SDL_GAME_MACROS_H
/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * GAME SETTINGS - configure the game to your liking *
 * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <stdbool.h>
#define SCALING 1
#define MAX_STRING_LENGTH 100

#define BOOL	_Bool
#define TRUE	1
#define FALSE	0

typedef enum {                      /* * * * Screen size configuration * * * */
    BOTTOM_MARGIN = 50 * SCALING,
    TOP_MARGIN    = 50 * SCALING,
    SIDE_MARGIN   = 20 * SCALING,
    SCREEN_WIDTH  = 500 * SCALING,
    SCREEN_HEIGHT = 500 * SCALING,
} SCREEN_SIZE;

typedef enum {                      /* * * * Alien configuration * * * */
    ALIEN_ROWS    = 5,
    ALIEN_COLUMNS = 10,
    ALIEN_COUNT   = ALIEN_ROWS * ALIEN_COLUMNS,
    ALIEN_SPEED   = 50 * SCALING,
    ALIEN_SIZE    = 24 * SCALING,
    ALIEN_GAP     = 8 * SCALING,      // the gap between the individual aliens
    ALIEN_PROJECTILE_COUNT = 10,      // the # of projectiles on stack for the aliens (default is 10)
    ALIEN_MOVE_DOWN_BUFFER = 10,
} ALIEN;
#define ALIEN_SHOOTING_INTERVAL 0.5

typedef enum {                      /* * * * Player configuration * * * */
    PLAYER_SPEED      = 200 * SCALING,
    PLAYER_HEIGHT     = 24 * SCALING,
    PLAYER_WIDTH      = 48 * SCALING,
    PLAYER_LIVES      = 5,
    PLAYER_GAMEMODE_0 = FALSE,         // allows "cheats" - projectiles don't do any damage
    RESTORE_HEALTH    = FALSE,         // the player will be fully healed after a winning round
} PLAYER;
#define MOVEMENT_BUFFER   0.01
#define SHOOTING_BUFFER   0.3
#define EXIT_AFTER        FALSE        // enable this, if you want to immediately exit after a win or lose

typedef enum {                      /* * * * Projectile configuration * * * */
    PROJECTILE_SPEED  = 300 * SCALING,
    PROJECTILE_WIDTH  = 6 * SCALING,
    PROJECTILE_HEIGHT = 12 * SCALING,
    PLAYER_PROJECTILE_W = 6 * SCALING,
    PLAYER_PROJECTILE_H = 24 * SCALING,
} PROJECTILE;

typedef enum {
  BLOCK_SIZE       = 18 * SCALING,
  BLOCK_DURABILITY = 4,
} BLOCK;

typedef enum {
  CONTAINER_WIDTH = SCREEN_WIDTH / 4,
  BUNKER_WIDTH    = BLOCK_SIZE * 4,
  BUNKER_1_POS    = (CONTAINER_WIDTH - BUNKER_WIDTH) / 2,
} BUNKER;


typedef enum { /* the coords of the "hiding spot" for the alien projectiles */
    OUT_X = -10,
    OUT_Y = -10,
} OUT_OF_BOUNDS_HIDING_SPOT;

/* * * * * * * * * SHOULD NOT BE MODIFIED* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
typedef enum {                                                                              /* */
    TEXT_TOP_MARGIN  = 10 * SCALING,                                                        /* */
    TEXT_SIDE_MARGIN = 10 * SCALING,                                                        /* */
    FONT_SIZE_WIDTH  = 20 * SCALING,                                                        /* */
    FONT_SIZE_HEIGHT = 30 * SCALING,                                                        /* */
} TEXTBOX;                                                                                  /* */

#define FONT_PATH               "assets/CommodorePixeled.ttf"                                   /* */
#define PLAYER_TEXTURE_PATH     "assets/player-ship2.png"                                       /* */
#define PLAYER_PROJECTILE_PATH  "assets/player-projectile.png"
#define ALIEN_PROJECTILE_PATH   "assets/alien-projectile.png"
#define ALIEN_1_PATH            "assets/alien-1.png"                                            /* */
#define ALIEN_2_PATH            "assets/alien-2.png"                                            /* */
#define ALIEN_3_PATH            "assets/alien-3.png"                                            /* */
#define EXPLOSION_PATH          "assets/explosion-sprite.png"                                   /* */
#define BLOCK_TYPE_0_PATH       "assets/block-type-0.png"
#define BLOCK_TYPE_1_PATH       "assets/block-type-1.png"
#define BLOCK_TYPE_2_PATH       "assets/block-type-2.png"
#define BLOCK_TYPE_3_PATH       "assets/block-type-3.png"
#define BLOCK_TYPE_4_PATH       "assets/block-type-4.png"
#define SCORE_FILE_PATH         "score-list.txt"

#define SOUND_PLAYER_HIT        "assets/sound/alien_hit.wav"

#define SOUND_ALIEN_HIT         "assets/sound/player_hit.wav"
#define SOUND_BUNKER_HIT        "assets/sound/bunker_hit.wav"
#define SOUND_PL_PROJ           "assets/sound/projectile_launch.wav"
#define SOUND_AL_PROJ           "assets/sound/alien_projectile.wav"
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */



#endif //SDL_GAME_MACROS_H
