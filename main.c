#define ALLEGRO_NO_MAGIC_MAIN

#include <stdlib.h>
#include <time.h>

#include "GameWindow.h"
int real_main(int argc, char** argv) {
    Game* game = New_Game();
    game->execute(game);
    game->game_destroy(game);
    return 0;
}

int main(int argc, char** argv) {
    srand(time(NULL));
    return al_run_main(argc, argv, real_main);
}
