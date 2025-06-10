
#ifndef SDF_BTN_H_INCLUDED
#define SDF_BTN_H_INCLUDED
#include <allegro5/allegro_font.h>

#include "../shapes/Shape.h"
#include "element.h"
typedef struct _sdf_btn {
  double x1, y1, x2, y2;
  ALLEGRO_COLOR color;
  bool selected;
  bool update_lock;
  int sdfstate[5];
  char sdfstatetext[5][32];
  int now_state;
  ALLEGRO_FONT *font;
  ALLEGRO_FONT *font_small;
  Shape *hitbox;
} sdf_btn;

Elements *New_sdf_btn(int label);
void sdf_btn_update(Elements *self);
void sdf_btn_interact(Elements *self);
void sdf_btn_draw(Elements *self);
void sdf_btn_destory(Elements *self);

#endif
