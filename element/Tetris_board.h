#ifndef TETRIS_BOARD_H_INCLUDED
#define TETRIS_BOARD_H_INCLUDED
#include <allegro5/allegro_font.h>

#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
#include "element.h"
#include "tetriminos.h"
#include "zombie.h"
void drop_tetrimino();
typedef struct _Tetris_board {
  double x1, y1, x2, y2;
  int side_len;
  bool occupied[10][1000];
  bool garbage_line[100];
  ALLEGRO_COLOR color_map[10][1000];
  ALLEGRO_FONT *font;
  ALLEGRO_FONT *font_small;
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
  int in_combo;
  int in_b2b;
  bool t_spun;
  bool game_over;
  int timer;

  char cancel_type[32];
  int board_height;

  int damage_timer;
  int garbage_queue;
  int garbage_timer;
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
  // Garbage queue display
  double gb_q_x, gb_q_y;
  // Back to Back display
  double b2b_x, b2b_y;
  // Cancel type display
  double cancel_x, cancel_y;
  // T spin display
  double tspin_x, tspin_y;
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
void clear_line(Tetris_board *board, Tetrimino *tetrimino);
void recieve_damage(Tetris_board *board);
void deal_damage(Tetris_board *board, int damage);
#endif
