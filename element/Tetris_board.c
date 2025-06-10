#include "Tetris_board.h"

#include "../scene/gamescene.h"
#include "../scene/sceneManager.h" // for scene variable
#include "../shapes/Rectangle.h"
#include "tetrimino_shape.h"
#include "tetriminos.h"
#include "zombie.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/color.h>
#include <allegro5/keycodes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int find_pos_0_tet() {
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

bool perfect_clear(Tetris_board *board) {
  for (int i = 19; i >= 0; --i) {
    for (int j = 0; j < 10; ++j) {
      if (board->occupied[j][i]) {
        return false;
      }
    }
  }
  return true;
}
void clear_line(Tetris_board *board, Tetrimino *tetrimino) {
  int line_clear = 0;
  int rows_cleared[20] = {0};
  for (int i = 0; i < 20; ++i) {
    int clear = 1;
    for (int j = 0; j < 10; ++j) {
      if (!board->occupied[j][i]) {
        clear = 0;
        break;
      }
    }
    line_clear += clear;
    rows_cleared[i] += clear;
  }
  if (!line_clear) {
    board->in_combo = 0;
    recieve_damage(board);
  } else {
    GameScene *gamescene = (GameScene *)(scene->pDerivedObj);
    board->in_combo++;
    if (line_clear == 1) {
      snprintf(board->cancel_type, sizeof(board->cancel_type), "SINGLE");
    } else if (line_clear == 2) {
      snprintf(board->cancel_type, sizeof(board->cancel_type), "DOUBLE");
    } else if (line_clear == 3) {
      snprintf(board->cancel_type, sizeof(board->cancel_type), "TRIPLE");
    } else if (line_clear == 4) {
      snprintf(board->cancel_type, sizeof(board->cancel_type), "TETRIS");
    }
    int attack_calc = 0;
    al_play_sample_instance(
        gamescene->combo_instance[min(board->in_combo - 1, 15)]);
    if (tetrimino->did_t_spin && (tetrimino->tetrimino_last_oper == ROTATE ||
                                  tetrimino->tetrimino_last_oper == GRAVITY)) {
      board->in_b2b++;
      board->t_spun = true;
      attack_calc += line_clear * 2 * (1 + board->in_combo / 4);
    } else if (line_clear <= 3) {
      board->t_spun = false;
      if (line_clear == 1 && board->in_combo >= 2) {
        attack_calc += log(1 + 1.25 * board->in_combo);
      }
      attack_calc += (line_clear - 1) * (1 + board->in_combo / 4);
      if (board->in_b2b > 0) {
        al_play_sample_instance(gamescene->b2b_break_instance);
      }
      board->in_b2b = -1;
    } else if (line_clear == 4) {
      board->t_spun = false;
      board->in_b2b++;
      attack_calc += 4;
    }
    // Back to Back bonus
    if (board->in_b2b == 1) {
      al_play_sample_instance(gamescene->b2b_1_instance);
    } else if (board->in_b2b == 2 || board->in_b2b == 3) {
      al_play_sample_instance(gamescene->b2b_2_instance);
      attack_calc++;
    } else if (board->in_b2b > 3) {
      al_play_sample_instance(gamescene->b2b_3_instance);
      attack_calc += 2;
    }
    bool garbage_rewarded = false;
    for (int i = 19; i >= 0; --i) {
      if (rows_cleared[i]) {
        // Check if it is garbage line
        if (board->garbage_line[i] && !garbage_rewarded) {
          attack_calc += 1;
          garbage_rewarded = true;
        }
        for (int j = 0; j < 10; ++j) {
          board->occupied[j][i] = false;
          board->color_map[j][i] = al_map_rgb(0, 0, 0);
          for (int k = i; k > 0; --k) {
            board->occupied[j][k] = board->occupied[j][k - 1];
            board->color_map[j][k] = board->color_map[j][k - 1];
          }
        }
        for (int k = i; k > 0; --k)
          rows_cleared[k] = rows_cleared[k - 1];
        ++i;
        rows_cleared[0] = 0;
      }
    }
    if (perfect_clear(board)) {
      al_play_sample_instance(gamescene->all_clear_instance);
      attack_calc += 10;
    }

    deal_damage(board, attack_calc);
    board->attack += attack_calc;
  }
}

