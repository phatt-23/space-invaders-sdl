//
// Created by phatt on 12/12/23.
//

#include "sound_manager.h"
#include "config.h"
#include "sdl_context.h"

// fn declarations ----------------------------------------------------------
static void SM_init_audio(void);
static void SM_cleanup_audio(SoundManager* SM);
static void SM_load_audio(SoundManager* SM);
static void SM_pause_audio(void);
static void SM_play_sound(Mix_Chunk* sound);

// constructor ------------------------------------------------------------------------
extern SoundManager new_sound_manager(void) {
    SoundManager new_SM;
    
    new_SM.init_audio = &SM_init_audio;
    new_SM.clean_audio = &SM_cleanup_audio;
    new_SM.load_audio = &SM_load_audio;
    new_SM.play_sound = &SM_play_sound;
    new_SM.pause_audio = &SM_pause_audio;
    new_SM.sound_on = TRUE;

    return new_SM;
}  

// fn definitions ----------------------------------------------------------------------

static void SM_init_audio(void) {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL could not initialize: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "SDL_mixer could not initialize: %s\n", Mix_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}
static void SM_load_audio(SoundManager* SM) {
    SM->sound1 = Mix_LoadWAV(SOUND_PLAYER_HIT);
    SM->sound2 = Mix_LoadWAV(SOUND_ALIEN_HIT);
    SM->sound3 = Mix_LoadWAV(SOUND_BUNKER_HIT);
    SM->sound4 = Mix_LoadWAV(SOUND_PL_PROJ);
    SM->sound5 = Mix_LoadWAV(SOUND_AL_PROJ);
}

static void SM_play_sound(Mix_Chunk* sound) {
    if(game.sound_man.sound_on)
      Mix_PlayChannel(-1, sound, 0);
}

static void SM_pause_audio(void) {
    Mix_Pause(-1);
}

static void SM_cleanup_audio(SoundManager* SM) {
    Mix_FreeChunk(SM->sound1);
    Mix_FreeChunk(SM->sound2);
    Mix_FreeChunk(SM->sound3);
    Mix_FreeChunk(SM->sound4);
    Mix_FreeChunk(SM->sound5);

    Mix_CloseAudio();
}