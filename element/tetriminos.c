#include "tetriminos.h"

#include <allegro5/allegro5.h>
#include <allegro5/keycodes.h>
#include <stdlib.h>
#include <time.h>

#include "tetrimino_shape.h"

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
    int type = rand() % 7;
    pDerivedObj->block_type = type;
    pDerivedObj->rotation = 0;
    pDerivedObj->coord_x = 4;
    pDerivedObj->coord_y = -3;
    pDerivedObj->timer = 0;
    pDerivedObj->color =
        al_map_rgb(tetrimino_colors[type][0], tetrimino_colors[type][1],
                   tetrimino_colors[type][2]);
    pDerivedObj->hitbox = New_Rectangle(0, 0, 0, 0);
    // setting derived object function
    pDerivedObj->move_lock = false;
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Tetrimino_update;
    pObj->Interact = Tetrimino_interact;
    pObj->Draw = Tetrimino_draw;
    pObj->Destroy = Tetrimino_destory;
    return pObj;
}
void Tetrimino_update(Elements* self) {
    Tetrimino* tetrimino = ((Tetrimino*)(self->pDerivedObj));
    if (key_state[ALLEGRO_KEY_LEFT] && !tetrimino->move_lock) {
        tetrimino->coord_x -= 1;
        tetrimino->move_lock = true;
    } else if (key_state[ALLEGRO_KEY_RIGHT] && !tetrimino->move_lock) {
        tetrimino->coord_x += 1;
        tetrimino->move_lock = true;
    } else if (!key_state[ALLEGRO_KEY_LEFT] && !key_state[ALLEGRO_KEY_RIGHT]) {
        tetrimino->move_lock = false;
    }
    tetrimino->timer += 1;
    if (!(tetrimino->timer % 90)) {
        tetrimino->coord_y += 1;
        tetrimino->timer = 0;
    }
}
void Tetrimino_interact(Elements* self) {}
void Tetrimino_draw(Elements* self) {
    ElementVec labelelem = _Get_label_elements(scene, Tetris_board_L);
    Tetrimino* tetrimino = ((Tetrimino*)(self->pDerivedObj));
    Tetris_board* board = ((Tetris_board*)(labelelem.arr[0]->pDerivedObj));
    int side_len = board->side_len;
    // TODO: Tackle the problem where the block spawns out of the screen.
    const TetriminoShape* cur_block_shape =
        tetrimino_shapes[tetrimino->block_type][tetrimino->rotation];
    for (int i = 0; i < 4; i++) {
        int block_x = tetrimino->coord_x + cur_block_shape[i].x;
        int block_y = tetrimino->coord_y + cur_block_shape[i].y;

        float screen_x1 = board->x1 + block_x * side_len;
        float screen_y1 = board->y1 + block_y * side_len;
        float screen_x2 = screen_x1 + side_len;
        float screen_y2 = screen_y1 + side_len;

        al_draw_filled_rectangle(screen_x1, screen_y1, screen_x2, screen_y2,
                                 tetrimino->color);
    }
}

void Tetrimino_destory(Elements* self) {
    Tetrimino* Obj = ((Tetrimino*)(self->pDerivedObj));
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
