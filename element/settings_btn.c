#include "settings_btn.h"
#include "../scene/gamescene.h"
#include "../scene/sceneManager.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "../shapes/Rectangle.h"
#include "arr_btn.h"
#include "back_btn.h"
#include "das_btn.h"
#include "sdf_btn.h"
Elements *New_Settings_btn(int label) {
  Settings_btn *pDerivedObj = (Settings_btn *)malloc(sizeof(Settings_btn));
  Elements *pObj = New_Elements(label);
  // setting derived object member
  pDerivedObj->x1 = (double)WIDTH / 2 - 200;
  pDerivedObj->y1 = (double)HEIGHT / 2 - 50;
  pDerivedObj->x2 = (double)WIDTH / 2 + 200;
  pDerivedObj->y2 = (double)HEIGHT / 2 + 50;
  pDerivedObj->color = al_map_rgb(255, 0, 0);
  pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x1, pDerivedObj->y1,
                                      pDerivedObj->x2, pDerivedObj->y2);
  pDerivedObj->selected = false;
  pDerivedObj->font = al_load_ttf_font("assests/font/hun2.ttf", 48, 0);
  // setting derived object function
  pObj->pDerivedObj = pDerivedObj;
  pObj->Update = Settings_btn_update;
  pObj->Interact = Settings_btn_interact;
  pObj->Draw = Settings_btn_draw;
  pObj->Destroy = Settings_btn_destory;
  return pObj;
}
void Settings_btn_update(Elements *self) {
  Settings_btn *btn = ((Settings_btn *)(self->pDerivedObj));
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
    _Register_elements(scene, New_das_btn(Das_btn_L));
    _Register_elements(scene, New_arr_btn(Arr_btn_L));
    _Register_elements(scene, New_back_btn(Back_btn_L));
    _Register_elements(scene, New_sdf_btn(Sdf_Btn_L));
  }
}
void Settings_btn_interact(Elements *self) {}
void Settings_btn_draw(Elements *self) {
  Settings_btn *btn = ((Settings_btn *)(self->pDerivedObj));
  al_draw_text(btn->font, btn->color, btn->x1 + 200, btn->y1 + 25,
               ALLEGRO_ALIGN_CENTRE, "Settings");
  al_draw_rounded_rectangle(btn->x1, btn->y1, btn->x2, btn->y2, 10, 10,
                            btn->color, 5);
}
void Settings_btn_destory(Elements *self) {
  Settings_btn *Obj = ((Settings_btn *)(self->pDerivedObj));
  free(Obj->hitbox);
  free(Obj);
  free(self);
}
