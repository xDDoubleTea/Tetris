
#ifndef SETTINGS_BTN_H_INCLUDED
#define SETTINGS_BTN_H_INCLUDED
#include <allegro5/allegro_font.h>

#include "../shapes/Shape.h"
#include "element.h"
typedef struct _Settings_btn {
  double x1, y1, x2, y2;
  ALLEGRO_COLOR color;
  bool selected;
  ALLEGRO_FONT *font;
  Shape *hitbox;
} Settings_btn;
Elements *New_Settings_btn(int label);
void Settings_btn_update(Elements *self);
void Settings_btn_interact(Elements *self);
void Settings_btn_draw(Elements *self);
void Settings_btn_destory(Elements *self);

#endif
