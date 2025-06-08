#include "tetriminos.h"

#include <allegro5/allegro5.h>
#include <allegro5/color.h>
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

#include "../scene/sceneManager.h" // for scene variable
#include "../shapes/Rectangle.h"
#include "Tetris_board.h"

int highest_from_y_at_x(Tetris_board *board, int x, int y) {
  int highest = 20;
  for (int j = 20; j > y; --j) {
    if (board->occupied[x][j] && j < highest) {
      highest = j;
    }
  }
  return highest;
}

void rotate(Tetrimino *t) {}

void soft_drop(Tetrimino *tetrimino, Tetris_board *board, int factor) {
  tetrimino->timer++;
  int actual_gravity = (int)(FPS * board->gravity) / factor;

  if (!(tetrimino->timer % actual_gravity)) {
    int temp_check_coord_y = tetrimino->coord_y + 1;
    const TetriminoShape *cur_block_shape =
        tetrimino_shapes[tetrimino->block_type][tetrimino->rotation];
    for (int i = 0; i < 4; ++i) {
      if (cur_block_shape[i].y + temp_check_coord_y >= 0 &&
          cur_block_shape[i].y + temp_check_coord_y >=
              highest_from_y_at_x(
                  board, tetrimino->coord_x + cur_block_shape[i].x,
                  cur_block_shape[i].y + temp_check_coord_y - 1)) {
        drop_tetrimino();
        return;
      }
    }
    tetrimino->coord_y++;
  }
}

