#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <math.h>

using namespace std;

int main() {
    ALLEGRO_DISPLAY *tela;
    ALLEGRO_EVENT_QUEUE *fila;
    ALLEGRO_BITMAP *bitmap;
    ALLEGRO_TIMER *timer;
    al_init();
    timer = al_create_timer(1.0 / 300.0);
    tela = al_create_display(720, 480);
    fila = al_create_event_queue();
    al_install_keyboard();
    al_install_mouse();
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_mouse_event_source());
    al_register_event_source(fila, al_get_display_event_source(tela));
    al_register_event_source(fila, al_get_timer_event_source(timer));
    al_init_image_addon();

    bitmap = al_load_bitmap("../assets/sprites/dexter.png");
    bool executando = true;

    float x = 0, y = 0, r = 0;
    al_start_timer(timer);
    while (executando) {

        ALLEGRO_EVENT evento;
        al_wait_for_event(fila, &evento);

        if (evento.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(53, 211, 221));
            al_draw_rotated_bitmap(bitmap, al_get_bitmap_width(bitmap) / 2, al_get_bitmap_height(bitmap) / 2, x, y, r, 0);
            al_flip_display();
        }
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            executando = false;
        }


        ALLEGRO_KEYBOARD_STATE keyState;
        al_get_keyboard_state(&keyState);

        if (al_key_down(&keyState, ALLEGRO_KEY_A)) {
            x -= 3;
            r = -M_PI / 2;
        } else if (al_key_down(&keyState, ALLEGRO_KEY_D)) {
            r = M_PI / 2;
            x += 3;
        } else if (al_key_down(&keyState, ALLEGRO_KEY_W)) {
            r = 0.0;
            y -= 3;
        } else if (al_key_down(&keyState, ALLEGRO_KEY_S)) {
            r = -M_PI;
            y += 3;
        }
    }

    al_destroy_display(tela);
    al_uninstall_keyboard();
    al_uninstall_mouse();
    al_destroy_bitmap(bitmap);
    al_destroy_timer(timer);
    return al_show_native_message_box(tela, "Title", "Heading", "Text", nullptr, 16 | 8);
}
