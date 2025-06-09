
#include "settings.h"

#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>

#include "../element/quit_btn.h"
#include "../element/settings_btn.h"
#include "../element/start_btn.h"
#include "gamescene.h"
#include "scene.h"
/*
   [Menu function]
*/
Scene *New_Settings(int label) {
  Settings *pDerivedObj = (Settings *)malloc(sizeof(Settings));
  Scene *pObj = New_Scene(label);
  // setting derived object member
  pDerivedObj->font = al_load_ttf_font("assests/font/hun2.ttf", 24, 0);
  // Load sound
  pDerivedObj->song = al_load_sample("assests/MainMenu.wav");
  al_reserve_samples(16);
  pDerivedObj->title_x = WIDTH / 2;
  pDerivedObj->title_y = HEIGHT / 2;
  // register elements
  _Register_elements(pObj, New_Start_btn(Start_btn_L));
  _Register_elements(pObj, New_Settings_btn(Settings_btn_L));
  _Register_elements(pObj, New_Quit_btn(Quit_btn_L));
  // Loop the song until the display closes
  pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
  al_set_sample_instance_playmode(pDerivedObj->sample_instance,
                                  ALLEGRO_PLAYMODE_LOOP);
  al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance,
                                     al_get_default_mixer());
  // set the volume of instance
  al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.2);
  pObj->pDerivedObj = pDerivedObj;
  // setting derived object function
  pObj->Update = settings_update;
  pObj->Draw = settings_draw;
  pObj->Destroy = settings_destroy;
  return pObj;
}
void settings_update(Scene *self) {
  ElementVec allEle = _Get_all_elements(self);
  for (int i = 0; i < allEle.len; i++) {
    Elements *ele = allEle.arr[i];
    ele->Update(ele);
  }
  // run interact for every element
  for (int i = 0; i < allEle.len; i++) {
    Elements *ele = allEle.arr[i];
    ele->Interact(ele);
  }
  // remove element
  for (int i = 0; i < allEle.len; i++) {
    Elements *ele = allEle.arr[i];
    if (ele->dele)
      _Remove_elements(self, ele);
  }
  /* if (key_state[ALLEGRO_KEY_ESCAPE]) { */
  /*     self->scene_end = true; */
  /*     window = -1; */
  /* } */
  return;
}
void settings_draw(Scene *self) {
  Settings *Obj = ((Settings *)(self->pDerivedObj));
  // al_draw_text(Obj->font, al_map_rgb(255, 0, 0), Obj->title_x,
  //              Obj->title_y + 175, ALLEGRO_ALIGN_CENTRE, "Quit");
  // al_draw_rectangle(Obj->title_x - 150, Obj->title_y + 150, Obj->title_x +
  // 150,
  //                   Obj->title_y + 200, al_map_rgb(255, 255, 255), 0);
  ElementVec allEle = _Get_all_elements(self);
  for (int i = 0; i < allEle.len; i++) {
    Elements *ele = allEle.arr[i];
    ele->Draw(ele);
  }
  if (!al_get_sample_instance_playing(Obj->sample_instance)) {
    al_play_sample_instance(Obj->sample_instance);
  }
}
void settings_destroy(Scene *self) {
  Settings *Obj = ((Settings *)(self->pDerivedObj));
  al_destroy_font(Obj->font);
  al_destroy_sample(Obj->song);
  al_destroy_sample_instance(Obj->sample_instance);
  free(Obj);
  free(self);
}
