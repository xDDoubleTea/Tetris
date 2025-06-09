#include <allegro5/config.h>
#define ALLEGRO_NO_MAGIC_MAIN

#include "GameWindow.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int real_main(int argc, char **argv) {
  Game *game = New_Game();
  game->execute(game);
  game->game_destroy(game);
  return 0;
}

int main(int argc, char **argv) {
  srand(time(NULL));
  ALLEGRO_CONFIG *cfg = al_load_config_file("config.cfg");
  arr = atoi(al_get_config_value(cfg, "", "arr"));
  das = atoi(al_get_config_value(cfg, "", "das"));
  sdf = atoi(al_get_config_value(cfg, "", "sdf"));
  al_destroy_config(cfg);
  return al_run_main(argc, argv, real_main);
}
