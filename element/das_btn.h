
#ifndef DAS_BTN_H_INCLUDED
#define DAS_BTN_H_INCLUDED
#include <allegro5/allegro_font.h>

#include "../shapes/Shape.h"
#include "element.h"
typedef struct _das_btn {
  double x1, y1, x2, y2;
  ALLEGRO_COLOR color;
  bool selected;
  bool update_lock;
  int dasstate[5];
  bool enabled;
  int enable_timer;
  char dasstatetext[5][32];
  int now_state;
  ALLEGRO_FONT *font;
  ALLEGRO_FONT *font_small;
  Shape *hitbox;
} das_btn;

Elements *New_das_btn(int label);
void das_btn_update(Elements *self);
void das_btn_interact(Elements *self);
void das_btn_draw(Elements *self);
void das_btn_destory(Elements *self);

#endif
