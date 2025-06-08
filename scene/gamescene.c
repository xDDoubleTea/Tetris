#include "gamescene.h"

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>

#include "../element/Tetris_board.h"
#include "../element/element.h"
#include "../element/tetrimino_shape.h"
#include "scene.h"
/*
   [GameScene function]
*/

void gen_tetr_7_bag(Scene *pObj, int now_pieces) {
  int arr[7] = {TETR_O, TETR_S, TETR_Z, TETR_T, TETR_I, TETR_J, TETR_L};
  // Randomly shuffle the array
  for (int i = 6; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
  for (int i = 0; i < 7; i++) {
    fprintf(stderr, "%d", arr[i]);
    _Register_elements(pObj,
                       New_Tetrimino(Tetrimino_L, arr[i], i + now_pieces));
  }
  fprintf(stderr, "\n");
}

Scene *New_GameScene(int label) {
  GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
  Scene *pObj = New_Scene(label);
  // setting derived object member
  pObj->pDerivedObj = pDerivedObj;
  // register element
  _Register_elements(pObj, New_Tetris_board(Tetris_board_L));
  gen_tetr_7_bag(pObj, 0);
  // setting derived object function
  pObj->Update = game_scene_update;
  pObj->Draw = game_scene_draw;
  pObj->Destroy = game_scene_destroy;
  return pObj;
}
void game_scene_update(Scene *self) {
  // update every element
  ElementVec boardele = _Get_label_elements(self, Tetris_board_L);
  Tetris_board *board = (Tetris_board *)boardele.arr[0]->pDerivedObj;
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
  if (key_state[ALLEGRO_KEY_ESCAPE]) {
    self->scene_end = true;
    window = 0;
  }
  if (board->pieces_in_queue <= 6) {
    gen_tetr_7_bag(self, board->pieces_in_queue);
    board->pieces_in_queue += 7;
  }
}
void game_scene_draw(Scene *self) {
  al_clear_to_color(al_map_rgb(0, 0, 0));
  /* GameScene* gs = ((GameScene*)(self->pDerivedObj)); */
  ElementVec allEle = _Get_all_elements(self);
  for (int i = 0; i < allEle.len; i++) {
    Elements *ele = allEle.arr[i];
    ele->Draw(ele);
  }
}
void game_scene_destroy(Scene *self) {
  GameScene *Obj = ((GameScene *)(self->pDerivedObj));
  ElementVec allEle = _Get_all_elements(self);
  for (int i = 0; i < allEle.len; i++) {
    Elements *ele = allEle.arr[i];
    ele->Destroy(ele);
  }
  free(Obj);
  free(self);
}
