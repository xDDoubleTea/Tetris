
#include "das_btn.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "../shapes/Rectangle.h"
Elements *New_das_btn(int label) {
  das_btn *pDerivedObj = (das_btn *)malloc(sizeof(das_btn));
  Elements *pObj = New_Elements(label);
  // setting derived object member
  pDerivedObj->x1 = (double)WIDTH / 2 - 350;
  pDerivedObj->y1 = (double)HEIGHT / 2 - 50;
  pDerivedObj->x2 = (double)WIDTH / 2 + 350;
  pDerivedObj->y2 = (double)HEIGHT / 2 + 100;
  pDerivedObj->color = al_map_rgb(255, 0, 0);
  pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x1, pDerivedObj->y1,
                                      pDerivedObj->x2, pDerivedObj->y2);
  pDerivedObj->selected = false;
  pDerivedObj->update_lock = false;
  pDerivedObj->dasstate[0] = 25;
  pDerivedObj->dasstate[1] = 20;
  pDerivedObj->dasstate[2] = 15;
  pDerivedObj->dasstate[3] = 10;
  pDerivedObj->dasstate[4] = 5;
  pDerivedObj->now_state = (25 - das) / 5;
  pDerivedObj->enabled = false;
  pDerivedObj->enable_timer = 0;
  snprintf(pDerivedObj->dasstatetext[0], sizeof(pDerivedObj->dasstatetext[0]),
           "Really Slow");
  snprintf(pDerivedObj->dasstatetext[1], sizeof(pDerivedObj->dasstatetext[1]),
           "Slow");
  snprintf(pDerivedObj->dasstatetext[2], sizeof(pDerivedObj->dasstatetext[2]),
           "Mid");
  snprintf(pDerivedObj->dasstatetext[3], sizeof(pDerivedObj->dasstatetext[3]),
           "Fast");
  snprintf(pDerivedObj->dasstatetext[4], sizeof(pDerivedObj->dasstatetext[4]),
           "Super Fast");
  pDerivedObj->font = al_load_ttf_font("assests/font/hun2.ttf", 48, 0);
  pDerivedObj->font_small = al_load_ttf_font("assests/font/hun2.ttf", 12, 0);
  // setting derived object function
  pObj->pDerivedObj = pDerivedObj;
  pObj->Update = das_btn_update;
  pObj->Interact = das_btn_interact;
  pObj->Draw = das_btn_draw;
  pObj->Destroy = das_btn_destory;
  return pObj;
}
void das_btn_update(Elements *self) {
  das_btn *btn = ((das_btn *)(self->pDerivedObj));
  if (mouse.x > btn->x1 && mouse.x < btn->x2 && mouse.y > btn->y1 &&
      mouse.y < btn->y2) {
    btn->color = al_map_rgb(0, 255, 0);
    btn->selected = true;
  } else {
    btn->color = al_map_rgb(255, 0, 0);
    btn->selected = false;
  }
  if (btn->enable_timer < (int)FPS * 0.3) {
    btn->enable_timer++;
    return;
  }
  ALLEGRO_MOUSE_STATE mouse_state;
  al_get_mouse_state(&mouse_state);
  if (!btn->update_lock && btn->selected &&
      al_mouse_button_down(&mouse_state, 1)) {
    btn->now_state = (btn->now_state + 1) % 5;
    das = btn->dasstate[btn->now_state];
    btn->update_lock = true;
  }
  if (btn->update_lock && !al_mouse_button_down(&mouse_state, 1)) {
    btn->update_lock = false;
  }
}
void das_btn_interact(Elements *self) {}
void das_btn_draw(Elements *self) {
  das_btn *btn = ((das_btn *)(self->pDerivedObj));
  al_draw_text(btn->font, btn->color, (btn->x1 + btn->x2) / 2, btn->y1 + 25,
               ALLEGRO_ALIGN_CENTRE, "Delayed Auto Shift (DAS)");
  al_draw_textf(btn->font, btn->color, (btn->x1 + btn->x2) / 2, btn->y1 + 75,
                ALLEGRO_ALIGN_CENTRE, "%s, %d",
                btn->dasstatetext[btn->now_state], das);
  al_draw_rounded_rectangle(btn->x1, btn->y1, btn->x2, btn->y2, 10, 10,
                            btn->color, 5);
}
void das_btn_destory(Elements *self) {
  das_btn *Obj = ((das_btn *)(self->pDerivedObj));
  free(Obj->hitbox);
  free(Obj);
  free(self);
}
