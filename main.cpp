#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
using namespace std;

int main() {
    ALLEGRO_DISPLAY *tela;
    ALLEGRO_EVENT_QUEUE *fila;
    ALLEGRO_BITMAP *bitmap;
    al_init();
    tela = al_create_display(720, 480);
    fila = al_create_event_queue();
    al_install_keyboard();
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_display_event_source(tela));
    al_init_image_addon();


    bitmap = al_load_bitmap("dexter.png");
    bool executando = true;

    while (executando) {
        al_flip_display();
        al_clear_to_color(al_map_rgb(53, 211, 221));
        al_draw_bitmap(bitmap, 50, 20, 0);

        ALLEGRO_EVENT evento;

        al_wait_for_event(fila, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_CHAR || evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            executando = false;
        }
    }

    al_destroy_display(tela);
    al_uninstall_keyboard();
    al_destroy_bitmap(bitmap);
    return al_show_native_message_box(tela, "Title", "Heading", "Text", nullptr, 16 | 8);
}

















