#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <stdio.h>

int main() {
    al_init();
    al_install_keyboard();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    // 1 second = 30 tick
    // second per tick
    // Minecraft runs at 20 ticks per second, so it would be 1.0 / 20.0

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    // creates a display with 854x480 resolution
    ALLEGRO_DISPLAY* disp = al_create_display(854, 480);

    // Loads fonts
    al_init_font_addon();
    al_init_ttf_addon();
    /* ALLEGRO_FONT* font = al_create_builtin_font(); */
    ALLEGRO_FONT* myfont = al_load_ttf_font("./FiraCodeNerdFont-Regular.ttf",
                                            24, ALLEGRO_TTF_NO_KERNING);
    if (!myfont) {
        fprintf(stderr, "Failed to load font!\n");
        return -1;
    }
    float line_height = al_get_font_line_height(myfont);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    printf("Event sources registered\n");
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while (1) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER)
            redraw = true;
        else if ((event.type == ALLEGRO_EVENT_KEY_DOWN) ||
                 (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE))
            break;

        if (redraw && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(37, 128, 255));
            /* al_draw_text(myfont, al_map_rgb(255, 128, 0), 0, 0, 0, */
            /*              "Hello world!\nPress any key to exit."); */
            al_draw_multiline_text(myfont, al_map_rgb(255, 128, 0), 0, 0, 850,
                                   line_height, 0,
                                   "Hello world!\nPress any to exit.");
            al_flip_display();

            redraw = false;
        }
    }
    printf("Exiting...\n");
    al_destroy_font(myfont);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
