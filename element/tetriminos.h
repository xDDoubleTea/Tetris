#ifndef TETRIMINOS_H
#define TETRIMINOS_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "../scene/gamescene.h"  // for element label
#include "../shapes/Shape.h"
#include "element.h"
extern const unsigned char gray[3];
extern const unsigned char o_color[3];
extern const unsigned char s_color[3];
extern const unsigned char z_color[3];
extern const unsigned char t_color[3];
extern const unsigned char i_color[3];
extern const unsigned char j_color[3];
extern const unsigned char l_color[3];

typedef struct _BLOCK {
    float side_len;
    int coord_x;
    int coord_y;
    ALLEGRO_COLOR color;
} Block;
typedef struct _O_PIECE O_PIECE;
typedef struct _I_PIECE I_PIECE;
typedef struct _J_PIECE J_PIECE;
typedef struct _L_PIECE L_PIECE;
typedef struct _T_PIECE T_PIECE;
typedef struct _Z_PIECE Z_PIECE;
typedef struct _S_PIECE S_PIECE;
typedef struct _Tetrimino {
    char block_type;
    int rotation;
    int coord_x, coord_y;
    ALLEGRO_COLOR color;
    Shape* hitbox;
} Tetrimino;
Elements* New_Tetrimino(int label);
void Tetrimino_update(Elements* self);
void Tetrimino_interact(Elements* self);
void Tetrimino_draw(Elements* self);
void Tetrimino_destory(Elements* self);

#endif
