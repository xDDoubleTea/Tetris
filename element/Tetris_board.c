#include "Tetris_board.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/color.h>
#include <allegro5/keycodes.h>

#include "../scene/gamescene.h"
#include "../scene/sceneManager.h" // for scene variable
#include "../shapes/Rectangle.h"
#include "tetriminos.h"

int find_pos_0_tet() {
  ElementVec board_elem = _Get_label_elements(scene, Tetris_board_L);
  ElementVec labelelem = _Get_label_elements(scene, Tetrimino_L);
  Tetrimino *t;
  for (int i = 0; i < labelelem.len; ++i) {
    t = (Tetrimino *)labelelem.arr[i]->pDerivedObj;
    if (t->pos_in_queue == 0) {
      return i;
    }
  }
  return -1;
}
int find_pos_1_tet() {
  ElementVec board_elem = _Get_label_elements(scene, Tetris_board_L);
  ElementVec labelelem = _Get_label_elements(scene, Tetrimino_L);
  Tetrimino *t;
  for (int i = 0; i < labelelem.len; ++i) {
    t = (Tetrimino *)labelelem.arr[i]->pDerivedObj;
    if (t->pos_in_queue == 1) {
      return i;
    }
  }
  return -1;
}

void swap(void *a, void *b) {
  void *temp = a;
  a = b;
  b = temp;
}

void recieve_damage(Tetris_board *board) {
  if (board->game_over || board->garbage_queue == 0) {
    return;
  }
  int garbage = board->garbage_queue;
  if (garbage >= 8) {
    board->garbage_queue -= 8;
    garbage = 8;
  } else {
    board->garbage_queue = 0;
    board->garbage_timer = 0;
  }
  int garbage_col = rand() % 10;
  for (int k = 0; k < garbage; ++k) {
    for (int i = 0; i < 19; ++i) {
      for (int j = 0; j < 10; ++j) {
        board->occupied[j][i] = board->occupied[j][i + 1];
        board->color_map[j][i] = board->color_map[j][i + 1];
      }
    }
  }
  for (int i = 0; i < garbage; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (j == garbage_col) {
        board->occupied[j][19 - i] = false;
        board->color_map[j][19 - i] = al_map_rgb(0, 0, 0);
        continue;
      }
      board->occupied[j][19 - i] = true;
      board->color_map[j][19 - i] = al_map_rgb(gray[0], gray[1], gray[2]);
    }
  }
}

void recieve_garbage(Tetris_board *board, int garbage) {
  if (board->game_over) {
    return;
  }
  board->garbage_queue += garbage;
}

