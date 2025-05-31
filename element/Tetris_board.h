#ifndef TETRIS_BOARD_H_INCLUDED
#define TETRIS_BOARD_H_INCLUDED
#include <allegro5/allegro_font.h>

#include "../scene/gamescene.h"  // for element label
#include "../shapes/Shape.h"
#include "element.h"
#include "tetriminos.h"
typedef struct _Tetris_board {
    double x1, y1, x2, y2;
    int side_len;
    bool occupied[10][20];
    ALLEGRO_COLOR color_map[10][20];
    ALLEGRO_FONT* font;
    ALLEGRO_COLOR font_color;
    int gravity;
    double gravity_increase_factor;
    int pieces;
    int attack;
    int pieces_in_queue;
    bool hard_drop_lock;
    int timer;

    // pps display coord
    double pps_x, pps_y;
    // apm display coord
    double apm_x, apm_y;
    // pieces display coord
    double pieces_x, pieces_y;
    // time display coord
    double time_x, time_y;
    Shape* hitbox;
} Tetris_board;
Elements* New_Tetris_board(int label);
void Tetris_board_update(Elements* self);
void Tetris_board_interact(Elements* self);
void Tetris_board_draw(Elements* self);
void Tetris_board_destory(Elements* self);

#endif
