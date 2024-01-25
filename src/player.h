//
// Created by phatt on 12/6/23.
//

#ifndef SDL_GAME_GAME_OBJECTS_H
#define SDL_GAME_GAME_OBJECTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "modules/dynamic_array.h"
#include "config.h"

typedef struct {
    float x;
    float y;
} Position;

typedef struct player_struct Player;
struct player_struct {
    /* methods */
    void (*move_right)(Player* self);
    void (*move_left)(Player* self);
    void (*shoot)(Player* self);
    void (*update)(Player* self);
    void (*render)(void);
    void (*clean)(Player* self);
    /* attributes */
    Position position;
    int lives;
    int score;
    BOOL gamemode_0;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;
    DynArray projectiles;
};

extern Player new_player();


#endif //SDL_GAME_GAME_OBJECTS_H