void clear_line(Tetris_board *board) {
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
  if (line_clear) {
    for (int i = 19; i >= 0; --i) {
      if (rows_cleared[i]) {
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
  }
}

void drop_tetrimino() {
  ElementVec labelelem = _Get_label_elements(scene, Tetrimino_L);
  ElementVec board_elem = _Get_label_elements(scene, Tetris_board_L);
  Tetris_board *board = board_elem.arr[0]->pDerivedObj;
  Tetrimino *t;
  int pos_0_tet = find_pos_0_tet();
  t = (Tetrimino *)labelelem.arr[pos_0_tet]->pDerivedObj;
  fprintf(stdout, "t->held = %d\n", t->held);
  fprintf(stdout, "\n");
  const TetriminoShape *cur_block_shape =
      tetrimino_shapes[t->block_type][t->rotation];
  int closest_x = 0;
  int closest_x_idx = 0;
  int closest_dist = 200;
  int highest_from_y = 0;
  for (int i = 0; i < 4; ++i) {
    highest_from_y =
        highest_from_y_at_x(board, t->coord_x + cur_block_shape[i].x,
                            t->coord_y + cur_block_shape[i].y);
    if (highest_from_y - (t->coord_y + cur_block_shape[i].y) < closest_dist) {
      closest_x = t->coord_x + cur_block_shape[i].x;
      closest_x_idx = i;
      closest_dist = highest_from_y_at_x(board, closest_x,
                                         t->coord_y + cur_block_shape[i].y) -
                     (t->coord_y + cur_block_shape[i].y);
    }
  }
  highest_from_y = highest_from_y_at_x(
      board, closest_x, t->coord_y + cur_block_shape[closest_x_idx].y);
  int place_at_x = t->coord_x;
  int place_at_y = highest_from_y - 1 - cur_block_shape[closest_x_idx].y;

  t->coord_x = place_at_x;
  t->coord_y = place_at_y;
  for (int i = 0; i < 4; i++) {
    int block_x = t->coord_x + cur_block_shape[i].x;
    int block_y = t->coord_y + cur_block_shape[i].y;
    board->occupied[block_x][block_y] = true;
    board->color_map[block_x][block_y] = t->color;
  }
  int pos_1_tet = find_pos_1_tet();
  fprintf(stderr, "pos_1_tet = %d\n\n", pos_1_tet);
  t = (Tetrimino *)labelelem.arr[pos_1_tet]->pDerivedObj;
  const TetriminoShape *nextblock_shape =
      tetrimino_shapes[t->block_type][t->rotation];
  for (int i = 0; i < 4; ++i) {
    if (t->coord_y + nextblock_shape[i].y >= 0 &&
        board->occupied[(t->coord_x + nextblock_shape[i].x) % 10]
                       [(t->coord_y + nextblock_shape[i].y) % 20]) {
      board->game_over = true;
    }
  }
  clear_line(board);
  if (!board->game_over) {
    labelelem.arr[find_pos_0_tet()]->dele = true;
    board->pieces++;
    board->pieces_in_queue--;
    for (int i = 0; i < labelelem.len; ++i) {
      t = (Tetrimino *)labelelem.arr[i]->pDerivedObj;
      if (!t->held) {
        t->pos_in_queue--;
      }
    }
  }
}

int tetr_get_right_most(Tetrimino *tetrimino) {
  int right_most = 0;
  for (int i = 0; i < 4; ++i) {
    int cur_x =
        tetrimino_shapes[tetrimino->block_type][tetrimino->rotation][i].x +
        tetrimino->coord_x;
    if (cur_x > right_most) {
      right_most = cur_x;
    }
  }
  return right_most;
}

int tetr_get_left_most(Tetrimino *tetrimino) {
  int left_most = 10;
  for (int i = 0; i < 4; ++i) {
    int cur_x =
        tetrimino_shapes[tetrimino->block_type][tetrimino->rotation][i].x +
        tetrimino->coord_x;
    if (cur_x < left_most) {
      left_most = cur_x;
    }
  }
  return left_most;
}

int tetr_get_highest(Tetrimino *tetrimino) {
  int highest = 20;
  for (int i = 0; i < 4; ++i) {
    int cur_y =
        tetrimino_shapes[tetrimino->block_type][tetrimino->rotation][i].y +
        tetrimino->coord_y;
    if (cur_y < highest) {
      highest = cur_y;
    }
  }
  return highest;
}

Elements *New_Tetrimino(int label, int type, int pos_in_queue) {
  Tetrimino *pDerivedObj = (Tetrimino *)malloc(sizeof(Tetrimino));
  Elements *pObj = New_Elements(label);
  // setting derived object member
  pDerivedObj->block_type = type;
  pDerivedObj->pos_in_queue = pos_in_queue;
  pDerivedObj->rotation = 0;
  pDerivedObj->coord_x = 4;
  pDerivedObj->held = false;
  pDerivedObj->dropped = false;
  pDerivedObj->soft_dropping = false;
  pDerivedObj->coord_y = 0;
  pDerivedObj->timer = 0;
  pDerivedObj->color =
      al_map_rgb(tetrimino_colors[type][0], tetrimino_colors[type][1],
                 tetrimino_colors[type][2]);
  pDerivedObj->hitbox = New_Rectangle(0, 0, 0, 0);
  // setting derived object function
  pDerivedObj->move_lock = false;
  pDerivedObj->rotate_lock = false;
  pObj->pDerivedObj = pDerivedObj;
  pObj->Update = Tetrimino_update;
  pObj->Interact = Tetrimino_interact;
  pObj->Draw = Tetrimino_draw;
  pObj->Destroy = Tetrimino_destory;
  return pObj;
}
void Tetrimino_update(Elements *self) {
  Tetrimino *tetrimino = ((Tetrimino *)(self->pDerivedObj));
  ElementVec labelelem = _Get_label_elements(scene, Tetris_board_L);
  Tetris_board *board = (Tetris_board *)labelelem.arr[0]->pDerivedObj;
  if (tetrimino->pos_in_queue || tetrimino->held || tetrimino->dropped ||
      board->game_over) {
    return;
  }
  if (key_state[ALLEGRO_KEY_LEFT] && !tetrimino->move_lock) {
    int temp_check_coord_x = tetrimino->coord_x - 1;
    for (int i = 0; i < 4; ++i) {
      if (tetrimino_shapes[tetrimino->block_type][tetrimino->rotation][i].x +
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
      if (tetrimino_shapes[tetrimino->block_type][tetrimino->rotation][i].x +
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
  if (key_state[ALLEGRO_KEY_UP] && !tetrimino->rotate_lock) {
    tetrimino->rotation = (tetrimino->rotation + 1) % 4;
    tetrimino->rotate_lock = true;
  }
  if (key_state[ALLEGRO_KEY_Z] && !tetrimino->rotate_lock) {
    tetrimino->rotation = (tetrimino->rotation + 3) % 4;
    // 3 \equiv -1 mod 4
    tetrimino->rotate_lock = true;
  }
  if (key_state[ALLEGRO_KEY_A] && !tetrimino->rotate_lock) {
    tetrimino->rotation = (tetrimino->rotation + 2) % 4;
    tetrimino->rotate_lock = true;
  }
  if (!key_state[ALLEGRO_KEY_A] && !key_state[ALLEGRO_KEY_Z] &&
      !key_state[ALLEGRO_KEY_UP]) {
    tetrimino->rotate_lock = false;
  }

  if (key_state[ALLEGRO_KEY_DOWN]) {
    tetrimino->soft_dropping = true;
    soft_drop(tetrimino, board, 8);
  }
  if (!key_state[ALLEGRO_KEY_DOWN]) {
    tetrimino->soft_dropping = false;
    soft_drop(tetrimino, board, 1);
  }
}
void Tetrimino_interact(Elements *self) {}
void Tetrimino_draw(Elements *self) {
  ElementVec labelelem = _Get_label_elements(scene, Tetris_board_L);
  Tetrimino *tetrimino = ((Tetrimino *)(self->pDerivedObj));
  Tetris_board *board = ((Tetris_board *)(labelelem.arr[0]->pDerivedObj));
  if (tetrimino->dropped) {
    return;
  }
  if (tetrimino->held) {
    int side_len = board->side_len;
    const TetriminoShape *cur_block_shape =
        tetrimino_shapes[tetrimino->block_type][tetrimino->rotation];
    for (int i = 0; i < 4; ++i) {
      float screen_x1 = board->hold_x - 50 + cur_block_shape[i].x * side_len;
      float screen_y1 = board->hold_y + 100 + cur_block_shape[i].y * side_len;
      float screen_x2 = screen_x1 + side_len;
      float screen_y2 = screen_y1 + side_len;
      al_draw_filled_rectangle(screen_x1, screen_y1, screen_x2, screen_y2,
                               tetrimino->color);
    }
    return;
  }
  if (tetrimino->pos_in_queue) {
    if (tetrimino->pos_in_queue <= 5 && tetrimino->pos_in_queue >= 0) {
      int side_len = board->side_len;
      const TetriminoShape *cur_block_shape =
          tetrimino_shapes[tetrimino->block_type][tetrimino->rotation];
      for (int i = 0; i < 4; ++i) {
        float screen_x1 = board->next_x[tetrimino->pos_in_queue - 1] +
                          cur_block_shape[i].x * side_len;
        float screen_y1 = board->next_y[tetrimino->pos_in_queue - 1] +
                          cur_block_shape[i].y * side_len;
        float screen_x2 = screen_x1 + side_len;
        float screen_y2 = screen_y1 + side_len;
        al_draw_filled_rectangle(screen_x1, screen_y1, screen_x2, screen_y2,
                                 tetrimino->color);
      }
    }
    return;
  }
  int side_len = board->side_len;
  const TetriminoShape *cur_block_shape =
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

void Tetrimino_destory(Elements *self) {
  Tetrimino *Obj = ((Tetrimino *)(self->pDerivedObj));
  free(Obj->hitbox);
  free(Obj);
  free(self);
}
