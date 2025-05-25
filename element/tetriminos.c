#include "tetriminos.h"

#include <allegro5/allegro5.h>

const unsigned char gray[3] = {100, 100, 100};
const unsigned char o_color[3] = {255, 255, 0};
const unsigned char s_color[3] = {187, 235, 102};
const unsigned char z_color[3] = {194, 62, 71};
const unsigned char t_color[3] = {177, 77, 166};
const unsigned char i_color[3] = {78, 207, 158};
const unsigned char j_color[3] = {96, 78, 181};
const unsigned char l_color[3] = {199, 177, 67};
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "../scene/sceneManager.h"  // for scene variable
#include "../shapes/Rectangle.h"
#include "Tetris_board.h"

Elements* New_Tetrimino(int label) {
    Tetrimino* pDerivedObj = (Tetrimino*)malloc(sizeof(Tetrimino));
    Elements* pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->block_type = 'O';
    pDerivedObj->rotation = 0;
    pDerivedObj->coord_x = 4;
    pDerivedObj->coord_y = -3;
    pDerivedObj->color = al_map_rgb(o_color[0], o_color[1], o_color[2]);
    pDerivedObj->hitbox = New_Rectangle(0, 0, 0, 0);
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Tetrimino_update;
    pObj->Interact = Tetrimino_interact;
    pObj->Draw = Tetrimino_draw;
    pObj->Destroy = Tetrimino_destory;
    return pObj;
}
void Tetrimino_update(Elements* self) {}
void Tetrimino_interact(Elements* self) {}
void Tetrimino_draw(Elements* self) {
    ElementVec labelelem = _Get_label_elements(scene, Tetris_board_L);
    Tetrimino* tetrimino = ((Tetrimino*)(self->pDerivedObj));
    Tetris_board* board = ((Tetris_board*)(labelelem.arr[0]->pDerivedObj));
    int side_len = board->side_len;
    switch (tetrimino->block_type) {
        case 'O': {
            al_draw_filled_rectangle(
                board->x1 + tetrimino->coord_x * side_len,
                board->y1 + tetrimino->coord_y * side_len,
                board->x1 + (tetrimino->coord_x + 1) * side_len,
                board->y1 + (tetrimino->coord_y + 1) * side_len,
                tetrimino->color);
            al_draw_filled_rectangle(
                board->x1 + (tetrimino->coord_x + 1) * side_len,
                board->y1 + tetrimino->coord_y * side_len,
                board->x1 + (tetrimino->coord_x + 2) * side_len,
                board->y1 + (tetrimino->coord_y + 1) * side_len,
                tetrimino->color);
            al_draw_filled_rectangle(
                board->x1 + tetrimino->coord_x * side_len,
                board->y1 + (tetrimino->coord_y + 1) * side_len,
                board->x1 + (tetrimino->coord_x + 1) * side_len,
                board->y1 + (tetrimino->coord_y + 2) * side_len,
                tetrimino->color);
            al_draw_filled_rectangle(
                board->x1 + (tetrimino->coord_x + 1) * side_len,
                board->y1 + (tetrimino->coord_y + 1) * side_len,
                board->x1 + (tetrimino->coord_x + 2) * side_len,
                board->y1 + (tetrimino->coord_y + 2) * side_len,
                tetrimino->color);
        }
    }
}
void Tetrimino_destory(Elements* self) {
    Tetrimino* Obj = ((Tetrimino*)(self->pDerivedObj));
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
