#ifndef TETRIMINO_SHAPE_H
#define TETRIMINO_SHAPE_H

#include <allegro5/allegro_color.h>

typedef struct {
    int x, y;
} TetriminoShape;

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

extern const TetriminoShape tetrimino_shapes[NUM_TETRIMINO_TYPES][4][4];
extern const unsigned char tetrimino_colors[NUM_TETRIMINO_TYPES][3];

#endif
