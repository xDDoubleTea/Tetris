#include "tetriminos.h"

#include <allegro5/allegro5.h>
#include <allegro5/keycodes.h>
#include <stdlib.h>

#include "element.h"
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

Elements* New_Tetrimino(int label, int type, int pos_in_queue) {
    Tetrimino* pDerivedObj = (Tetrimino*)malloc(sizeof(Tetrimino));
    Elements* pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->block_type = type;
    pDerivedObj->pos_in_queue = pos_in_queue;
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
    if (tetrimino->pos_in_queue) {
        return;
    }
    if (key_state[ALLEGRO_KEY_LEFT] && !tetrimino->move_lock) {
        int temp_check_coord_x = tetrimino->coord_x - 1;
        for (int i = 0; i < 4; ++i) {
            if (tetrimino_shapes[tetrimino->block_type][tetrimino->rotation][i]
                        .x +
                    temp_check_coord_x <
                0) {
                tetrimino->move_lock = true;
                return;
            }
        }
        tetrimino->coord_x--;
        tetrimino->move_lock = true;
    } else if (key_state[ALLEGRO_KEY_RIGHT] && !tetrimino->move_lock) {
        int temp_check_coord_x = tetrimino->coord_x + 1;
        for (int i = 0; i < 4; ++i) {
            if (tetrimino_shapes[tetrimino->block_type][tetrimino->rotation][i]
                        .x +
                    temp_check_coord_x >=
                10) {
                tetrimino->move_lock = true;
                return;
            }
        }
        tetrimino->coord_x++;
        tetrimino->move_lock = true;
    } else if (!key_state[ALLEGRO_KEY_LEFT] && !key_state[ALLEGRO_KEY_RIGHT]) {
        tetrimino->move_lock = false;
    }
    tetrimino->timer++;
    if (!(tetrimino->timer % (int)(FPS * 0.1))) {
        int temp_check_coord_y = tetrimino->coord_y + 1;
        int lowest_y = 0;
        for (int i = 0; i < 4; ++i) {
            lowest_y =
                tetrimino_shapes[tetrimino->block_type][tetrimino->rotation][i]
                            .y > lowest_y
                    ? tetrimino_shapes[tetrimino->block_type]
                                      [tetrimino->rotation][i]
                                          .y
                    : lowest_y;
        }
        if (lowest_y + temp_check_coord_y >= 20) {
            ElementVec labelelem = _Get_label_elements(scene, Tetrimino_L);

            for (int i = 0; i < labelelem.len; ++i) {
                if (!i) {
                    labelelem.arr[i]->dele = true;
                }
                Tetrimino* tetrimino =
                    (Tetrimino*)labelelem.arr[i]->pDerivedObj;
                tetrimino->pos_in_queue--;
            }
            labelelem = _Get_label_elements(scene, Tetris_board_L);
            Tetris_board* board = labelelem.arr[0]->pDerivedObj;
            board->pieces_in_queue--;
            board->pieces++;
            return;
        }
        tetrimino->coord_y++;
        tetrimino->timer = 0;
    }
}
void Tetrimino_interact(Elements* self) {}
void Tetrimino_draw(Elements* self) {
    ElementVec labelelem = _Get_label_elements(scene, Tetris_board_L);
    Tetrimino* tetrimino = ((Tetrimino*)(self->pDerivedObj));
    if (tetrimino->pos_in_queue) {
        return;
    }
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
