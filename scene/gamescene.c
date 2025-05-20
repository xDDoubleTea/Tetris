#include "gamescene.h"

#include <allegro5/allegro_audio.h>

#include "../element/Tetris_board.h"
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
    _Register_elements(pObj, New_Tetris_board(Tetris_board_L));
    _Register_elements(pObj, New_Tetrimino(Tetrimino_L));
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
    if (key_state[ALLEGRO_KEY_ESCAPE]) {
        self->scene_end = true;
        window = 0;
    }
}
void game_scene_draw(Scene* self) {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    /* GameScene* gs = ((GameScene*)(self->pDerivedObj)); */
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
