#include "back_btn.h"
#include "../scene/gamescene.h"
#include "../scene/sceneManager.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "../shapes/Rectangle.h"
#include "quit_btn.h"
#include "settings_btn.h"
#include "start_btn.h"
Elements *New_back_btn(int label) {
  back_btn *pDerivedObj = (back_btn *)malloc(sizeof(back_btn));
  Elements *pObj = New_Elements(label);
  // setting derived object member
  pDerivedObj->x1 = (double)WIDTH / 2 - 200;
  pDerivedObj->y1 = (double)HEIGHT / 2 + 400;
  pDerivedObj->x2 = (double)WIDTH / 2 + 200;
  pDerivedObj->y2 = (double)HEIGHT / 2 + 500;
  pDerivedObj->color = al_map_rgb(255, 0, 0);
  pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x1, pDerivedObj->y1,
                                      pDerivedObj->x2, pDerivedObj->y2);
  pDerivedObj->selected = false;
  pDerivedObj->font = al_load_ttf_font("assets/font/hun2.ttf", 48, 0);
  // setting derived object function
  pObj->pDerivedObj = pDerivedObj;
  pObj->Update = back_btn_update;
  pObj->Interact = back_btn_interact;
  pObj->Draw = back_btn_draw;
  pObj->Destroy = back_btn_destory;
  return pObj;
}
void back_btn_update(Elements *self) {
  back_btn *btn = ((back_btn *)(self->pDerivedObj));
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
  if (btn->selected && al_mouse_button_down(&mouse_state, 1)) {
    ElementVec allele = _Get_all_elements(scene);
    for (int i = 0; i < allele.len; ++i) {
      allele.arr[i]->dele = true;
    }
    _Register_elements(scene, New_Start_btn(Start_btn_L));
    _Register_elements(scene, New_Settings_btn(Settings_btn_L));
    _Register_elements(scene, New_Quit_btn(Quit_btn_L));
  }
}
void back_btn_interact(Elements *self) {}
void back_btn_draw(Elements *self) {
  back_btn *btn = ((back_btn *)(self->pDerivedObj));
  al_draw_text(btn->font, btn->color, (btn->x1 + btn->x2) / 2,
               (btn->y1 + btn->y2) / 2 - 25, ALLEGRO_ALIGN_CENTRE,
               "Back to menu");
  al_draw_rounded_rectangle(btn->x1, btn->y1, btn->x2, btn->y2, 10, 10,
                            btn->color, 5);
}
void back_btn_destory(Elements *self) {
  back_btn *Obj = ((back_btn *)(self->pDerivedObj));
  free(Obj->hitbox);
  free(Obj);
  free(self);
}
