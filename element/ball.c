#include "ball.h"

#include <allegro5/allegro_primitives.h>

#include "../shapes/Circle.h"
Elements* New_Ball(int label) {
    Ball* pDerivedObj = (Ball*)malloc(sizeof(Ball));
    Elements* pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->x = mouse.x;
    pDerivedObj->y = mouse.y;
    pDerivedObj->r = 10;
    pDerivedObj->color = al_map_rgb(255, 0, 0);
    pDerivedObj->hitbox =
        New_Circle(pDerivedObj->x, pDerivedObj->y, pDerivedObj->r);
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Ball_update;
    pObj->Interact = Ball_interact;
    pObj->Draw = Ball_draw;
    pObj->Destroy = Ball_destory;
    return pObj;
}
void Ball_update(Elements* self) {
    Ball* ball = ((Ball*)(self->pDerivedObj));
    Shape* hitbox = ball->hitbox;
    hitbox->update_center_x(hitbox, mouse.x - ball->x);
    hitbox->update_center_y(hitbox, mouse.y - ball->y);
    ball->x = mouse.x;
    ball->y = mouse.y;
}
void Ball_interact(Elements* self) {}
void Ball_draw(Elements* self) {
    Ball* ball = ((Ball*)(self->pDerivedObj));
    al_draw_circle(ball->x, ball->y, ball->r, ball->color, 10);
}
void Ball_destory(Elements* self) {
    Ball* Obj = ((Ball*)(self->pDerivedObj));
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
