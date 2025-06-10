#ifndef ZOMBIE_H_INCLUDED
#define ZOMBIE_H_INCLUDED
#include "../algif5/algif.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
#include "element.h"
#include <allegro5/allegro_font.h>
typedef struct _Zombie {
  int x1, y1, x2, y2;
  int w, h;
  ALLEGRO_COLOR color;
  ALGIF_ANIMATION *zombiewalk;
  int walkspeed;
  int hp;
  int stop_walk;
  ALLEGRO_FONT *font;
  int walktimer;
  Shape *hitbox;
} Zombie;
Elements *New_zombie(int label);
void zombie_update(Elements *self);
void zombie_interact(Elements *self);
void zombie_draw(Elements *self);
void zombie_destory(Elements *self);

#endif
