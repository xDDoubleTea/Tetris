
#include "start_btn.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "../element/element.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
Elements *New_Start_btn(int label) {
  Start_btn *pDerivedObj = (Start_btn *)malloc(sizeof(Start_btn));
  Elements *pObj = New_Elements(label);
  // setting derived object member
  pDerivedObj->x1 = (double)WIDTH / 2 - 150;
  pDerivedObj->y1 = (double)HEIGHT / 2 - 200;
  pDerivedObj->x2 = (double)WIDTH / 2 + 150;
  pDerivedObj->y2 = (double)HEIGHT / 2 - 100;
  pDerivedObj->color = al_map_rgb(255, 0, 0);
  pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x1, pDerivedObj->y1,
                                      pDerivedObj->x2, pDerivedObj->y2);
  pDerivedObj->selected = false;
  pDerivedObj->font = al_load_ttf_font("assets/font/hun2.ttf", 48, 0);
  // setting derived object function
  pObj->pDerivedObj = pDerivedObj;
  pObj->Update = Start_btn_update;
  pObj->Interact = Start_btn_interact;
  pObj->Draw = Start_btn_draw;
  pObj->Destroy = Start_btn_destory;
  return pObj;
}
void Start_btn_update(Elements *self) {
  Start_btn *btn = ((Start_btn *)(self->pDerivedObj));
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
    scene->scene_end = true;
    window = 1;
  }
}
void Start_btn_interact(Elements *self) {}
void Start_btn_draw(Elements *self) {
  Start_btn *btn = ((Start_btn *)(self->pDerivedObj));
  al_draw_text(btn->font, btn->color, btn->x1 + 150, btn->y1 + 25,
               ALLEGRO_ALIGN_CENTRE, "Start");
  al_draw_rounded_rectangle(btn->x1, btn->y1, btn->x2, btn->y2, 10, 10,
                            btn->color, 5);
}
void Start_btn_destory(Elements *self) {
  Start_btn *Obj = ((Start_btn *)(self->pDerivedObj));
  free(Obj->hitbox);
  free(Obj);
  free(self);
}
