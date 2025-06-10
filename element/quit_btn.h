
#ifndef QUIT_BTN_H_INCLUDED
#define QUIT_BTN_H_INCLUDED
#include <allegro5/allegro_font.h>

#include "../shapes/Shape.h"
#include "element.h"
typedef struct _Quit_btn {
  double x1, y1, x2, y2;
  ALLEGRO_COLOR color;
  bool selected;
  ALLEGRO_FONT *font;
  Shape *hitbox;
} Quit_btn;
Elements *New_Quit_btn(int label);
void Quit_btn_update(Elements *self);
void Quit_btn_interact(Elements *self);
void Quit_btn_draw(Elements *self);
void Quit_btn_destory(Elements *self);

#endif
