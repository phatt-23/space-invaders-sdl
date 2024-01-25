#ifndef SDL_GAME_TEXTURE_MANAGER_H
#define SDL_GAME_TEXTURE_MANAGER_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct game_textures TextureManager;
typedef struct texture_text Text;
struct texture_text {
    void (*load_text)(Text* self, char* string);
    void (*render)(Text* self);
    SDL_Texture* text_texture;
    SDL_Rect dst_rect;
    TTF_Font* font;
};
struct game_textures {
    void (*destroy)(TextureManager* textures);
    SDL_Texture* player_texture;           // Texture of Player's ship.
    SDL_Texture* alien_1_texture;          // Texture of the Alien-1 (for 5 points)
    SDL_Texture* alien_2_texture;          // Texture of the Alien-2 (for 10 point)
    SDL_Texture* alien_3_texture;          // Texture of the Alien-3 (for 15 point)
    SDL_Texture* alien_4_texture;          // Texture of the Alien-4 (for 20 point)
    SDL_Texture* player_projectile_texture;       // Texture of the Projectile
    SDL_Texture* alien_projectile_texture;       // Texture of the Projectile
    SDL_Texture* explosion_texture;        // animated sprite for the explosion
    SDL_Texture* block_type_0;             // animated sprite for the block type 0
    SDL_Texture* block_type_1;             // animated sprite for the block type 1
    SDL_Texture* block_type_2;             // animated sprite for the block type 1
    SDL_Texture* block_type_3;             // animated sprite for the block type 1
    SDL_Texture* block_type_4;             // animated sprite for the block type 1

    TTF_Font* font;
    
    Text title_1;
    Text title_2;
    Text highest_score;
    Text score;
    Text lives;
    Text prompt;
    Text you_won;
    Text you_lost;
    Text press_r;
    Text paused;
    Text press_q;
    Text press_esc;
    Text press_m;
};

extern TextureManager new_texture_manager(SDL_Renderer* renderer);

extern char* format_title_1(void);
extern char* format_title_2(void);
extern char* format_score(void);
extern char* format_lives(void);
extern char* format_prompt(void);
extern char* format_you_won(void);
extern char* format_you_lost(void);
extern char* format_press_r(void);
extern char* format_paused(void);
extern char* format_press_q(void);
extern char* format_press_esc(void);
extern char* format_hi_score(void);
extern char* format_press_q(void);

extern void init_startup_text(TextureManager* textures);
extern void render_startup_text(TextureManager* textures);
extern void update_all_text(TextureManager* textures);
extern void free_all_text(TextureManager* textures);
extern void dim_screen(TextureManager* textures);

#endif //SDL_GAME_TEXTURE_MANAGER_H
