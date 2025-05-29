#include "Tetris_board.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "../scene/sceneManager.h"  // for scene variable
#include "../shapes/Rectangle.h"
Elements* New_Tetris_board(int label) {
    Tetris_board* pDerivedObj = (Tetris_board*)malloc(sizeof(Tetris_board));
    Elements* pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->x1 = (double)WIDTH / 2 - 700;
    pDerivedObj->y1 = (double)HEIGHT / 2 - 400;
    pDerivedObj->x2 = (double)WIDTH / 2 - 300;
    pDerivedObj->y2 = (double)HEIGHT / 2 + 400;
    pDerivedObj->side_len = 40;
    pDerivedObj->gravity = 1;
    pDerivedObj->gravity_increase_factor = 0.1;
    pDerivedObj->timer = 0;
    pDerivedObj->pieces = 0;
    pDerivedObj->attack = 0;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x1, pDerivedObj->y1,
                                        pDerivedObj->x2, pDerivedObj->y2);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            pDerivedObj->occupied[i][j] = false;
            pDerivedObj->color_map[i][j] = al_map_rgb(0, 0, 0);
        }
    }
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Tetris_board_update;
    pObj->Interact = Tetris_board_interact;
    pObj->Draw = Tetris_board_draw;
    pObj->Destroy = Tetris_board_destory;
    return pObj;
}
void Tetris_board_update(Elements* self) {
    Tetris_board* board = ((Tetris_board*)(self->pDerivedObj));
    board->timer = (board->timer + 1) % 1500;
}
void Tetris_board_interact(Elements* self) {
    /* Tetris_board* board = ((Tetris_board*)(self->pDerivedObj)); */
    /* ElementVec labelelem = _Get_label_elements(scene, Tetrimino_L); */
    /* Tetrimino* tetrimino = ((Tetrimino*)(labelelem.arr[0]->pDerivedObj)); */
}
void Tetris_board_draw(Elements* self) {
    Tetris_board* board = ((Tetris_board*)(self->pDerivedObj));
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_rectangle(board->x1, board->y1, board->x2, board->y2,
                      al_map_rgb(255, 255, 255), 2);
    int side_len = board->side_len;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 20; j++) {
            al_draw_rectangle(
                board->x1 + i * side_len, board->y1 + j * side_len,
                board->x1 + (i + 1) * side_len, board->y1 + (j + 1) * side_len,
                al_map_rgb(100, 100, 100), 2);
        }
    }
}
void Tetris_board_destory(Elements* self) {
    Tetris_board* Obj = ((Tetris_board*)(self->pDerivedObj));
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
