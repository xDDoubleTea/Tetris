#include "tetriminos.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
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

int srs_kick(Tetrimino *tetrimino, Tetris_board *board, int origin_rotation,
             int next_rotation) {
  int temp_check_coord_x = 0;
  int temp_check_coord_y = 0;
  int amog = (tetrimino->block_type == TETR_I) ? 1 : 0;
  const TetriminoShape *cur_block_shape =
      tetrimino_shapes[tetrimino->block_type][next_rotation];
  for (int i = 0; i < 5; ++i) {
    temp_check_coord_x = tetrimino->coord_x;
    temp_check_coord_y = tetrimino->coord_y;
    temp_check_coord_x += srsoffset[amog][origin_rotation][i].x -
                          srsoffset[amog][next_rotation][i].x;
    temp_check_coord_y -= srsoffset[amog][origin_rotation][i].y -
                          srsoffset[amog][next_rotation][i].y;
    int valid = 1;
    for (int j = 0; j < 4; ++j) {
      if ((temp_check_coord_x + cur_block_shape[j].x < 0 ||
           temp_check_coord_x + cur_block_shape[j].x >= 10) ||
          (temp_check_coord_y + cur_block_shape[j].y >= 20) ||
          board->occupied[temp_check_coord_x + cur_block_shape[j].x]
                         [temp_check_coord_y + cur_block_shape[j].y]) {
        valid = 0;
        break;
      }
    }
    if (valid) {
      return i;
    }
  }
  return -1;
}

bool check_t_spin(Tetrimino *tetrimino, Tetris_board *board) {
  const TetriminoShape *cur_block_shape =
      tetrimino_shapes[tetrimino->block_type][tetrimino->rotation];
  // Try to see if it cannot be moved right or left or up.
  bool check_left_flag = false;
  bool check_right_flag = false;
  bool check_up_flag = false;
  for (int i = 0; i < 4; ++i) {
    int check_left = tetrimino->coord_x - 1 + cur_block_shape[i].x;

    int check_right = tetrimino->coord_x + 1 + cur_block_shape[i].x;
    int check_up = tetrimino->coord_y - 1 + cur_block_shape[i].y;
    if (board
            ->occupied[check_left][tetrimino->coord_y + cur_block_shape[i].y] ||
        check_left < 0) {
      check_left_flag = true;
    }
    if (board->occupied[check_right]
                       [tetrimino->coord_y + cur_block_shape[i].y] ||
        check_right >= 10) {
      check_right_flag = true;
    }
    if (board->occupied[tetrimino->coord_x + cur_block_shape[i].x][check_up]) {
      check_up_flag = true;
    }
  }
  return check_left_flag & check_right_flag & check_up_flag;
}

void srs_handler(Tetrimino *tetrimino, Tetris_board *board, int origin_rotation,
                 int next_rotation) {
  int amog = (tetrimino->block_type == TETR_I) ? 1 : 0;
  int kick_offset = srs_kick(tetrimino, board, origin_rotation, next_rotation);
  if (kick_offset != -1) {
    tetrimino->coord_x += srsoffset[amog][origin_rotation][kick_offset].x -
                          srsoffset[amog][next_rotation][kick_offset].x;
    tetrimino->coord_y -= srsoffset[amog][origin_rotation][kick_offset].y -
                          srsoffset[amog][next_rotation][kick_offset].y;
    tetrimino->rotation = next_rotation;
    if (tetrimino->block_type == TETR_T) {
      tetrimino->did_t_spin = check_t_spin(tetrimino, board);
    } else {
      tetrimino->did_t_spin = false;
    }
    tetrimino->tetrimino_last_oper = ROTATE;
  }
  tetrimino->rotate_lock = true;
}

void rotate(Tetrimino *tetrimino, Tetris_board *board) {
  if (tetrimino->block_type == TETR_O) {
    // no O spin for you lol
    return;
  }
  if (key_state[ALLEGRO_KEY_UP] && !tetrimino->rotate_lock) {
    srs_handler(tetrimino, board, tetrimino->rotation,
                (tetrimino->rotation + 1) % 4);
  }
  if (key_state[ALLEGRO_KEY_Z] && !tetrimino->rotate_lock) {
    srs_handler(tetrimino, board, tetrimino->rotation,
                (tetrimino->rotation + 3) % 4);
  }
  if (key_state[ALLEGRO_KEY_A] && !tetrimino->rotate_lock) {
    srs_handler(tetrimino, board, tetrimino->rotation,
                (tetrimino->rotation + 2) % 4);
  }
  if (!key_state[ALLEGRO_KEY_A] && !key_state[ALLEGRO_KEY_Z] &&
      !key_state[ALLEGRO_KEY_UP]) {
    tetrimino->rotate_lock = false;
  }
}

