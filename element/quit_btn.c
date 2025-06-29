#include "quit_btn.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
Elements *New_Quit_btn(int label) {
  Quit_btn *pDerivedObj = (Quit_btn *)malloc(sizeof(Quit_btn));
  Elements *pObj = New_Elements(label);
  // setting derived object member
  pDerivedObj->x1 = (double)WIDTH / 2 - 200;
  pDerivedObj->y1 = (double)HEIGHT / 2 + 100;
  pDerivedObj->x2 = (double)WIDTH / 2 + 200;
  pDerivedObj->y2 = (double)HEIGHT / 2 + 200;
  pDerivedObj->color = al_map_rgb(255, 0, 0);
  pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x1, pDerivedObj->y1,
                                      pDerivedObj->x2, pDerivedObj->y2);
  pDerivedObj->selected = false;
  pDerivedObj->font = al_load_ttf_font("assets/font/hun2.ttf", 48, 0);
  // setting derived object function
  pObj->pDerivedObj = pDerivedObj;
  pObj->Update = Quit_btn_update;
  pObj->Interact = Quit_btn_interact;
  pObj->Draw = Quit_btn_draw;
  pObj->Destroy = Quit_btn_destory;
  return pObj;
}
void Quit_btn_update(Elements *self) {
  Quit_btn *btn = ((Quit_btn *)(self->pDerivedObj));
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
    window = -1;
  }
}
void Quit_btn_interact(Elements *self) {}
void Quit_btn_draw(Elements *self) {
  Quit_btn *btn = ((Quit_btn *)(self->pDerivedObj));
  al_draw_text(btn->font, btn->color, btn->x1 + 200, btn->y1 + 25,
               ALLEGRO_ALIGN_CENTRE, "QUIT");
  al_draw_rounded_rectangle(btn->x1, btn->y1, btn->x2, btn->y2, 10, 10,
                            btn->color, 5);
}
void Quit_btn_destory(Elements *self) {
  Quit_btn *Obj = ((Quit_btn *)(self->pDerivedObj));
  free(Obj->hitbox);
  free(Obj);
  free(self);
}
