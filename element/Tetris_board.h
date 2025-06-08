#ifndef TETRIS_BOARD_H_INCLUDED
#define TETRIS_BOARD_H_INCLUDED
#include <allegro5/allegro_font.h>

#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
#include "element.h"
#include "tetriminos.h"
void drop_tetrimino();
typedef struct _Tetris_board {
  double x1, y1, x2, y2;
  int side_len;
  bool occupied[10][20];
  int highest_occupied[10];
  ALLEGRO_COLOR color_map[10][20];
  ALLEGRO_FONT *font;
  ALLEGRO_COLOR font_color;
  double gravity;
  double gravity_increase_factor;
  int pieces;
  int attack;
  int pieces_in_queue;
  bool hold_piece;
  int hold_piece_type;
  bool hold_lock;
  bool hard_drop_lock;
  bool game_over;
  int timer;

  // pps display coord
  double pps_x, pps_y;
  // apm display coord
  double apm_x, apm_y;
  // pieces display coord
  double pieces_x, pieces_y;
  // time display coord
  double time_x, time_y;
  // tetriminos next queue display
  double next_x[5], next_y[5];
  // tetrimino hold display
  double hold_x, hold_y;
  Shape *hitbox;

} Tetris_board;
Elements *New_Tetris_board(int label);
void update_highest_occupied(Tetris_board *board);
void Tetris_board_update(Elements *self);
void Tetris_board_interact(Elements *self);
void Tetris_board_draw(Elements *self);
void Tetris_board_destory(Elements *self);
int find_pos_0_tet();
int find_pos_1_tet();
#endif
