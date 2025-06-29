#include "sdf_btn.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/mouse.h>

#include "../shapes/Rectangle.h"
Elements *New_sdf_btn(int label) {
  sdf_btn *pDerivedObj = (sdf_btn *)malloc(sizeof(sdf_btn));
  Elements *pObj = New_Elements(label);
  // setting derived object member
  pDerivedObj->x1 = (double)WIDTH / 2 - 350;
  pDerivedObj->y1 = (double)HEIGHT / 2 + 150;
  pDerivedObj->x2 = (double)WIDTH / 2 + 350;
  pDerivedObj->y2 = (double)HEIGHT / 2 + 300;
  pDerivedObj->color = al_map_rgb(255, 0, 0);
  pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x1, pDerivedObj->y1,
                                      pDerivedObj->x2, pDerivedObj->y2);
  pDerivedObj->selected = false;
  pDerivedObj->update_lock = false;
  pDerivedObj->sdfstate[0] = 8;
  pDerivedObj->sdfstate[1] = 16;
  pDerivedObj->sdfstate[2] = 24;
  pDerivedObj->sdfstate[3] = 32;
  pDerivedObj->sdfstate[4] = 40;
  pDerivedObj->now_state = (sdf / 8) - 1;
  snprintf(pDerivedObj->sdfstatetext[0], sizeof(pDerivedObj->sdfstatetext[0]),
           "Really Slow");
  snprintf(pDerivedObj->sdfstatetext[1], sizeof(pDerivedObj->sdfstatetext[1]),
           "Slow");
  snprintf(pDerivedObj->sdfstatetext[2], sizeof(pDerivedObj->sdfstatetext[2]),
           "Mid");
  snprintf(pDerivedObj->sdfstatetext[3], sizeof(pDerivedObj->sdfstatetext[3]),
           "Fast");
  snprintf(pDerivedObj->sdfstatetext[4], sizeof(pDerivedObj->sdfstatetext[4]),
           "Super Fast");
  pDerivedObj->font = al_load_ttf_font("assets/font/hun2.ttf", 48, 0);
  pDerivedObj->font_small = al_load_ttf_font("assets/font/hun2.ttf", 12, 0);
  // setting derived object function
  pObj->pDerivedObj = pDerivedObj;
  pObj->Update = sdf_btn_update;
  pObj->Interact = sdf_btn_interact;
  pObj->Draw = sdf_btn_draw;
  pObj->Destroy = sdf_btn_destory;
  return pObj;
}
void sdf_btn_update(Elements *self) {
  sdf_btn *btn = ((sdf_btn *)(self->pDerivedObj));
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
    sdf = btn->sdfstate[btn->now_state];
    btn->update_lock = true;
  }
  if (btn->update_lock && !al_mouse_button_down(&mouse_state, 1)) {
    btn->update_lock = false;
  }
}
void sdf_btn_interact(Elements *self) {}
void sdf_btn_draw(Elements *self) {
  sdf_btn *btn = ((sdf_btn *)(self->pDerivedObj));
  al_draw_text(btn->font, btn->color, (btn->x1 + btn->x2) / 2, btn->y1 + 25,
               ALLEGRO_ALIGN_CENTRE, "Soft Drop Factor (SDF)");
  al_draw_textf(btn->font, btn->color, (btn->x1 + btn->x2) / 2, btn->y1 + 75,
                ALLEGRO_ALIGN_CENTRE, "%s, %d",
                btn->sdfstatetext[btn->now_state], sdf);
  al_draw_rounded_rectangle(btn->x1, btn->y1, btn->x2, btn->y2, 10, 10,
                            btn->color, 5);
}
void sdf_btn_destory(Elements *self) {
  sdf_btn *Obj = ((sdf_btn *)(self->pDerivedObj));
  free(Obj->hitbox);
  free(Obj);
  free(self);
}
