#ifndef START_BTN_H_INCLUDED
#define START_BTN_H_INCLUDED
#include <allegro5/allegro_font.h>

#include "../shapes/Shape.h"
#include "element.h"
typedef struct _Start_btn {
  double x1, y1, x2, y2;
  ALLEGRO_COLOR color;
  bool selected;
  ALLEGRO_FONT *font;
  Shape *hitbox;
} Start_btn;
Elements *New_Start_btn(int label);
void Start_btn_update(Elements *self);
void Start_btn_interact(Elements *self);
void Start_btn_draw(Elements *self);
void Start_btn_destory(Elements *self);

#endif
