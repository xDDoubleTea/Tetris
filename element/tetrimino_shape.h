#ifndef TETRIMINO_SHAPE_H
#define TETRIMINO_SHAPE_H

#include <allegro5/allegro_color.h>

typedef struct {
  int x, y;
} TetriminoShape;

typedef struct {
  int x, y;
} SRSoffset;

typedef struct {
  int first, last;
} RotationStateDiff;

enum TetriminoType {
  TETR_O,
  TETR_S,
  TETR_Z,
  TETR_T,
  TETR_I,
  TETR_J,
  TETR_L,
  NUM_TETRIMINO_TYPES
};

enum RotationStateDiffs {
  X_TO_R,
  R_TO_X,
  R_TO_2,
  _2_TO_R,
  _2_TO_L,
  L_TO_2,
  L_TO_X,
  X_TO_L,
  NUM_ROTATIONSTATEDIFFS,
};

extern const TetriminoShape tetrimino_shapes[NUM_TETRIMINO_TYPES][4][4];
extern const SRSoffset srsoffset[2][4][5];
extern const unsigned char tetrimino_colors[NUM_TETRIMINO_TYPES][3];
extern const RotationStateDiff rotationStateDiff[NUM_ROTATIONSTATEDIFFS];

#endif
