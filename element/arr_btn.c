#include "arr_btn.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/mouse.h>

#include "../shapes/Rectangle.h"
Elements *New_arr_btn(int label) {
  arr_btn *pDerivedObj = (arr_btn *)malloc(sizeof(arr_btn));
  Elements *pObj = New_Elements(label);
  // setting derived object member
  pDerivedObj->x1 = (double)WIDTH / 2 - 350;
  pDerivedObj->y1 = (double)HEIGHT / 2 - 250;
  pDerivedObj->x2 = (double)WIDTH / 2 + 350;
  pDerivedObj->y2 = (double)HEIGHT / 2 - 100;
  pDerivedObj->color = al_map_rgb(255, 0, 0);
  pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x1, pDerivedObj->y1,
                                      pDerivedObj->x2, pDerivedObj->y2);
  pDerivedObj->selected = false;
  pDerivedObj->update_lock = false;
  pDerivedObj->arrstate[0] = 25;
  pDerivedObj->arrstate[1] = 20;
  pDerivedObj->arrstate[2] = 15;
  pDerivedObj->arrstate[3] = 10;
  pDerivedObj->arrstate[4] = 5;
  pDerivedObj->now_state = (25 - arr) / 5;
  snprintf(pDerivedObj->arrstatetext[0], sizeof(pDerivedObj->arrstatetext[0]),
           "Really Slow");
  snprintf(pDerivedObj->arrstatetext[1], sizeof(pDerivedObj->arrstatetext[1]),
           "Slow");
  snprintf(pDerivedObj->arrstatetext[2], sizeof(pDerivedObj->arrstatetext[2]),
           "Mid");
  snprintf(pDerivedObj->arrstatetext[3], sizeof(pDerivedObj->arrstatetext[3]),
           "Fast");
  snprintf(pDerivedObj->arrstatetext[4], sizeof(pDerivedObj->arrstatetext[4]),
           "Super Fast");
  pDerivedObj->font = al_load_ttf_font("assests/font/hun2.ttf", 48, 0);
  pDerivedObj->font_small = al_load_ttf_font("assests/font/hun2.ttf", 12, 0);
  // setting derived object function
  pObj->pDerivedObj = pDerivedObj;
  pObj->Update = arr_btn_update;
  pObj->Interact = arr_btn_interact;
  pObj->Draw = arr_btn_draw;
  pObj->Destroy = arr_btn_destory;
  return pObj;
}
void arr_btn_update(Elements *self) {
  arr_btn *btn = ((arr_btn *)(self->pDerivedObj));
  if (mouse.x > btn->x1 && mouse.x < btn->x2 && mouse.y > btn->y1 &&
      mouse.y < btn->y2) {
    btn->color = al_map_rgb(0, 255, 0);
    btn->selected = true;
  } else {
    btn->color = al_map_rgb(255, 0, 0);
    btn->selected = false;
  }
  ALLEGRO_MOUSE_STATE mouse_state;
  al_get_mouse_state(&mouse_state);
  if (!btn->update_lock && btn->selected &&
      al_mouse_button_down(&mouse_state, 1)) {
    btn->now_state = (btn->now_state + 1) % 5;
    arr = btn->arrstate[btn->now_state];
    btn->update_lock = true;
  }
  if (btn->update_lock && !al_mouse_button_down(&mouse_state, 1)) {
    btn->update_lock = false;
  }
}
void arr_btn_interact(Elements *self) {}
void arr_btn_draw(Elements *self) {
  arr_btn *btn = ((arr_btn *)(self->pDerivedObj));
  al_draw_text(btn->font, btn->color, (btn->x1 + btn->x2) / 2, btn->y1 + 25,
               ALLEGRO_ALIGN_CENTRE, "Auto Repeat Rate (ARR)");
  al_draw_textf(btn->font, btn->color, (btn->x1 + btn->x2) / 2, btn->y1 + 75,
                ALLEGRO_ALIGN_CENTRE, "%s, %d",
                btn->arrstatetext[btn->now_state], arr);
  al_draw_rounded_rectangle(btn->x1, btn->y1, btn->x2, btn->y2, 10, 10,
                            btn->color, 5);
}
void arr_btn_destory(Elements *self) {
  arr_btn *Obj = ((arr_btn *)(self->pDerivedObj));
  free(Obj->hitbox);
  free(Obj);
  free(self);
}
