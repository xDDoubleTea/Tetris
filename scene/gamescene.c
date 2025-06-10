#include "gamescene.h"

#include "../algif5/algif.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/config.h>
#include <stdarg.h>
#include <stdio.h>

#include "../element/Tetris_board.h"
#include "../element/element.h"
#include "../element/tetrimino_shape.h"
#include "../element/zombie.h"
#include "scene.h"
/*
   [GameScene function]
*/

void gen_tetr_7_bag(Scene *pObj, int now_pieces) {
  int arr[7] = {TETR_O, TETR_S, TETR_Z, TETR_T, TETR_I, TETR_J, TETR_L};
  // Randomly shuffle the array
  for (int i = 6; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
  for (int i = 0; i < 7; i++) {
    // fprintf(stderr, "%d", arr[i]);
    _Register_elements(pObj,
                       New_Tetrimino(Tetrimino_L, arr[i], i + now_pieces));
  }
  // fprintf(stderr, "\n");
}
Scene *New_GameScene(int label) {
  GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
  Scene *pObj = New_Scene(label);
  // setting derived object member
  pObj->pDerivedObj = pDerivedObj;
  // register element
  _Register_elements(pObj, New_Tetris_board(Tetris_board_L));
  _Register_elements(pObj, New_zombie(Zombie_L));
  gen_tetr_7_bag(pObj, 0);
  // setting derived object function
  al_reserve_samples(64);

  pDerivedObj->background = al_load_bitmap("assests/bg.webp");
  pDerivedObj->zombie_gif = algif_new_gif("assests/zombiewalking.gif", 1);

  pDerivedObj->o_mino = al_load_sample("assests/Tetrio_retro_pack/o.wav");
  pDerivedObj->i_mino = al_load_sample("assests/Tetrio_retro_pack/i.wav");
  pDerivedObj->j_mino = al_load_sample("assests/Tetrio_retro_pack/j.wav");
  pDerivedObj->l_mino = al_load_sample("assests/Tetrio_retro_pack/l.wav");
  pDerivedObj->z_mino = al_load_sample("assests/Tetrio_retro_pack/z.wav");
  pDerivedObj->s_mino = al_load_sample("assests/Tetrio_retro_pack/s.wav");
  pDerivedObj->t_mino = al_load_sample("assests/Tetrio_retro_pack/t.wav");
  pDerivedObj->sfx_gain = 0.2;
  pDerivedObj->bgm_gain = 0.05;
  pDerivedObj->bgm_piercing_wind =
      al_load_sample("assests/bgm/PiercingWind.wav");
  pDerivedObj->b2b_1 = al_load_sample("assests/Tetrio_retro_pack/btb_1.wav");
  pDerivedObj->b2b_2 = al_load_sample("assests/Tetrio_retro_pack/btb_2.wav");
  pDerivedObj->b2b_3 = al_load_sample("assests/Tetrio_retro_pack/btb_3.wav");
  pDerivedObj->b2b_break =
      al_load_sample("assests/Tetrio_retro_pack/btb_break.wav");
  for (int i = 0; i < 16; ++i) {
    char filename[128];
    snprintf(filename, sizeof(filename),
             "assests/Tetrio_retro_pack/combo_%d.wav", i + 1);
    pDerivedObj->combo[i] = al_load_sample(filename);
    pDerivedObj->combo_instance[i] =
        al_create_sample_instance(pDerivedObj->combo[i]);
    al_attach_sample_instance_to_mixer(pDerivedObj->combo_instance[i],
                                       al_get_default_mixer());
    al_set_sample_instance_gain(pDerivedObj->combo_instance[i],
                                pDerivedObj->sfx_gain);
    al_set_sample_instance_playmode(pDerivedObj->combo_instance[i],
                                    ALLEGRO_PLAYMODE_ONCE);
  }

  // load_and_set_sample("assests/Tetrio_retro_pack/harddrop.wav",
  //                     pDerivedObj->zombie_attack_sfx,
  //                     pDerivedObj->zombie_attack_sfx_instance,
  //                     pDerivedObj->sfx_gain, ALLEGRO_PLAYMODE_ONCE);
  pDerivedObj->hard_drop_sfx =
      al_load_sample("assests/Tetrio_retro_pack/harddrop.wav");
  pDerivedObj->hard_drop_sfx_instance =
      al_create_sample_instance(pDerivedObj->hard_drop_sfx);
  al_set_sample_instance_gain(pDerivedObj->hard_drop_sfx_instance,
                              pDerivedObj->sfx_gain);
  al_set_sample_instance_playmode(pDerivedObj->hard_drop_sfx_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->hard_drop_sfx_instance,
                                     al_get_default_mixer());

  pDerivedObj->game_over_sfx =
      al_load_sample("assests/Tetrio_retro_pack/failure.wav");
  pDerivedObj->game_over_sfx_instance =
      al_create_sample_instance(pDerivedObj->game_over_sfx);
  al_set_sample_instance_gain(pDerivedObj->game_over_sfx_instance,
                              pDerivedObj->sfx_gain);
  al_set_sample_instance_playmode(pDerivedObj->game_over_sfx_instance,
                                  ALLEGRO_PLAYMODE_BIDIR);
  al_attach_sample_instance_to_mixer(pDerivedObj->game_over_sfx_instance,
                                     al_get_default_mixer());

  pDerivedObj->all_clear =
      al_load_sample("assests/Tetrio_retro_pack/allclear.wav");
  pDerivedObj->all_clear_instance =
      al_create_sample_instance(pDerivedObj->all_clear);
  al_set_sample_instance_gain(pDerivedObj->all_clear_instance,
                              pDerivedObj->sfx_gain);
  al_set_sample_instance_playmode(pDerivedObj->all_clear_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->all_clear_instance,
                                     al_get_default_mixer());

  pDerivedObj->all_clear =
      al_load_sample("assests/Tetrio_retro_pack/allclear.wav");
  pDerivedObj->all_clear_instance =
      al_create_sample_instance(pDerivedObj->all_clear);
  al_set_sample_instance_gain(pDerivedObj->all_clear_instance,
                              pDerivedObj->sfx_gain);
  al_set_sample_instance_playmode(pDerivedObj->all_clear_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->all_clear_instance,
                                     al_get_default_mixer());

  pDerivedObj->bgm_original_tetris =
      al_load_sample("assests/bgm/Original_tetris_theme_test.wav");
  pDerivedObj->bgm_original_tetris_instance =
      al_create_sample_instance(pDerivedObj->bgm_original_tetris);
  al_attach_sample_instance_to_mixer(pDerivedObj->bgm_original_tetris_instance,
                                     al_get_default_mixer());
  al_set_sample_instance_gain(pDerivedObj->bgm_original_tetris_instance,
                              pDerivedObj->bgm_gain);
  al_set_sample_instance_playmode(pDerivedObj->bgm_original_tetris_instance,
                                  ALLEGRO_PLAYMODE_LOOP);
  pDerivedObj->b2b_1_instance = al_create_sample_instance(pDerivedObj->b2b_1);
  al_set_sample_instance_playmode(pDerivedObj->b2b_1_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->b2b_1_instance,
                                     al_get_default_mixer());
  // set the volume of instance
  al_set_sample_instance_gain(pDerivedObj->b2b_1_instance,
                              pDerivedObj->sfx_gain);

  pDerivedObj->b2b_2_instance = al_create_sample_instance(pDerivedObj->b2b_2);
  al_set_sample_instance_playmode(pDerivedObj->b2b_2_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->b2b_2_instance,
                                     al_get_default_mixer());
  // set the volume of instance
  al_set_sample_instance_gain(pDerivedObj->b2b_2_instance,
                              pDerivedObj->sfx_gain);

  pDerivedObj->b2b_break_instance =
      al_create_sample_instance(pDerivedObj->b2b_break);
  al_set_sample_instance_playmode(pDerivedObj->b2b_break_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->b2b_break_instance,
                                     al_get_default_mixer());
  // set the volume of instance
  al_set_sample_instance_gain(pDerivedObj->b2b_break_instance,
                              pDerivedObj->sfx_gain);

  pDerivedObj->b2b_3_instance = al_create_sample_instance(pDerivedObj->b2b_3);
  al_set_sample_instance_playmode(pDerivedObj->b2b_3_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->b2b_3_instance,
                                     al_get_default_mixer());
  // set the volume of instance
  al_set_sample_instance_gain(pDerivedObj->b2b_3_instance,
                              pDerivedObj->sfx_gain);

  pDerivedObj->bgm_piercing_wind_instance =
      al_create_sample_instance(pDerivedObj->bgm_piercing_wind);
  al_set_sample_instance_playmode(pDerivedObj->bgm_piercing_wind_instance,
                                  ALLEGRO_PLAYMODE_LOOP);
  al_attach_sample_instance_to_mixer(pDerivedObj->bgm_piercing_wind_instance,
                                     al_get_default_mixer());
  al_set_sample_instance_gain(pDerivedObj->bgm_piercing_wind_instance,
                              pDerivedObj->bgm_gain);

  pDerivedObj->o_sample_instance =
      al_create_sample_instance(pDerivedObj->o_mino);
  al_set_sample_instance_playmode(pDerivedObj->o_sample_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->o_sample_instance,
                                     al_get_default_mixer());
  // set the volume of instance
  al_set_sample_instance_gain(pDerivedObj->o_sample_instance,
                              pDerivedObj->sfx_gain);

  pDerivedObj->i_sample_instance =
      al_create_sample_instance(pDerivedObj->i_mino);
  al_set_sample_instance_playmode(pDerivedObj->i_sample_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->i_sample_instance,
                                     al_get_default_mixer());
  // set the volume of instance
  al_set_sample_instance_gain(pDerivedObj->i_sample_instance,
                              pDerivedObj->sfx_gain);

  pDerivedObj->j_sample_instance =
      al_create_sample_instance(pDerivedObj->j_mino);
  al_set_sample_instance_playmode(pDerivedObj->j_sample_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->j_sample_instance,
                                     al_get_default_mixer());
  // set the volume of instance
  al_set_sample_instance_gain(pDerivedObj->j_sample_instance,
                              pDerivedObj->sfx_gain);

  pDerivedObj->l_sample_instance =
      al_create_sample_instance(pDerivedObj->l_mino);
  al_set_sample_instance_playmode(pDerivedObj->l_sample_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->l_sample_instance,
                                     al_get_default_mixer());
  // set the volume of instance
  al_set_sample_instance_gain(pDerivedObj->l_sample_instance,
                              pDerivedObj->sfx_gain);

  pDerivedObj->z_sample_instance =
      al_create_sample_instance(pDerivedObj->z_mino);
  al_set_sample_instance_playmode(pDerivedObj->z_sample_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->z_sample_instance,
                                     al_get_default_mixer());
  // set the volume of instance
  al_set_sample_instance_gain(pDerivedObj->z_sample_instance,
                              pDerivedObj->sfx_gain);

  pDerivedObj->t_sample_instance =
      al_create_sample_instance(pDerivedObj->t_mino);
  al_set_sample_instance_playmode(pDerivedObj->t_sample_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->t_sample_instance,
                                     al_get_default_mixer());
  // set the volume of instance
  al_set_sample_instance_gain(pDerivedObj->t_sample_instance,
                              pDerivedObj->sfx_gain);

  pDerivedObj->s_sample_instance =
      al_create_sample_instance(pDerivedObj->s_mino);
  al_set_sample_instance_playmode(pDerivedObj->s_sample_instance,
                                  ALLEGRO_PLAYMODE_ONCE);
  al_attach_sample_instance_to_mixer(pDerivedObj->s_sample_instance,
                                     al_get_default_mixer());
  // set the volume of instance
  al_set_sample_instance_gain(pDerivedObj->s_sample_instance,
                              pDerivedObj->sfx_gain);

  pDerivedObj->bgm_choice = rand() % 2;
  if (pDerivedObj->bgm_choice == 0) {
    pDerivedObj->bgm_choice_sample = pDerivedObj->bgm_piercing_wind;
    pDerivedObj->bgm_choice_sample_instance =
        pDerivedObj->bgm_piercing_wind_instance;
  }
  if (pDerivedObj->bgm_choice == 1) {
    pDerivedObj->bgm_choice_sample = pDerivedObj->bgm_original_tetris;
    pDerivedObj->bgm_choice_sample_instance =
        pDerivedObj->bgm_original_tetris_instance;
  }
  al_play_sample_instance(pDerivedObj->bgm_choice_sample_instance);

  pDerivedObj->start_music = 300;
  pDerivedObj->start_music_timer = 0;
  pDerivedObj->zombie_spawn_time_interval = (int)FPS * 30;
  pDerivedObj->zombie_spawn_timer = 0;
  pObj->Update = game_scene_update;
  pObj->Draw = game_scene_draw;
  pObj->Destroy = game_scene_destroy;
  return pObj;
}
void game_scene_update(Scene *self) {
  GameScene *gamescene = (GameScene *)(self->pDerivedObj);
  ElementVec labelelem = _Get_label_elements(self, Tetris_board_L);
  Tetris_board *board = (Tetris_board *)labelelem.arr[0]->pDerivedObj;
  gamescene->zombie_spawn_timer++;
  if (!(gamescene->zombie_spawn_timer %
        gamescene->zombie_spawn_time_interval) &&
      !board->game_over) {
    _Register_elements(self, New_zombie(Zombie_L));
  }
  labelelem = _Get_label_elements(self, Zombie_L);
  if (board->game_over && labelelem.len) {
    for (int i = 0; i < labelelem.len; ++i) {
      Zombie *zombie = (Zombie *)labelelem.arr[i]->pDerivedObj;
      zombie->stop_walk = true;
    }
  }
  if (gamescene->start_music_timer > 0 &&
      gamescene->start_music_timer <= gamescene->start_music &&
      !(gamescene->start_music_timer % 50)) {
    al_set_sample_instance_gain(gamescene->bgm_original_tetris_instance,
                                gamescene->bgm_gain +
                                    0.005 * ((float)gamescene->start_music /
                                             gamescene->start_music_timer));
    gamescene->start_music_timer++;
  }

  // update every element
  ElementVec allEle = _Get_all_elements(self);
  for (int i = allEle.len - 1; i >= 0; --i) {
    Elements *ele = allEle.arr[i];
    ele->Update(ele);
  }
  // run interact for every element
  for (int i = allEle.len - 1; i >= 0; --i) {
    Elements *ele = allEle.arr[i];
    ele->Interact(ele);
  }
  // remove element
  for (int i = allEle.len - 1; i >= 0; --i) {
    Elements *ele = allEle.arr[i];
    if (ele->dele) {
      _Remove_elements(self, ele);
    }
  }
  if (key_state[ALLEGRO_KEY_ESCAPE]) {
    self->scene_end = true;
    window = 0;
  }
}
void game_scene_draw(Scene *self) {
  GameScene *gs = ((GameScene *)(self->pDerivedObj));
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_tinted_scaled_bitmap(gs->background, al_map_rgb(100, 100, 100), 0, 0,
                               6000, 3375, 0, 0, WIDTH, HEIGHT, 0);
  ElementVec allEle = _Get_all_elements(self);
  for (int i = 0; i < allEle.len; i++) {
    Elements *ele = allEle.arr[i];
    ele->Draw(ele);
  }
}
void game_scene_destroy(Scene *self) {
  GameScene *Obj = ((GameScene *)(self->pDerivedObj));
  ElementVec allEle = _Get_all_elements(self);
  for (int i = 0; i < allEle.len; i++) {
    Elements *ele = allEle.arr[i];
    ele->Destroy(ele);
  }
  al_destroy_sample(Obj->i_mino);
  al_destroy_sample_instance(Obj->i_sample_instance);
  al_destroy_sample(Obj->j_mino);
  al_destroy_sample_instance(Obj->j_sample_instance);
  al_destroy_sample(Obj->l_mino);
  al_destroy_sample_instance(Obj->l_sample_instance);
  al_destroy_sample(Obj->z_mino);
  al_destroy_sample_instance(Obj->z_sample_instance);
  al_destroy_sample(Obj->t_mino);
  al_destroy_sample_instance(Obj->t_sample_instance);
  al_destroy_sample(Obj->s_mino);
  al_destroy_sample_instance(Obj->s_sample_instance);
  al_destroy_sample(Obj->o_mino);
  al_destroy_sample_instance(Obj->o_sample_instance);
  al_destroy_sample(Obj->bgm_piercing_wind);
  al_destroy_sample_instance(Obj->bgm_piercing_wind_instance);
  al_destroy_sample(Obj->b2b_1);
  al_destroy_sample_instance(Obj->b2b_1_instance);
  al_destroy_sample(Obj->b2b_2);
  al_destroy_sample_instance(Obj->b2b_2_instance);
  al_destroy_sample(Obj->b2b_3);
  al_destroy_sample_instance(Obj->b2b_3_instance);
  al_destroy_sample(Obj->b2b_break);
  al_destroy_sample_instance(Obj->b2b_break_instance);

  for (int i = 0; i < 16; ++i) {
    al_destroy_sample(Obj->combo[i]);
    al_destroy_sample_instance(Obj->combo_instance[i]);
  }

  al_destroy_sample(Obj->all_clear);
  al_destroy_sample_instance(Obj->all_clear_instance);

  al_destroy_sample(Obj->bgm_original_tetris);
  al_destroy_sample_instance(Obj->bgm_original_tetris_instance);

  al_destroy_sample(Obj->game_over_sfx);
  al_destroy_sample_instance(Obj->game_over_sfx_instance);
  al_destroy_bitmap(Obj->background);
  algif_destroy_animation(Obj->zombie_gif);
  free(Obj);
  free(self);
}
