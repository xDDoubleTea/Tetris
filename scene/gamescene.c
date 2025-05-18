#include "gamescene.h"

#include <allegro5/allegro_audio.h>

#include "../element/element.h"
/*
   [GameScene function]
*/
Scene* New_GameScene(int label) {
    GameScene* pDerivedObj = (GameScene*)malloc(sizeof(GameScene));
    Scene* pObj = New_Scene(label);
    // setting derived object member
    pObj->pDerivedObj = pDerivedObj;
    // register element
    // setting derived object function
    pObj->Update = game_scene_update;
    pObj->Draw = game_scene_draw;
    pObj->Destroy = game_scene_destroy;
    return pObj;
}
void game_scene_update(Scene* self) {
    // update every element
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
}
void game_scene_draw(Scene* self) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    GameScene* gs = ((GameScene*)(self->pDerivedObj));
    al_draw_bitmap(gs->background, 0, 0, 0);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++) {
        Elements* ele = allEle.arr[i];
        ele->Draw(ele);
    }
}
void game_scene_destroy(Scene* self) {
    GameScene* Obj = ((GameScene*)(self->pDerivedObj));
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++) {
        Elements* ele = allEle.arr[i];
        ele->Destroy(ele);
    }
    free(Obj);
    free(self);
}
