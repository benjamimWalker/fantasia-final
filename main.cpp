#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <cmath>
#include <iostream>
#include "player.cpp"

using namespace std;

int main() {
    //creating pointer variables
    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_EVENT_QUEUE *eventQueue;
    ALLEGRO_BITMAP *playerSprite;
    ALLEGRO_TIMER *timer;

    if (!al_init()) {
        al_show_native_message_box(display, "Temos problemas", "O jogo não conseguiu executar","Veja se as instruções foram seguidas corretamente e  tente recompilar com algumas mudanças",nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    }

    //display timer and fps creation
    const int FPS = 60;
    timer = al_create_timer(1.0 / FPS);
    display = al_create_display(860, 483);
    if (!display) {
        al_show_native_message_box(display, "Temos problemas", "A tela não conseguiu carregar","Veja se as instruções foram seguidas corretamente e  tente recompilar com algumas mudanças",nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    }

    // getting game settings done
    enum Direction {DOWN, LEFT, RIGHT, UP};
    al_set_window_title(display, "Fantasia Final");;
    eventQueue = al_create_event_queue();
    al_install_keyboard();
    al_install_mouse();
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_init_image_addon();

    // setting player character
    Player player = Player(0, 0, 1.2, "alan");
    player.setDimensions();
    playerSprite = al_load_bitmap(player.spritePath.c_str());
    float playerSpriteWidth =  player.individualSpriteX;
    float playerSpriteHeight =  player.individualSpriteY;
    int spriteSheetAnimationRefreshFPS = 0;
    bool running = true;
    float x = 0, y = 0; //coordinates for drawing each image
    int direction; //range from zero to four for changing the image's direction
    float sX = playerSpriteWidth; //the width of each individual sprite in the sprite sheet
    bool isSpriteInNeedToUpdateByKeyInput = false;

    al_start_timer(timer);

    // main loop
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(eventQueue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            //TODO descomentar essa parte no final
         //   if (al_show_native_message_box(display, "Confirmação de saída", "Tem certeza que quer sair?", "", nullptr, ALLEGRO_MESSAGEBOX_YES_NO) == 1)
                running = false;
        }
        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(53, 211, 221));
            al_draw_bitmap_region(playerSprite, sX, (float) direction * playerSpriteHeight, playerSpriteWidth, playerSpriteHeight, x, y, 0);
            al_flip_display();

            // setting keyboard input
            ALLEGRO_KEYBOARD_STATE keyState;
            al_get_keyboard_state(&keyState);
            isSpriteInNeedToUpdateByKeyInput = true;
            if (al_key_down(&keyState, ALLEGRO_KEY_A)) {
                x -= player.moveSpeed;
                direction = LEFT;
            } else if (al_key_down(&keyState, ALLEGRO_KEY_D)) {
                x += player.moveSpeed;
                direction = RIGHT;
            } else if (al_key_down(&keyState, ALLEGRO_KEY_W)) {
                direction = UP;
                y -= player.moveSpeed;
            } else if (al_key_down(&keyState, ALLEGRO_KEY_S)) {
                y += player.moveSpeed;
                direction = DOWN;
            } else {
                isSpriteInNeedToUpdateByKeyInput = false;
            }

            //handles the characters animation
            if (spriteSheetAnimationRefreshFPS == 10){
                if (isSpriteInNeedToUpdateByKeyInput) sX += playerSpriteWidth;
                else sX = playerSpriteWidth;
                cout << sX << endl;
                if (sX >= (float) player.totalSpriteX) sX = 0;
            }
            spriteSheetAnimationRefreshFPS += 1;
            if (spriteSheetAnimationRefreshFPS == 11) spriteSheetAnimationRefreshFPS = 0;
        }
    }

    // Cleaning garbage
    al_destroy_display(display);
    al_uninstall_keyboard();
    al_uninstall_mouse();
    al_destroy_bitmap(playerSprite);
    al_destroy_timer(timer);

    return 0;
    //END OF CODE
}
