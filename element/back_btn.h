
#ifndef BACK_BTN_H_INCLUDED
#define BACK_BTN_H_INCLUDED
#include <allegro5/allegro_font.h>

#include "../shapes/Shape.h"
#include "element.h"
typedef struct _back_btn {
  double x1, y1, x2, y2;
  ALLEGRO_COLOR color;
  bool selected;
  ALLEGRO_FONT *font;
  Shape *hitbox;
} back_btn;
Elements *New_back_btn(int label);
void back_btn_update(Elements *self);
void back_btn_interact(Elements *self);
void back_btn_draw(Elements *self);
void back_btn_destory(Elements *self);

#endif