bool on_ground(Tetrimino *tetrimino, Tetris_board *board) {
  int temp_check_coord_y = tetrimino->coord_y + 1;
  const TetriminoShape *cur_block_shape =
      tetrimino_shapes[tetrimino->block_type][tetrimino->rotation];
  for (int i = 0; i < 4; ++i) {
    if (cur_block_shape[i].y + temp_check_coord_y >= 0 &&
        cur_block_shape[i].y + temp_check_coord_y >=
            highest_from_y_at_x(
                board, tetrimino->coord_x + cur_block_shape[i].x,
                cur_block_shape[i].y + temp_check_coord_y - 1)) {
      return true;
    }
  }
  return false;
}

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
        // drop_tetrimino();
        return;
      }
    }
    tetrimino->coord_y++;
  }
}

void drop_tetrimino() {
  ElementVec labelelem = _Get_label_elements(scene, Tetrimino_L);
  ElementVec board_elem = _Get_label_elements(scene, Tetris_board_L);
  Tetris_board *board = board_elem.arr[0]->pDerivedObj;
  Tetrimino *t;
  int pos_0_tet = find_pos_0_tet();
  t = (Tetrimino *)labelelem.arr[pos_0_tet]->pDerivedObj;
  // fprintf(stdout, "t->held = %d\n", t->held);
  // fprintf(stdout, "\n");
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
  clear_line(board, t);
  int pos_1_tet = find_pos_1_tet();
  t = (Tetrimino *)labelelem.arr[pos_1_tet]->pDerivedObj;
  const TetriminoShape *nextblock_shape =
      tetrimino_shapes[t->block_type][t->rotation];
  for (int i = 0; i < 4; ++i) {
    if (t->coord_y + nextblock_shape[i].y + 2 >= 0 &&
        board->occupied[(t->coord_x + nextblock_shape[i].x) % 10]
                       [(t->coord_y + nextblock_shape[i].y + 2)]) {
      board->game_over = true;
    }
  }
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

Elements *New_Tetrimino(int label, int type, int pos_in_queue) {
  Tetrimino *pDerivedObj = (Tetrimino *)malloc(sizeof(Tetrimino));
  Elements *pObj = New_Elements(label);
  // setting derived object member
  pDerivedObj->block_type = type;
  pDerivedObj->pos_in_queue = pos_in_queue;
  pDerivedObj->rotation = 0;
  pDerivedObj->coord_x = 4;
  pDerivedObj->coord_y = -1;
  pDerivedObj->held = false;
  pDerivedObj->dropped = false;
  pDerivedObj->soft_dropping = false;
  pDerivedObj->timer = 0;
  pDerivedObj->soft_drop_factor = 40;
  pDerivedObj->das = das;
  pDerivedObj->arr = arr;
  pDerivedObj->das_timer = 0;
  pDerivedObj->arr_timer = 0;
  pDerivedObj->color =
      al_map_rgb(tetrimino_colors[type][0], tetrimino_colors[type][1],
                 tetrimino_colors[type][2]);
  pDerivedObj->ghost_block_color =
      al_map_rgba(tetrimino_colors[type][0], tetrimino_colors[type][1],
                  tetrimino_colors[type][2], 1);
  pDerivedObj->hitbox = New_Rectangle(0, 0, 0, 0);
  // setting derived object function
  pDerivedObj->move_lock = false;
  pDerivedObj->tetrimino_last_oper = GRAVITY;
  pDerivedObj->rotate_lock = false;
  pObj->pDerivedObj = pDerivedObj;
  pObj->Update = Tetrimino_update;
  pObj->Interact = Tetrimino_interact;
  pObj->Draw = Tetrimino_draw;
  pObj->Destroy = Tetrimino_destory;
  return pObj;
}

void move_tetrimino(Tetrimino *tetrimino, Tetris_board *board,
                    int temp_check_coord_x, int offset) {

  const TetriminoShape *cur_block_shape =
      tetrimino_shapes[tetrimino->block_type][tetrimino->rotation];
  temp_check_coord_x += offset;
  for (int i = 0; i < 4; ++i) {
    if ((cur_block_shape[i].x + temp_check_coord_x < 0 ||
         cur_block_shape[i].x + temp_check_coord_x >= 10 ||
         (tetrimino->coord_y + cur_block_shape[i].y > 0 &&
          board->occupied[temp_check_coord_x + cur_block_shape[i].x]
                         [tetrimino->coord_y + cur_block_shape[i].y]))) {
      tetrimino->move_lock = true;
      return;
    }
  }
  tetrimino->coord_x += offset;
  // fprintf(stderr, "das_timer = %d\n", tetrimino->das_timer);
  tetrimino->tetrimino_last_oper = MOVE;
  GameScene *gamescene = (GameScene *)(scene->pDerivedObj);
  switch (tetrimino->block_type) {
  case TETR_O: {
    al_play_sample_instance(gamescene->o_sample_instance);
    break;
  }
  case TETR_I: {
    al_play_sample_instance(gamescene->i_sample_instance);
    break;
  }
  case TETR_T: {
    al_play_sample_instance(gamescene->t_sample_instance);
    break;
  }
  case TETR_Z: {
    al_play_sample_instance(gamescene->z_sample_instance);
    break;
  }
  case TETR_S: {
    al_play_sample_instance(gamescene->s_sample_instance);
    break;
  }
  case TETR_J: {
    al_play_sample_instance(gamescene->j_sample_instance);
    break;
  }
  case TETR_L: {
    al_play_sample_instance(gamescene->l_sample_instance);
    break;
  }
  }
}
void Tetrimino_update(Elements *self) {
  Tetrimino *tetrimino = ((Tetrimino *)(self->pDerivedObj));
  ElementVec labelelem = _Get_label_elements(scene, Tetris_board_L);
  Tetris_board *board = (Tetris_board *)labelelem.arr[0]->pDerivedObj;
  if (tetrimino->pos_in_queue || tetrimino->held || tetrimino->dropped ||
      board->game_over) {
    return;
  }
  if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_RIGHT]) {
    tetrimino->das_timer++;
    if (tetrimino->das_timer < tetrimino->das) {
      if (key_state[ALLEGRO_KEY_LEFT] && !tetrimino->move_lock) {
        move_tetrimino(tetrimino, board, tetrimino->coord_x, -1);
      } else if (key_state[ALLEGRO_KEY_RIGHT] && !tetrimino->move_lock) {
        move_tetrimino(tetrimino, board, tetrimino->coord_x, 1);
      }
      tetrimino->move_lock = true;
    } else {
      tetrimino->move_lock = false;
      tetrimino->arr_timer++;
      if (!(tetrimino->arr_timer % tetrimino->arr) &&
          tetrimino->arr_timer > 0) {
        if (key_state[ALLEGRO_KEY_LEFT] && !tetrimino->move_lock) {
          move_tetrimino(tetrimino, board, tetrimino->coord_x, -1);
        } else if (key_state[ALLEGRO_KEY_RIGHT] && !tetrimino->move_lock) {
          move_tetrimino(tetrimino, board, tetrimino->coord_x, 1);
        }
      }
    }
  } else if (!key_state[ALLEGRO_KEY_LEFT] && !key_state[ALLEGRO_KEY_RIGHT]) {
    tetrimino->move_lock = false;
    tetrimino->das_timer = 0;
    tetrimino->arr_timer = 0;
  }

  rotate(tetrimino, board);
  if (key_state[ALLEGRO_KEY_DOWN]) {
    tetrimino->soft_dropping = true;
    soft_drop(tetrimino, board, tetrimino->soft_drop_factor);
  }
  if (!key_state[ALLEGRO_KEY_DOWN]) {
    tetrimino->soft_dropping = false;
    // soft_drop(tetrimino, board, 1);
  }
}
void Tetrimino_interact(Elements *self) {}
void draw_ghost_block(Tetrimino *tetrimino, Tetris_board *board) {
  int side_len = board->side_len;
  const TetriminoShape *cur_block_shape =
      tetrimino_shapes[tetrimino->block_type][tetrimino->rotation];
  int closest_x = 0;
  int closest_x_idx = 0;
  int closest_dist = 200;
  int highest_from_y = 0;
  for (int i = 0; i < 4; ++i) {
    highest_from_y =
        highest_from_y_at_x(board, tetrimino->coord_x + cur_block_shape[i].x,
                            tetrimino->coord_y + cur_block_shape[i].y);
    if (highest_from_y - (tetrimino->coord_y + cur_block_shape[i].y) <
        closest_dist) {
      closest_x = tetrimino->coord_x + cur_block_shape[i].x;
      closest_x_idx = i;
      closest_dist =
          highest_from_y_at_x(board, closest_x,
                              tetrimino->coord_y + cur_block_shape[i].y) -
          (tetrimino->coord_y + cur_block_shape[i].y);
    }
  }
  highest_from_y = highest_from_y_at_x(
      board, closest_x, tetrimino->coord_y + cur_block_shape[closest_x_idx].y);
  int place_at_x = tetrimino->coord_x;
  int place_at_y =
      min(highest_from_y - 1 - cur_block_shape[closest_x_idx].y, 19);

  for (int i = 0; i < 4; i++) {
    int block_x = place_at_x + cur_block_shape[i].x;
    int block_y = place_at_y + cur_block_shape[i].y;

    float screen_x1 = board->x1 + block_x * side_len;
    float screen_y1 = board->y1 + block_y * side_len;
    float screen_x2 = screen_x1 + side_len;
    float screen_y2 = screen_y1 + side_len;

    al_draw_filled_rectangle(screen_x1, screen_y1, screen_x2, screen_y2,
                             tetrimino->ghost_block_color);
  }
}
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
  draw_ghost_block(tetrimino, board);
}

void Tetrimino_destory(Elements *self) {
  Tetrimino *Obj = ((Tetrimino *)(self->pDerivedObj));
  free(Obj->hitbox);
  free(Obj);
  free(self);
}
