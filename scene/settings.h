#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/bitmap.h>

#include "scene.h"
/*
   [Menu object]
*/
typedef struct _Settings {
  ALLEGRO_FONT *font;
  ALLEGRO_BITMAP *background;
  ALLEGRO_SAMPLE *song;
  ALLEGRO_SAMPLE_INSTANCE *sample_instance;
  int title_x, title_y;
} Settings;
Scene *New_Settings(int label);
void settings_update(Scene *self);
void settings_draw(Scene *self);
void settings_destroy(Scene *self);

#endif