void update_highest_occupied(Tetris_board *board) {
  for (int i = 0; i < 10; ++i) {
    int highest = board->highest_occupied[i];
    for (int j = 0; j < 20; ++j) {
      if (board->occupied[i][j] && j < highest) {
        highest = j;
      }
    }
    board->highest_occupied[i] = highest;
  }
}
Elements *New_Tetris_board(int label) {
  Tetris_board *pDerivedObj = (Tetris_board *)malloc(sizeof(Tetris_board));
  Elements *pObj = New_Elements(label);
  // setting derived object member
  pDerivedObj->x1 = (double)WIDTH / 2 - 700;
  pDerivedObj->y1 = (double)HEIGHT / 2 - 400;
  pDerivedObj->x2 = (double)WIDTH / 2 - 300;
  pDerivedObj->y2 = (double)HEIGHT / 2 + 400;
  pDerivedObj->pps_x = pDerivedObj->x1 - 100;
  pDerivedObj->pps_y = (pDerivedObj->y2 + pDerivedObj->y1) / 2 + 100;
  pDerivedObj->apm_x = pDerivedObj->pps_x;
  pDerivedObj->apm_y = pDerivedObj->pps_y + 50;
  pDerivedObj->pieces_x = pDerivedObj->apm_x;
  pDerivedObj->pieces_y = pDerivedObj->apm_y + 50;
  pDerivedObj->time_x = pDerivedObj->apm_x;
  pDerivedObj->time_y = pDerivedObj->pieces_y + 50;
  pDerivedObj->font = al_load_ttf_font("assests/font/pirulen.ttf", 24, 0);
  pDerivedObj->side_len = 40;
  pDerivedObj->gravity = 0.5;
  pDerivedObj->garbage_queue = 0;
  pDerivedObj->in_b2b = 0;
  pDerivedObj->in_combo = 0;
  pDerivedObj->hold_x = pDerivedObj->x1 - 100;
  pDerivedObj->hold_y = pDerivedObj->y1;
  pDerivedObj->hold_piece = false;
  pDerivedObj->hold_lock = false;
  pDerivedObj->gb_q_x = pDerivedObj->y2;
  pDerivedObj->gb_q_y = pDerivedObj->x1;
  pDerivedObj->gravity_increase_factor = 0.1;
  pDerivedObj->timer = 0;
  pDerivedObj->font_color = al_map_rgb(255, 255, 255);
  pDerivedObj->pieces = 0;
  pDerivedObj->game_over = false;
  pDerivedObj->hard_drop_lock = false;
  pDerivedObj->attack = 0;
  pDerivedObj->pieces_in_queue = 7;
  pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x1, pDerivedObj->y1,
                                      pDerivedObj->x2, pDerivedObj->y2);
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      pDerivedObj->occupied[i][j] = false;
      pDerivedObj->color_map[i][j] = al_map_rgb(0, 0, 0);
    }
  }
  for (int i = 0; i < 5; i++) {
    pDerivedObj->next_x[i] = pDerivedObj->x2 + 100;
    pDerivedObj->next_y[i] = pDerivedObj->y1 + 50 + i * 150;
  }
  for (int i = 0; i < 10; ++i) {
    pDerivedObj->highest_occupied[i] = 20;
  }
  // setting derived object function
  pObj->pDerivedObj = pDerivedObj;
  pObj->Update = Tetris_board_update;
  pObj->Interact = Tetris_board_interact;
  pObj->Draw = Tetris_board_draw;
  pObj->Destroy = Tetris_board_destory;
  return pObj;
}
void Tetris_board_update(Elements *self) {
  Tetris_board *board = ((Tetris_board *)(self->pDerivedObj));
  if (board->game_over) {
    return;
  }
  board->timer++;
  ElementVec labelelem = _Get_label_elements(scene, Tetrimino_L);
  if (!board->hold_lock && !board->hold_piece && key_state[ALLEGRO_KEY_C]) {
    board->hold_lock = true;
    board->hold_piece = true;
    int pos_0_tet = find_pos_0_tet();
    Tetrimino *t = (Tetrimino *)labelelem.arr[pos_0_tet]->pDerivedObj;
    t->rotation = 0;
    t->held = true;
    for (int i = 0; i < labelelem.len; ++i) {
      t = (Tetrimino *)labelelem.arr[i]->pDerivedObj;
      t->pos_in_queue--;
    }
    board->pieces_in_queue--;
  }
  if (!board->hold_lock && board->hold_piece && key_state[ALLEGRO_KEY_C]) {
    board->hold_lock = true;
    Tetrimino *hold;
    Tetrimino *change;
    Tetrimino *t;
    int hold_idx = -1, change_idx = -1;
    for (int i = 0; i < labelelem.len; ++i) {
      t = (Tetrimino *)labelelem.arr[i]->pDerivedObj;
      if (t->held || t->pos_in_queue == -1) {
        hold_idx = i;
      }
      if (t->pos_in_queue == 0) {
        change_idx = i;
      }
    }
    if (hold_idx == -1) {
      board->game_over = true;
      return;
    }
    hold = (Tetrimino *)labelelem.arr[hold_idx]->pDerivedObj;
    labelelem.arr[hold_idx]->dele = false;
    change = (Tetrimino *)labelelem.arr[change_idx]->pDerivedObj;
    labelelem.arr[change_idx]->dele = false;
    hold->held = false;
    hold->dropped = false;
    change->held = true;
    change->dropped = false;
    board->hold_piece_type = change->block_type;
    change->rotation = 0;
    hold->pos_in_queue = 0;
    change->pos_in_queue = -1;
    hold->coord_x = 4;
    hold->coord_y = 0;
    change->coord_x = 4;
    change->coord_y = 0;
    return;
  }

  if (board->hold_lock && !key_state[ALLEGRO_KEY_C]) {
    board->hold_lock = false;
  }
  if (!board->hard_drop_lock && key_state[ALLEGRO_KEY_SPACE]) {
    board->hard_drop_lock = true;
    drop_tetrimino();
  } else if (board->hard_drop_lock && !key_state[ALLEGRO_KEY_SPACE]) {
    board->hard_drop_lock = false;
  }
  if (board->pieces_in_queue <= 6) {
    gen_tetr_7_bag(scene, board->pieces_in_queue);
    board->pieces_in_queue += 7;
  }
  // if (!(board->timer % (int)(FPS * 10)) && board->timer > 0) {
  //   recieve_garbage(board, 2);
  // }
  // if (board->garbage_queue) {
  //   board->garbage_timer++;
  // }
  // if (!(board->garbage_timer % (int)(FPS * 0.5)) && board->timer > 0) {
  //   // Garbage entry delays 0.5 seconds.
  //   recieve_damage(board);
  // }
}
void Tetris_board_interact(Elements *self) {
  // Tetris_board* board = ((Tetris_board*)(self->pDerivedObj));
  // ElementVec labelelem = _Get_label_elements(scene, Tetrimino_L);
  // Tetrimino* tetrimino = ((Tetrimino*)(labelelem.arr[0]->pDerivedObj));
}
void Tetris_board_draw(Elements *self) {
  Tetris_board *board = ((Tetris_board *)(self->pDerivedObj));
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_rectangle(board->x1, board->y1, board->x2, board->y2,
                    al_map_rgb(255, 255, 255), 2);

  double actual_time = (double)board->timer / FPS;
  if (!board->game_over) {
    al_draw_textf(board->font, board->font_color, board->pps_x, board->pps_y,
                  ALLEGRO_ALIGN_CENTER, "PPS:\n%.1lf",
                  board->pieces / actual_time);
    al_draw_textf(board->font, board->font_color, board->apm_x, board->apm_y,
                  ALLEGRO_ALIGN_CENTER, "APM:%.2lf",
                  board->attack * 60 / actual_time);
    al_draw_textf(board->font, board->font_color, board->pieces_x,
                  board->pieces_y, ALLEGRO_ALIGN_CENTER, "PIECES:%d",
                  board->pieces);
    al_draw_textf(board->font, board->font_color, board->time_x, board->time_y,
                  ALLEGRO_ALIGN_CENTER, "TIME:%02d:%02d", (int)actual_time / 60,
                  (int)actual_time % 60);
    al_draw_textf(board->font, board->font_color, board->hold_x, board->hold_y,
                  ALLEGRO_ALIGN_CENTER, "HOLD");
  }
  al_draw_textf(board->font, board->font_color, board->next_x[0],
                board->next_y[0] - 150, ALLEGRO_ALIGN_CENTER, "NEXT");

  int side_len = board->side_len;
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 20; j++) {
      al_draw_rectangle(board->x1 + i * side_len, board->y1 + j * side_len,
                        board->x1 + (i + 1) * side_len,
                        board->y1 + (j + 1) * side_len,
                        al_map_rgb(100, 100, 100), 2);

      al_draw_filled_rectangle(
          board->x1 + i * side_len, board->y1 + j * side_len,
          board->x1 + (i + 1) * side_len, board->y1 + (j + 1) * side_len,
          board->color_map[i][j]);
    }
  }
  if (board->game_over) {
    al_draw_textf(board->font, board->font_color, (board->x2 + board->x1) / 2,
                  (board->y1 + board->y2) / 2, ALLEGRO_ALIGN_CENTER,
                  "GAME OVER");
  }
}
void Tetris_board_destory(Elements *self) {
  Tetris_board *Obj = ((Tetris_board *)(self->pDerivedObj));
  free(Obj->hitbox);
  al_destroy_font(Obj->font);

  free(Obj);
  free(self);
}
