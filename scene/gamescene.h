#ifndef GAMESCENE_H_INCLUDED
#define GAMESCENE_H_INCLUDED
#include "scene.h"
#include <allegro5/allegro_audio.h>

/*
   [game scene object]
*/
typedef enum EleType {
  Floor_L,
  Teleport_L,
  Tree_L,
  Character_L,
  Projectile_L,
  Ball_L,
  Settings_btn_L,
  Start_btn_L,
  Quit_btn_L,
  Back_btn_L,
  Das_btn_L,
  Arr_btn_L,
  Sdf_Btn_L,
  Tetris_board_L,
  Tetrimino_L,
} EleType;
typedef struct _GameScene {
  ALLEGRO_BITMAP *background;
  ALLEGRO_SAMPLE *i_mino;
  ALLEGRO_SAMPLE *o_mino;
  ALLEGRO_SAMPLE *l_mino;
  ALLEGRO_SAMPLE *j_mino;
  ALLEGRO_SAMPLE *t_mino;
  ALLEGRO_SAMPLE *z_mino;
  ALLEGRO_SAMPLE *s_mino;
  ALLEGRO_SAMPLE *bgm_piercing_wind;
  ALLEGRO_SAMPLE *bgm_original_tetris;
  ALLEGRO_SAMPLE *b2b_1;
  ALLEGRO_SAMPLE *b2b_2;
  ALLEGRO_SAMPLE *b2b_3;
  ALLEGRO_SAMPLE *b2b_break;
  ALLEGRO_SAMPLE *combo[16];
  ALLEGRO_SAMPLE *all_clear;

  ALLEGRO_SAMPLE_INSTANCE *o_sample_instance;
  ALLEGRO_SAMPLE_INSTANCE *i_sample_instance;
  ALLEGRO_SAMPLE_INSTANCE *l_sample_instance;
  ALLEGRO_SAMPLE_INSTANCE *j_sample_instance;
  ALLEGRO_SAMPLE_INSTANCE *t_sample_instance;
  ALLEGRO_SAMPLE_INSTANCE *s_sample_instance;
  ALLEGRO_SAMPLE_INSTANCE *z_sample_instance;
  ALLEGRO_SAMPLE_INSTANCE *bgm_piercing_wind_instance;
  ALLEGRO_SAMPLE_INSTANCE *bgm_original_tetris_instance;
  ALLEGRO_SAMPLE_INSTANCE *combo_instance[16];
  ALLEGRO_SAMPLE_INSTANCE *all_clear_instance;

  ALLEGRO_SAMPLE_INSTANCE *b2b_1_instance;
  ALLEGRO_SAMPLE_INSTANCE *b2b_2_instance;
  ALLEGRO_SAMPLE_INSTANCE *b2b_3_instance;
  ALLEGRO_SAMPLE_INSTANCE *b2b_break_instance;

  int bgm_choice;
  ALLEGRO_SAMPLE *bgm_choice_sample;
  ALLEGRO_SAMPLE_INSTANCE *bgm_choice_sample_instance;

  int start_music;
  int start_music_timer;
  float sfx_gain;
  float bgm_gain;
} GameScene;
Scene *New_GameScene(int label);
void gen_tetr_7_bag(Scene *self, int now_pieces);
void game_scene_update(Scene *self);
void game_scene_draw(Scene *self);
void game_scene_destroy(Scene *self);

#endif
