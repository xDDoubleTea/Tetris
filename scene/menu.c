#include "menu.h"

#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>

#include "../element/start_btn.h"
#include "gamescene.h"
/*
   [Menu function]
*/
Scene* New_Menu(int label) {
    Menu* pDerivedObj = (Menu*)malloc(sizeof(Menu));
    Scene* pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->font = al_load_ttf_font("assests/font/pirulen.ttf", 10, 0);
    // Load sound
    pDerivedObj->song = al_load_sample("assests/MainMenu.wav");
    al_reserve_samples(16);
    pDerivedObj->title_x = WIDTH / 2;
    pDerivedObj->title_y = HEIGHT / 2;
    // register elements
    _Register_elements(pObj, New_Start_btn(Start_btn_L));
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
    pObj->Update = menu_update;
    pObj->Draw = menu_draw;
    pObj->Destroy = menu_destroy;
    return pObj;
}
void menu_update(Scene* self) {
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++) {
        Elements* ele = allEle.arr[i];
        ele->Update(ele);
    }
    // run interact for every element
    for (int i = 0; i < allEle.len; i++) {
        Elements* ele = allEle.arr[i];
        ele->Interact(ele);
    }
    // remove element
    for (int i = 0; i < allEle.len; i++) {
        Elements* ele = allEle.arr[i];
        if (ele->dele) _Remove_elements(self, ele);
    }
    ElementVec labelele = _Get_label_elements(self, Start_btn_L);
    Start_btn* stbtn = (Start_btn*)(labelele.arr[0]->pDerivedObj);
    ALLEGRO_MOUSE_STATE mouse_state;
    al_get_mouse_state(&mouse_state);
    if (stbtn->selected && al_mouse_button_down(&mouse_state, 1)) {
        self->scene_end = true;
        window = 1;
    }
    if (key_state[ALLEGRO_KEY_ESCAPE]) {
        self->scene_end = true;
        window = -1;
    }
    return;
}
void menu_draw(Scene* self) {
    Menu* Obj = ((Menu*)(self->pDerivedObj));
    al_draw_text(Obj->font, al_map_rgb(255, 255, 255), Obj->title_x,
                 Obj->title_y + 175, ALLEGRO_ALIGN_CENTRE,
                 "Press 'ESC' to quit");
    al_draw_rectangle(Obj->title_x - 150, Obj->title_y + 150,
                      Obj->title_x + 150, Obj->title_y + 200,
                      al_map_rgb(255, 255, 255), 0);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++) {
        Elements* ele = allEle.arr[i];
        ele->Draw(ele);
    }
    if (!al_get_sample_instance_playing(Obj->sample_instance)) {
        al_play_sample_instance(Obj->sample_instance);
    }
}
void menu_destroy(Scene* self) {
    Menu* Obj = ((Menu*)(self->pDerivedObj));
    al_destroy_font(Obj->font);
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    free(Obj);
    free(self);
}
