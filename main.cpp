#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <vector>
#include <iostream>
#include "player.cpp"
#include "audiomanager.cpp"
#include "gamemanager.cpp"
#include <unistd.h>

using namespace std;


//global gamemanager's static variables
map<vector<Monster>, pair<int, int>> GameManager::enemiesLocalization;
unsigned short GameManager::gameMode;
unsigned short GameManager::numEnemies = 42; //The answer
unsigned short GameManager::level = 1;

int main() {

    //audio files id
    const unsigned short int prologueId = 0;
    const unsigned short int generalID = 1;
    const unsigned short int battleId = 2;
    const unsigned short int victoryId = 3;

    //creating pointer variables
    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_EVENT_QUEUE *eventQueue;
    ALLEGRO_BITMAP *playerSprite;
    ALLEGRO_BITMAP *map1;
    ALLEGRO_BITMAP *map2;
    ALLEGRO_BITMAP *map3;
    ALLEGRO_BITMAP *chest1;
    ALLEGRO_BITMAP *chest2;
    ALLEGRO_BITMAP *chest3;
    ALLEGRO_TIMER *timer;

    const unsigned short windowWidth = 919;
    const unsigned short windowHeight = 517; //window properties

    if (!al_init()) {
        al_show_native_message_box(display, "Temos problemas", "O jogo não conseguiu executar","Veja se as instruções foram seguidas corretamente e  tente recompilar com algumas mudanças",nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    }

    //display timer and fps creation
    const int FPS = 60;
    timer = al_create_timer(1.0 / FPS);
    display = al_create_display(windowWidth, windowHeight);
    if (!display) {
        al_show_native_message_box(display, "Temos problemas", "A tela não conseguiu carregar","Veja se as instruções foram seguidas corretamente e  tente recompilar com algumas mudanças",nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    }

    // getting game settings done
    enum Direction {DOWN, LEFT, RIGHT, UP};
    enum GameModes{EXPLORING, FIGHTING};
    al_set_window_title(display, "Fantasia Final");
    eventQueue = al_create_event_queue();
    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_primitives_addon();

    // creating the instance of the audio manager
    AudioManger audioManger;

    // creating the instance of the game manager
    GameManager gameManager;

    // setting player character
    Player player = Player(0, 0, 1.2, "alan");
    player.setDimensions();
    playerSprite = al_load_bitmap(player.spritePath.c_str());
    float playerSpriteWidth =  player.individualSpriteX;
    float playerSpriteHeight =  player.individualSpriteY;

    // loading images for the 3 maps and chests
    map1 = al_load_bitmap("../assets/sprites/maps/mapa1.png");
    chest1 = al_load_bitmap("../assets/sprites/chest1.png");
    map2 = al_load_bitmap("../assets/sprites/maps/mapa2.png");
    chest2 = al_load_bitmap("../assets/sprites/chest2.png");
    map3 = al_load_bitmap("../assets/sprites/maps/mapa3.png");
    chest3 = al_load_bitmap("../assets/sprites/chest3.png");

    int spriteSheetAnimationRefreshFPS = 0;
    bool running = true;
    int direction; //range from zero to four for changing the image's direction
    float sX = playerSpriteWidth; //the width of each individual sprite in the sprite sheet
    bool isSpriteInNeedToUpdateByKeyInput = false;
    al_start_timer(timer);

    gameManager.sortPositions((int) (windowWidth - playerSpriteWidth) - 2, (int) (windowHeight- playerSpriteWidth) - 2); //spreading monsters
    GameManager::gameMode = EXPLORING;

    // main loop
    while (running) {

        // detects whether the player has found a monster and changes the game mode
        if (gameManager.foundMonster(player)) {
            //TODO UNCOMMENT
         //   GameManager::gameMode = FIGHTING;
        }

        if (GameManager::gameMode == EXPLORING) {
            al_draw_bitmap(map1, 0.0, 0.0, 0);
            if (player.foundChest())
                al_draw_tinted_bitmap(chest1, al_map_rgb(168, 118, 204), windowWidth - 63, 8, 0);
            else
                al_draw_bitmap(chest1, windowWidth - 63, 8, 0);
            ALLEGRO_EVENT event;
            al_wait_for_event(eventQueue, &event);

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                //TODO descomentar essa parte no final
                //   if (al_show_native_message_box(display, "Confirmação de saída", "Tem certeza que quer sair?", "", nullptr, ALLEGRO_MESSAGEBOX_YES_NO) == 1)
                running = false;
            }
            if (event.type == ALLEGRO_EVENT_TIMER) {
                //call to function for detecting player trying to cross the limits and make sure it doesn't do that
                player.borderCollision();

                // TODO [DEBUG DRAWING]
                for (const auto &enemy: GameManager::enemiesLocalization) {
                    for (auto monster: enemy.first) {
                        al_draw_filled_circle(enemy.second.first, enemy.second.second, 13, al_map_rgb(255, 255, 255));
                    }
                }

                // TODO [DEBUG DRAWING]
                al_draw_filled_circle(player.x, player.y, 4, al_map_rgb(255, 255, 255));


                //drawing player
                al_draw_bitmap_region(playerSprite, sX, (float) direction * playerSpriteHeight, playerSpriteWidth,
                                      playerSpriteHeight, player.x, player.y, 0);
                al_flip_display();
                //playing the themesong
                if (!audioManger.isPlaying) {
                    audioManger.playLoop(generalID);
                }
                // setting keyboard input
                ALLEGRO_KEYBOARD_STATE keyState;
                al_get_keyboard_state(&keyState);
                isSpriteInNeedToUpdateByKeyInput = true;
                if (al_key_down(&keyState, ALLEGRO_KEY_A)) {
                    player.x -= player.moveSpeed;
                    direction = LEFT;
                } else if (al_key_down(&keyState, ALLEGRO_KEY_D)) {
                    audioManger.playOnce("passo");
                    player.x += player.moveSpeed;
                    direction = RIGHT;
                } else if (al_key_down(&keyState, ALLEGRO_KEY_W)) {
                    direction = UP;
                    player.y -= player.moveSpeed;
                } else if (al_key_down(&keyState, ALLEGRO_KEY_S)) {
                    player.y += player.moveSpeed;
                    direction = DOWN;
                } else {
                    isSpriteInNeedToUpdateByKeyInput = false;
                }

                //handles the characters animation
                if (spriteSheetAnimationRefreshFPS == 10) {
                    if (isSpriteInNeedToUpdateByKeyInput) sX += playerSpriteWidth;
                    else sX = playerSpriteWidth;
                    if (sX >= (float) player.totalSpriteX) sX = 0;
                }
                spriteSheetAnimationRefreshFPS++;
                if (spriteSheetAnimationRefreshFPS == 11) spriteSheetAnimationRefreshFPS = 0;
            }
        }

        else if (GameManager::gameMode == FIGHTING){
            /*
             *
             * BATTLE CODE HERE
             */
        }
    }

    // Cleaning garbage
    al_destroy_display(display);
    al_uninstall_keyboard();
    al_uninstall_mouse();
    al_uninstall_audio();
    al_destroy_bitmap(playerSprite);
    al_destroy_bitmap(map1);
    al_destroy_timer(timer);

    return 0;
    //END OF CODE
}