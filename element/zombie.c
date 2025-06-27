#include "zombie.h"

#include "../scene/gamescene.h"
#include "../scene/scene.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap_draw.h>
Elements *New_zombie(int label) {
  Zombie *pDerivedObj = (Zombie *)malloc(sizeof(Zombie));
  Elements *pObj = New_Elements(label);
  GameScene *gs = (GameScene *)(scene->pDerivedObj);
  // setting derived object member
  pDerivedObj->color = al_map_rgb(255, 0, 0);
  // setting derived object function
  pObj->pDerivedObj = pDerivedObj;
  pDerivedObj->w = 300;
  pDerivedObj->h = 300;
  pDerivedObj->x1 = WIDTH - 100;
  pDerivedObj->x2 = WIDTH + pDerivedObj->w;
  pDerivedObj->y1 = 780;
  pDerivedObj->y2 = 1080;
  pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x1, pDerivedObj->y1,
                                      pDerivedObj->x2, pDerivedObj->y2);
  pDerivedObj->walkspeed = 5;
  pDerivedObj->walktimer = 0;
  pDerivedObj->stop_walk = false;
  pDerivedObj->font = al_load_ttf_font("assests/font/Consolas.ttf", 24, 0);
  pDerivedObj->hp = 10;
  pObj->Update = zombie_update;
  pObj->Interact = zombie_interact;
  pObj->Draw = zombie_draw;
  pObj->Destroy = zombie_destory;
  return pObj;
}
void zombie_update(Elements *self) {
  Zombie *zombie = ((Zombie *)(self->pDerivedObj));
  Shape *hitbox = zombie->hitbox;
  zombie->walktimer++;
  if (!(zombie->walktimer % ((int)FPS / zombie->walkspeed)) &&
      !zombie->stop_walk) {
    zombie->x1 -= 10;
    zombie->x2 -= 10;
    hitbox->update_center_x(hitbox, -10);
  }
  if (zombie->hp <= 0) {
    self->dele = true;
  }
}
void zombie_interact(Elements *self) {}
void zombie_draw(Elements *self) {
  Zombie *zombie = (Zombie *)self->pDerivedObj;
  GameScene *gs = (GameScene *)(scene->pDerivedObj);
  ALLEGRO_BITMAP *frame = algif_get_bitmap(gs->zombie_gif, al_get_time());
  if (frame == NULL)
    return;
  al_draw_scaled_bitmap(frame, 0, 0, zombie->w, zombie->h, zombie->x1,
                        zombie->y1, -zombie->w, zombie->h, 0);
  al_draw_textf(zombie->font, zombie->color, zombie->x1 - (float)zombie->w / 2,
                zombie->y1, ALLEGRO_ALIGN_CENTER, "%d", zombie->hp);
}
void zombie_destory(Elements *self) {
  Zombie *Obj = ((Zombie *)(self->pDerivedObj));
  free(Obj->hitbox);
  free(Obj);
  free(self);
}
