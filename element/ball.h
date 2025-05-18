#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED
#include "../scene/gamescene.h"  // for element label
#include "../shapes/Shape.h"
#include "element.h"
typedef struct _Ball {
    int x, y;
    int r;
    ALLEGRO_COLOR color;
    Shape* hitbox;
} Ball;
Elements* New_Ball(int label);
void Ball_update(Elements* self);
void Ball_interact(Elements* self);
void Ball_draw(Elements* self);
void Ball_destory(Elements* self);

#endif