void deal_damage(Tetris_board *board, int damage) {
  ElementVec labelelem = _Get_label_elements(scene, Zombie_L);
  Zombie *zombie;
  for (int i = 0; i < labelelem.len; ++i) {
    zombie = (Zombie *)labelelem.arr[i]->pDerivedObj;
    zombie->hp -= damage;
    zombie->x1 += 20 * damage;
    zombie->x2 += 20 * damage;
    zombie->hitbox->update_center_x(zombie->hitbox, 20 * damage);
    zombie->stop_walk = false;
  }
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
  for (int i = 19; i > 19 - garbage; --i) {
    board->garbage_line[i] = true;
  }
}

void recieve_garbage(Tetris_board *board, int garbage) {
  if (board->game_over) {
    return;
  }
  board->garbage_queue += garbage;
}

Elements *New_Tetris_board(int label) {
  Tetris_board *pDerivedObj = (Tetris_board *)malloc(sizeof(Tetris_board));
  Elements *pObj = New_Elements(label);
  // setting derived object member
  pDerivedObj->x1 = (double)WIDTH / 2 - 700;
  pDerivedObj->y1 = (double)HEIGHT / 2 - 400;
  pDerivedObj->x2 = (double)WIDTH / 2 - 300;
  pDerivedObj->y2 = (double)HEIGHT / 2 + 400;
  pDerivedObj->pps_x = pDerivedObj->x1 - 150;
  pDerivedObj->pps_y = (pDerivedObj->y2 + pDerivedObj->y1) / 2 + 100;
  pDerivedObj->apm_x = pDerivedObj->pps_x;
  pDerivedObj->apm_y = pDerivedObj->pps_y + 50;
  pDerivedObj->pieces_x = pDerivedObj->apm_x;
  pDerivedObj->pieces_y = pDerivedObj->apm_y + 50;
  pDerivedObj->time_x = pDerivedObj->apm_x;
  pDerivedObj->time_y = pDerivedObj->pieces_y + 50;
  pDerivedObj->font = al_load_ttf_font("assests/font/hun2.ttf", 24, 0);
  pDerivedObj->font_small = al_load_ttf_font("assests/font/hun2.ttf", 16, 0);
  pDerivedObj->side_len = 40;
  pDerivedObj->gravity = 0.5;
  snprintf(pDerivedObj->cancel_type, sizeof(pDerivedObj->cancel_type), "None");
  pDerivedObj->t_spun = false;
  pDerivedObj->garbage_queue = 0;
  pDerivedObj->in_b2b = -1;
  pDerivedObj->board_height = 20;
  pDerivedObj->in_combo = 0;
  pDerivedObj->hold_x = pDerivedObj->x1 - 100;
  pDerivedObj->hold_y = pDerivedObj->y1;
  pDerivedObj->hold_piece = false;
  pDerivedObj->hold_lock = false;
  pDerivedObj->damage_timer = 0;
  pDerivedObj->gb_q_x = pDerivedObj->x1;
  pDerivedObj->gb_q_y = pDerivedObj->y2;
  pDerivedObj->b2b_x = pDerivedObj->hold_x;
  pDerivedObj->b2b_y = pDerivedObj->y1 + 200;
  pDerivedObj->cancel_x = pDerivedObj->b2b_x;
  pDerivedObj->cancel_y = pDerivedObj->b2b_y + 150;
  pDerivedObj->gravity_increase_factor = 0.1;
  pDerivedObj->timer = 0;
  pDerivedObj->font_color = al_map_rgb(255, 255, 255);
  pDerivedObj->pieces = 0;
  pDerivedObj->game_over = false;
  pDerivedObj->hard_drop_lock = false;
  pDerivedObj->attack = 0;

  pDerivedObj->pieces_in_queue = 7;
  pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x1, pDerivedObj->y1,
                                      pDerivedObj->x2 + 250, pDerivedObj->y2);
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 30; j++) {
      pDerivedObj->occupied[i][j] = false;
      pDerivedObj->color_map[i][j] = al_map_rgb(0, 0, 0);
    }
  }
  for (int i = 0; i < 5; i++) {
    pDerivedObj->next_x[i] = pDerivedObj->x2 + 100;
    pDerivedObj->next_y[i] = pDerivedObj->y1 + 50 + i * 150;
  }
  for (int i = 0; i < 30; ++i) {
    pDerivedObj->garbage_line[i] = false;
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
  GameScene *gs = (GameScene *)scene->pDerivedObj;
  if (board->game_over) {
    if (al_get_sample_instance_playing(gs->bgm_choice_sample_instance)) {
      al_set_sample_instance_playing(gs->bgm_choice_sample_instance, 0);
    }
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
    hold->coord_y = -1;
    change->coord_x = 4;
    change->coord_y = -1;
    return;
  }

  if (board->hold_lock && !key_state[ALLEGRO_KEY_C]) {
    board->hold_lock = false;
  }
  if (!board->hard_drop_lock && key_state[ALLEGRO_KEY_SPACE]) {
    board->hard_drop_lock = true;
    drop_tetrimino();
    GameScene *gamescene = (GameScene *)scene->pDerivedObj;
    if (al_get_sample_instance_playing(gamescene->hard_drop_sfx_instance)) {
      al_set_sample_instance_position(gamescene->hard_drop_sfx_instance, 0);
    }
    al_play_sample_instance(gamescene->hard_drop_sfx_instance);
  } else if (board->hard_drop_lock && !key_state[ALLEGRO_KEY_SPACE]) {
    board->hard_drop_lock = false;
  }
  if (board->pieces_in_queue <= 6) {
    gen_tetr_7_bag(scene, board->pieces_in_queue);
    board->pieces_in_queue += 7;
  }
}
void Tetris_board_interact(Elements *self) {
  Tetris_board *board = ((Tetris_board *)(self->pDerivedObj));
  ElementVec labelelem = _Get_label_elements(scene, Zombie_L);
  Zombie *zombie;
  Shape *selfhitbox = board->hitbox;
  int garbage = 0;

  for (int i = 0; i < labelelem.len; ++i) {
    zombie = (Zombie *)labelelem.arr[i]->pDerivedObj;
    if (selfhitbox->overlap(selfhitbox, zombie->hitbox)) {
      garbage++;
      zombie->stop_walk = true;

    } else {
      zombie->stop_walk = false;
    }
  }
  if (garbage) {
    board->damage_timer++;
  }
  if (board->damage_timer && !(board->damage_timer % ((int)FPS * 3)) &&
      garbage) {
    recieve_garbage(board, garbage);
  }
}
void Tetris_board_draw(Elements *self) {
  Tetris_board *board = ((Tetris_board *)(self->pDerivedObj));
  // al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_rectangle(board->x1, board->y1, board->x2, board->y2,
                    al_map_rgb(255, 255, 255), 2);

  double actual_time = (double)board->timer / FPS;
  al_draw_textf(board->font, board->font_color, board->pps_x, board->pps_y,
                ALLEGRO_ALIGN_LEFT, "PPS:\n%.2lf", board->pieces / actual_time);
  al_draw_textf(board->font, board->font_color, board->apm_x, board->apm_y,
                ALLEGRO_ALIGN_LEFT, "APM:%.2lf",
                board->attack * 60 / actual_time);
  al_draw_textf(board->font, board->font_color, board->pieces_x,
                board->pieces_y, ALLEGRO_ALIGN_LEFT, "PIECES:%d",
                board->pieces);
  al_draw_textf(board->font, board->font_color, board->time_x, board->time_y,
                ALLEGRO_ALIGN_LEFT, "TIME:%02d:%02d", (int)actual_time / 60,
                (int)actual_time % 60);
  al_draw_textf(board->font, board->font_color, board->time_x,
                board->time_y + 50, ALLEGRO_ALIGN_LEFT, "APP:%.2lf",
                (board->pieces) ? (double)board->attack / board->pieces : 0);
  al_draw_textf(board->font, board->font_color, board->hold_x, board->hold_y,
                ALLEGRO_ALIGN_CENTER, "HOLD");
  al_draw_textf(board->font, board->font_color, board->next_x[0],
                board->next_y[0] - 150, ALLEGRO_ALIGN_CENTER, "NEXT");
  if (board->in_b2b >= 1) {
    al_draw_textf(board->font_small, board->font_color, board->b2b_x - 20,
                  board->b2b_y, ALLEGRO_ALIGN_CENTER, "BACK-TO-BACK x%d",
                  board->in_b2b);
  }
  if (strcmp(board->cancel_type, "None")) {
    al_draw_text(board->font, board->font_color, board->cancel_x,
                 board->cancel_y, ALLEGRO_ALIGN_CENTER, board->cancel_type);
  }
  if (board->t_spun) {
    al_draw_text(
        board->font,
        al_map_rgb(tetrimino_colors[TETR_T][0], tetrimino_colors[TETR_T][1],
                   tetrimino_colors[TETR_T][2]),
        board->cancel_x, board->cancel_y + 50, ALLEGRO_ALIGN_CENTER, "T-SPIN");
  }
  int side_len = board->side_len;
  for (int i = 0; i < board->garbage_queue; ++i) {
    al_draw_filled_rectangle(
        board->gb_q_x, board->gb_q_y - side_len * i, board->gb_q_x - side_len,
        board->gb_q_y - side_len * (i + 1), al_map_rgb(255, 0, 0));
  }

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
