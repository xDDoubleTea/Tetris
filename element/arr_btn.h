
#ifndef ARR_BTN_H_INCLUDED
#define ARR_BTN_H_INCLUDED
#include <allegro5/allegro_font.h>

#include "../shapes/Shape.h"
#include "element.h"
enum ArrStates {
  Really_Slow,
  Slow,
  Mid,
  Fast,
  SuperFast,
  NUM_ARR_STATE,
};
typedef struct _arr_btn {
  double x1, y1, x2, y2;
  ALLEGRO_COLOR color;
  bool selected;
  bool update_lock;
  int arrstate[NUM_ARR_STATE];
  char arrstatetext[NUM_ARR_STATE][32];
  int now_state;
  ALLEGRO_FONT *font;
  ALLEGRO_FONT *font_small;
  Shape *hitbox;
} arr_btn;

Elements *New_arr_btn(int label);
void arr_btn_update(Elements *self);
void arr_btn_interact(Elements *self);
void arr_btn_draw(Elements *self);
void arr_btn_destory(Elements *self);

#endif
