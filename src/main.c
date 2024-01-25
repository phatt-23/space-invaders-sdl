#include "sdl_context.h"

int main(int argc, char** argv) {
    game = new_context(argc, argv);
    game.sdl_init(&game, "Space Invaders");
    while(game.running) {
        game.event_handler.handle_events(&game.event_handler);
        game.sdl_update(&game);
        game.sdl_render(&game);
    }
    game.sdl_clean(&game);
    return 0;
}


