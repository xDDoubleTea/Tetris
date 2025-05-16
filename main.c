#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <stdio.h>

#include "./tetriminos.h"
#define GAME_TERMINATE 666

#define AL_INIT_FAILED -1
#define DISPLAY_INIT_FAILED -2
#define QUEUE_INIT_FAILED -3
#define FONT_INIT_FAILED -4
#define AUDIO_INIT_FAILED -5

const int width = 1280;
const int height = 720;

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* queue = NULL;
ALLEGRO_EVENT event;
ALLEGRO_FONT* font = NULL;
ALLEGRO_SAMPLE* sample = NULL;
ALLEGRO_SAMPLE_INSTANCE* startSound = NULL;

int key_pressed = 0;

void game_init();
void game_begin();
void game_destroy();
void game_draw();

int process_event();
int game_update();
int keyboard_event();
int game_run();

void draw_block(int coord_x, int coord_y, char block_type) {
    BLOCK new_block = {25, coord_x, coord_x,
                       al_map_rgb(o_color[0], o_color[1], o_color[2])};
    al_draw_filled_rectangle(new_block.coord_x, new_block.coord_y,
                             new_block.coord_x + new_block.side_len,
                             new_block.coord_y + new_block.side_len,
                             new_block.color);
}

int main(int argc, char* argv[]) {
    int msg = 0;

    game_init();
    game_begin();
    printf("Hello world!!!\n");
    printf("Close window to terminate.\n");

    while (msg != GAME_TERMINATE) {
        msg = game_run();
        if (msg == GAME_TERMINATE) printf("See you, world\n");
    }
    game_destroy();
    return 0;
}

void show_err_msg(int msg) {
    fprintf(stderr, "unexpected msg: %d\n", msg);
    game_destroy();
    exit(9);
}

void game_begin() {
    startSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(startSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(startSound, al_get_default_mixer());
    al_play_sample_instance(startSound);
    al_set_sample_instance_gain(startSound, 0.2);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_block(100, 100, 'o');
    al_flip_display();
}
void game_init() {
    if (!al_init()) {
        show_err_msg(AL_INIT_FAILED);
    }

    /*
     * TODO: initialize display and event queue, and register specific event
     * into event queue.
     */
    al_install_keyboard();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(16);
    queue = al_create_event_queue();
    display = al_create_display(width, height);
    font = al_create_builtin_font();
    sample = al_load_sample("F1.wav");
    if (display == NULL) {
        show_err_msg(DISPLAY_INIT_FAILED);
    }
    if (!queue) {
        show_err_msg(QUEUE_INIT_FAILED);
    }
    if (!font) {
        show_err_msg(FONT_INIT_FAILED);
    }
    if (!sample) {
        show_err_msg(AUDIO_INIT_FAILED);
    }
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
}
void game_draw() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_block(100, 100, 'o');
    al_draw_textf(font, al_map_rgb(255, 255, 255), (float)width / 2,
                  (float)height / 2, ALLEGRO_ALIGN_CENTER,
                  "Hello world!!! key_pressed = %d", key_pressed);
    al_flip_display();
}
int key_event() {
    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
        return GAME_TERMINATE;
    }
    key_pressed = event.keyboard.keycode;
    return 0;
}
int process_event() {
    al_wait_for_event(queue, &event);
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_TERMINATE;
    }
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        return key_event();
    }
    game_update();
    return 0;
}
int game_run() {
    int err = 0;
    if (!al_is_event_queue_empty(queue)) {
        err = process_event();
    }
    game_draw();
    return err;
}
int game_update() { return 0; }

void game_destroy() {
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_font(font);
    al_destroy_sample(sample);
}
