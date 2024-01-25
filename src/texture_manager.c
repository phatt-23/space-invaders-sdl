//
// Created by phatt on 12/6/23.
//
#include "texture_manager.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sdl_context.h"
#include "config.h"
// preprocessor ----------------------------------------------------------------------------------------------

static BOOL lock = FALSE;

// fn declarations -------------------------------------------------------------------------------------------
static SDL_Texture* load_texture(const char* filepath, SDL_Renderer* renderer);
static void textures_destroy(TextureManager* textures);
static void load_text(Text* text, char* string);
static void text_render(Text* text);
static Text new_text(TTF_Font* font);
// fn implementation -----------------------------------------------------------------------------------------
TextureManager new_texture_manager(SDL_Renderer* renderer) {
    if(TTF_Init() == -1) {
      fprintf(stderr,"Couldn't initialise the SDL2_ttf\nError: %s\n", TTF_GetError());
      SDL_Quit();exit(1);
    }
    TextureManager new_game_textures;
    new_game_textures.player_texture = load_texture(PLAYER_TEXTURE_PATH, renderer);
    new_game_textures.player_projectile_texture = load_texture(PLAYER_PROJECTILE_PATH, renderer);
    new_game_textures.alien_projectile_texture = load_texture(ALIEN_PROJECTILE_PATH, renderer);
    new_game_textures.alien_1_texture = load_texture(ALIEN_1_PATH, renderer);
    new_game_textures.alien_2_texture = load_texture(ALIEN_2_PATH, renderer);
    new_game_textures.alien_3_texture = load_texture(ALIEN_3_PATH, renderer);
    new_game_textures.explosion_texture = load_texture(EXPLOSION_PATH, renderer);

    new_game_textures.block_type_0 = load_texture(BLOCK_TYPE_0_PATH, renderer);
    new_game_textures.block_type_1 = load_texture(BLOCK_TYPE_1_PATH, renderer);
    new_game_textures.block_type_2 = load_texture(BLOCK_TYPE_2_PATH, renderer);
    new_game_textures.block_type_3 = load_texture(BLOCK_TYPE_3_PATH, renderer);
    new_game_textures.block_type_4 = load_texture(BLOCK_TYPE_4_PATH, renderer);

    new_game_textures.font = TTF_OpenFont(FONT_PATH, 32);

    new_game_textures.destroy = &textures_destroy;
    new_game_textures.title_1 = new_text(new_game_textures.font);
    new_game_textures.title_2 = new_text(new_game_textures.font);
    new_game_textures.highest_score = new_text(new_game_textures.font);
    new_game_textures.score = new_text(new_game_textures.font);
    new_game_textures.lives = new_text(new_game_textures.font);
    new_game_textures.prompt = new_text(new_game_textures.font);
    new_game_textures.you_won = new_text(new_game_textures.font);
    new_game_textures.you_lost = new_text(new_game_textures.font);
    new_game_textures.press_r = new_text(new_game_textures.font);
    new_game_textures.paused = new_text(new_game_textures.font);
    new_game_textures.press_q = new_text(new_game_textures.font);
    new_game_textures.press_esc = new_text(new_game_textures.font);
    new_game_textures.press_m = new_text(new_game_textures.font);
    return new_game_textures;
}
extern char* format_press_m(void) {
    static char string[MAX_STRING_LENGTH];
    if(game.sound_man.sound_on) {
      strcpy(string, "PRESS M TO MUTE"); lock = TRUE;
    } else {
      strcpy(string, "PRESS M TO UNMUTE"); lock = TRUE;
    }
    game.textures.press_m.dst_rect.w = (FONT_SIZE_WIDTH * strlen(string));
    game.textures.press_m.dst_rect.h = FONT_SIZE_HEIGHT;
    game.textures.press_m.dst_rect.x = (SCREEN_WIDTH / 2) - ((FONT_SIZE_WIDTH * strlen(string)) / 2);
    game.textures.press_m.dst_rect.y = (SCREEN_HEIGHT/ 2) + FONT_SIZE_HEIGHT * 2;
    return string;
}
extern char* format_title_1(void) {
    static int scale = 3;
    static char string[MAX_STRING_LENGTH] = "SPACE";
    SDL_Rect* tmp_rect = &game.textures.title_1.dst_rect;
    tmp_rect->w = (FONT_SIZE_WIDTH * scale * strlen(string));
    tmp_rect->h = FONT_SIZE_HEIGHT * scale;
    tmp_rect->x = (SCREEN_WIDTH / 2) - ((FONT_SIZE_WIDTH * scale * strlen(string)) / 2);
    tmp_rect->y = (SCREEN_HEIGHT / 2) - FONT_SIZE_HEIGHT * 2 * scale;
    return string;
}
extern char* format_title_2(void) {
    static int scale = 3;
    static char string[MAX_STRING_LENGTH] = "INVADERS";
    SDL_Rect* tmp_rect = &game.textures.title_2.dst_rect;
    tmp_rect->w = (FONT_SIZE_WIDTH * scale * strlen(string));
    tmp_rect->h = FONT_SIZE_HEIGHT * scale;
    tmp_rect->x = (SCREEN_WIDTH / 2) - ((FONT_SIZE_WIDTH * scale * strlen(string)) / 2);
    tmp_rect->y = (SCREEN_HEIGHT / 2) - FONT_SIZE_HEIGHT * scale;
    return string;
}
extern char* format_prompt(void) {
    static char string[MAX_STRING_LENGTH] = "PRESS ANY KEY TO PLAY";
    game.textures.prompt.dst_rect.w = (FONT_SIZE_WIDTH * strlen(string));
    game.textures.prompt.dst_rect.h = FONT_SIZE_HEIGHT;
    game.textures.prompt.dst_rect.x = (SCREEN_WIDTH / 2) - ((FONT_SIZE_WIDTH * strlen(string)) / 2);
    game.textures.prompt.dst_rect.y = (SCREEN_HEIGHT/ 2) + (FONT_SIZE_HEIGHT * 4);
    return string;
}
extern char* format_you_won(void) {
    static char string[MAX_STRING_LENGTH] = "YOU WON!";
    game.textures.you_won.dst_rect.w = (FONT_SIZE_WIDTH * strlen(string));
    game.textures.you_won.dst_rect.h = FONT_SIZE_HEIGHT;
    game.textures.you_won.dst_rect.x = (SCREEN_WIDTH / 2) - ((FONT_SIZE_WIDTH * strlen(string)) / 2);
    game.textures.you_won.dst_rect.y = (SCREEN_HEIGHT / 2) - FONT_SIZE_HEIGHT - FONT_SIZE_HEIGHT;
    return string;
}
extern char* format_you_lost(void) {
    static char string[MAX_STRING_LENGTH] = "YOU LOST!";
    game.textures.you_lost.dst_rect.w = (FONT_SIZE_WIDTH * strlen(string));
    game.textures.you_lost.dst_rect.h = FONT_SIZE_HEIGHT;
    game.textures.you_lost.dst_rect.x = (SCREEN_WIDTH / 2) - (game.textures.you_lost.dst_rect.w / 2);
    game.textures.you_lost.dst_rect.y = (SCREEN_HEIGHT / 2) - game.textures.you_lost.dst_rect.h - FONT_SIZE_HEIGHT;
    return string;
}
extern char* format_press_r(void) {
    static char string[MAX_STRING_LENGTH];
    if(game.player.lives && game.end && !lock) {
        strcpy(string, "PRESS R TO CONTINUE"); lock = TRUE;
    } else if(!(game.player.lives) && game.end && !lock) {
        strcpy(string, "PRESS R TO REPLAY"); lock = TRUE;
    }
    lock = (game.end) ? TRUE : FALSE;
    game.textures.press_r.dst_rect.w = (FONT_SIZE_WIDTH * strlen(string));
    game.textures.press_r.dst_rect.h = FONT_SIZE_HEIGHT;
    game.textures.press_r.dst_rect.x = (SCREEN_WIDTH / 2) - (game.textures.press_r.dst_rect.w / 2);
    game.textures.press_r.dst_rect.y = (SCREEN_HEIGHT / 2) - game.textures.press_r.dst_rect.h + FONT_SIZE_HEIGHT;
    return string;
}
extern char* format_press_q(void) {
    static char string[MAX_STRING_LENGTH];
    strcpy(string, "PRESS Q TO EXIT");
    game.textures.press_q.dst_rect.w = (FONT_SIZE_WIDTH * strlen(string));
    game.textures.press_q.dst_rect.h = FONT_SIZE_HEIGHT;
    game.textures.press_q.dst_rect.x = (SCREEN_WIDTH / 2) - ((FONT_SIZE_WIDTH * strlen(string)) / 2);
    game.textures.press_q.dst_rect.y = (SCREEN_HEIGHT/ 2) - FONT_SIZE_HEIGHT + FONT_SIZE_HEIGHT * 2;
    return string;
}
extern char* format_press_esc(void) {
    static char string[MAX_STRING_LENGTH];
    strcpy(string, "PRESS ESCAPE TO RESUME");
    game.textures.press_esc.dst_rect.w = (FONT_SIZE_WIDTH * strlen(string));
    game.textures.press_esc.dst_rect.h = FONT_SIZE_HEIGHT;
    game.textures.press_esc.dst_rect.x = (SCREEN_WIDTH / 2) - ((FONT_SIZE_WIDTH * strlen(string)) / 2);
    game.textures.press_esc.dst_rect.y = (SCREEN_HEIGHT/ 2) - FONT_SIZE_HEIGHT + FONT_SIZE_HEIGHT;
    return string;
}
extern char* format_paused(void) {
    static char string[MAX_STRING_LENGTH] = "GAME PAUSED";
    game.textures.paused.dst_rect.w = (FONT_SIZE_WIDTH * strlen(string));
    game.textures.paused.dst_rect.h = FONT_SIZE_HEIGHT;
    game.textures.paused.dst_rect.x = (SCREEN_WIDTH / 2) - ((FONT_SIZE_WIDTH * strlen(string)) / 2);
    game.textures.paused.dst_rect.y = (SCREEN_HEIGHT/ 2) - FONT_SIZE_HEIGHT - FONT_SIZE_HEIGHT;
    return string;
}
extern char* format_score(void) {
    static char string[MAX_STRING_LENGTH];
    sprintf(string, "SCORE: %d", game.player.score);
    game.textures.score.dst_rect.w = (FONT_SIZE_WIDTH * strlen(string));
    game.textures.score.dst_rect.h = FONT_SIZE_HEIGHT;
    game.textures.score.dst_rect.x = TEXT_SIDE_MARGIN;
    game.textures.score.dst_rect.y = TEXT_SIDE_MARGIN;
    return string;
}
extern char* format_lives(void) {
    static char string[MAX_STRING_LENGTH];
    sprintf(string, "LIVES: %d", game.player.lives);
    game.textures.lives.dst_rect.w = (FONT_SIZE_WIDTH * strlen(string));
    game.textures.lives.dst_rect.h = FONT_SIZE_HEIGHT;
    game.textures.lives.dst_rect.x = SCREEN_WIDTH - (FONT_SIZE_WIDTH * strlen(string))- TEXT_TOP_MARGIN;
    game.textures.lives.dst_rect.y = TEXT_SIDE_MARGIN;
    return string;
}
extern char* format_hi_score(void) {
    static char string[MAX_STRING_LENGTH];
    int highest_score = 0;
    FILE* score_list = fopen(SCORE_FILE_PATH, "r");
    int temp;
    while(fscanf(score_list, "%d", &temp) == 1)
        if (temp > highest_score)
          highest_score = temp;
    fclose(score_list);
    sprintf(string, "HIGHEST SCORE: %d", highest_score);
    game.textures.highest_score.dst_rect.w = (FONT_SIZE_WIDTH * strlen(string));
    game.textures.highest_score.dst_rect.h = FONT_SIZE_HEIGHT;
    game.textures.highest_score.dst_rect.x = (SCREEN_WIDTH / 2) - ((FONT_SIZE_WIDTH * strlen(string)) / 2);
    game.textures.highest_score.dst_rect.y = (SCREEN_HEIGHT/ 2) + (FONT_SIZE_HEIGHT * 3);
    return string;
}
static void load_text(Text* text, char* string) {
    static SDL_Color colour;
    colour.r = 76;
    colour.g = 255;
    colour.b = 0;
    colour.a = 255;
    SDL_Surface* tmp_text_surface = TTF_RenderText_Solid(text->font, string, colour);
    text->text_texture = SDL_CreateTextureFromSurface(game.renderer, tmp_text_surface);
    SDL_FreeSurface(tmp_text_surface);
    string = NULL;
}
static void text_render(Text* text) {
    SDL_RenderCopy(game.renderer, text->text_texture, NULL, &text->dst_rect);
}
static Text new_text(TTF_Font* font) {
    Text text;
    text.load_text = &load_text;
    text.dst_rect.x = 0;
    text.dst_rect.y = 0;
    text.dst_rect.w = 0;
    text.dst_rect.h = 0;
    text.text_texture = NULL;
    text.render = &text_render;
    text.font = font;
    return text;
}
static void textures_destroy(TextureManager* textures) {
    SDL_DestroyTexture(textures->player_texture);
    SDL_DestroyTexture(textures->player_projectile_texture);
    SDL_DestroyTexture(textures->alien_projectile_texture);
    SDL_DestroyTexture(textures->alien_1_texture);
    SDL_DestroyTexture(textures->alien_2_texture);
    SDL_DestroyTexture(textures->alien_3_texture);
    SDL_DestroyTexture(textures->block_type_0);
    SDL_DestroyTexture(textures->block_type_1);
    SDL_DestroyTexture(textures->block_type_2);
    SDL_DestroyTexture(textures->block_type_3);
    SDL_DestroyTexture(textures->block_type_4);
    TTF_Quit();
}
SDL_Texture* load_texture(const char* filepath, SDL_Renderer* renderer) {
    SDL_Surface *tmp_surface = IMG_Load(filepath);
    SDL_Texture *texture;
    if((texture = SDL_CreateTextureFromSurface(renderer, tmp_surface)) == NULL) {
        printf("While loading %s path\n", filepath);
        printf("Error: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_FreeSurface(tmp_surface);
    return texture;
}
extern void update_all_text(TextureManager* textures) {
        textures->score.load_text(&textures->score, format_score());
        textures->lives.load_text(&textures->lives, format_lives());

        textures->paused.load_text(&textures->paused, format_paused());
        textures->press_q.load_text(&textures->press_q, format_press_q());

        textures->press_esc.load_text(&textures->press_esc, format_press_esc());
        textures->highest_score.load_text(&textures->highest_score, format_hi_score());
        textures->press_m.load_text(&textures->press_m, format_press_m());

        textures->you_won.load_text(&textures->you_won, format_you_won());
        textures->you_lost.load_text(&textures->you_lost, format_you_lost());
        textures->press_r.load_text(&textures->press_r, format_press_r());
}
extern void free_all_text(TextureManager * textures) {
      SDL_DestroyTexture(textures->score.text_texture);
      SDL_DestroyTexture(textures->lives.text_texture);

      SDL_DestroyTexture(textures->paused.text_texture);
      SDL_DestroyTexture(textures->press_q.text_texture);

      SDL_DestroyTexture(textures->press_esc.text_texture);
      SDL_DestroyTexture(textures->highest_score.text_texture);
      SDL_DestroyTexture(textures->press_m.text_texture);

      SDL_DestroyTexture(textures->you_won.text_texture);
      SDL_DestroyTexture(textures->you_lost.text_texture);
      SDL_DestroyTexture(textures->press_r.text_texture);
}
extern void init_startup_text(TextureManager* textures) {
    textures->prompt.load_text(&textures->prompt, format_prompt());
    textures->title_1.load_text(&textures->title_1, format_title_1());
    textures->title_2.load_text(&textures->title_2, format_title_2());
    textures->highest_score.load_text(&textures->highest_score, format_hi_score());
}
extern void render_startup_text(TextureManager* textures) {
    textures->prompt.render(&textures->prompt);
    textures->title_1.render(&textures->title_1);
    textures->title_2.render(&textures->title_2);
    textures->highest_score.render(&textures->highest_score);
}
extern void dim_screen(TextureManager* textures) {
    if(game.paused == TRUE || !(game.player.lives) || !(Aliens.size)) {
        static uint8_t dimmer = 100;
        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = SCREEN_WIDTH;
        rect.h = SCREEN_HEIGHT;
        SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, dimmer);
        SDL_RenderFillRect(game.renderer, &rect);
        SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderDrawRect(game.renderer, &rect);
        textures->press_q.render(&textures->press_q);
        textures->highest_score.render(&textures->highest_score);
        if (!(Aliens.size)) {
          textures->you_won.render(&textures->you_won);
          textures->press_r.render(&textures->press_r);
        } else if (!(game.player.lives)) {
          textures->you_lost.render(&textures->you_lost);
          textures->press_r.render(&textures->press_r);
        } else if (game.paused) {
          textures->paused.render(&textures->paused);
          textures->press_esc.render(&textures->press_esc);
          textures->press_m.render(&textures->press_m);

        }
    }
}

