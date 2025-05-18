#include "menu.h"

#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
/*
   [Menu function]
*/
Scene* New_Menu(int label) {
    Menu* pDerivedObj = (Menu*)malloc(sizeof(Menu));
    Scene* pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->font = al_create_builtin_font();
    // Load sound
    pDerivedObj->song = al_load_sample("../assests/F1.mp3");
    al_reserve_samples(20);
    pDerivedObj->title_x = WIDTH / 2;
    pDerivedObj->title_y = HEIGHT / 2;
    // Loop the song until the display closes
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    al_set_sample_instance_playmode(pDerivedObj->sample_instance,
                                    ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance,
                                       al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 1);
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Update = menu_update;
    pObj->Draw = menu_draw;
    pObj->Destroy = menu_destroy;
    return pObj;
}
void menu_update(Scene* self) {
    if (key_state[ALLEGRO_KEY_ENTER]) {
        self->scene_end = true;
        window = -1;
        printf("Enter pressed\n");
    }
    return;
}
void menu_draw(Scene* self) {
    Menu* Obj = ((Menu*)(self->pDerivedObj));
    al_draw_text(Obj->font, al_map_rgb(255, 255, 255), Obj->title_x,
                 Obj->title_y, ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to start");
    al_draw_rectangle(Obj->title_x - 150, Obj->title_y - 30, Obj->title_x + 150,
                      Obj->title_y + 30, al_map_rgb(255, 255, 255), 0);
}
void menu_destroy(Scene* self) {
    Menu* Obj = ((Menu*)(self->pDerivedObj));
    al_destroy_font(Obj->font);
    free(Obj);
    free(self);
}
