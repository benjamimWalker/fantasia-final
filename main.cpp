#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <cmath>
#include <iostream>

using namespace std;

int main() {
    //creating pointer variables
    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_EVENT_QUEUE *eventQueue;
    ALLEGRO_BITMAP *bitmap;
    ALLEGRO_TIMER *timer;
    enum Direction {DOWN, LEFT, RIGHT, UP};
    if (!al_init()) {
        al_show_native_message_box(display, "Temos problemas", "O jogo não conseguiu executar","Veja se as instruções foram seguidas corretamente e  tente recompilar com algumas mudanças",nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    }
    timer = al_create_timer(1.0 / 300.0);
    display = al_create_display(860, 483);
    if (!display) {
        al_show_native_message_box(display, "Temos problemas", "A tela não conseguiu carregar","Veja se as instruções foram seguidas corretamente e  tente recompilar com algumas mudanças",nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    }
    al_set_window_title(display, "Fantasia Final");
    eventQueue = al_create_event_queue();
    al_install_keyboard();
    al_install_mouse();
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_init_image_addon();

    bitmap = al_load_bitmap("../assets/sprites/characters/Actors_1.png");
    float spriteWidth =  (float) al_get_bitmap_width(bitmap) / 6;
    float spriteHeight =  (float) al_get_bitmap_height(bitmap) / 4;

    bool running = true;
    float x = 0, y = 0, moveSpeed = 2.2;
    int direction;
    float sX = spriteWidth, sY = 0;
    bool isSpriteInNeedToUpdateByKeyInput = false;

    al_start_timer(timer);
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            if (al_show_native_message_box(display, "Confirmação de saída", "Tem certeza que quer sair?", "", nullptr, ALLEGRO_MESSAGEBOX_YES_NO) == 1) {
                running = false;
            }
        }
        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(53, 211, 221));
            al_draw_bitmap_region(bitmap, sX, direction * spriteHeight, spriteWidth, spriteHeight, x, y, 0);
            al_flip_display();


            ALLEGRO_KEYBOARD_STATE keyState;
            al_get_keyboard_state(&keyState);

            isSpriteInNeedToUpdateByKeyInput = true;
            if (al_key_down(&keyState, ALLEGRO_KEY_A)) {
                x -= moveSpeed;
                direction = LEFT;
            } else if (al_key_down(&keyState, ALLEGRO_KEY_D)) {
                x += moveSpeed;
                direction = RIGHT;
            } else if (al_key_down(&keyState, ALLEGRO_KEY_W)) {
                direction = UP;
                y -= moveSpeed;
            } else if (al_key_down(&keyState, ALLEGRO_KEY_S)) {
                y += moveSpeed;
                direction = DOWN;
            } else {
                isSpriteInNeedToUpdateByKeyInput = false;
            }
            sY = direction;
            if (isSpriteInNeedToUpdateByKeyInput) sX += spriteWidth;
            else sX = spriteWidth;
            cout << sX << endl;
            if (sX >= 192) sX = 0;
        }

    }

    // Cleaning garbage
    al_destroy_display(display);
    al_uninstall_keyboard();
    al_uninstall_mouse();
    al_destroy_bitmap(bitmap);
    al_destroy_timer(timer);

    return 0;
    //END OF CODE
}
