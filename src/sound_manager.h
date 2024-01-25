//
// Created by phatt on 12/12/23.
//

#ifndef SDL_GAME_SOUND_MANAGER_H
#define SDL_GAME_SOUND_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "config.h"

typedef struct sound_manager SoundManager;
struct sound_manager {
    Mix_Chunk* sound1;
    Mix_Chunk* sound2;
    Mix_Chunk* sound3;
    Mix_Chunk* sound4;
    Mix_Chunk* sound5;

    BOOL sound_on;

    void (*init_audio)(void);
    void (*load_audio)(SoundManager* self);
    void (*play_sound)(Mix_Chunk* sound);
    void (*pause_audio)(void);
    void (*clean_audio)(SoundManager* self);
};


extern SoundManager new_sound_manager(void);  

#endif // SDL_GAME_SOUND_MANAGER_H
