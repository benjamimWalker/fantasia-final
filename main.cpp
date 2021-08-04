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
#include "uimanager.cpp"

#include <unistd.h>
// REALLY BIG TODO GARANTIR QUE O PERSONAGEM NÃO COMECE JÁ ENCIMA DE UM BANDO DE INIMIGOS

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
    ALLEGRO_BITMAP *playerBattleSprite;
    ALLEGRO_BITMAP *map1;
    ALLEGRO_BITMAP *map2;
    ALLEGRO_BITMAP *map3;
    ALLEGRO_BITMAP *battleBitmaps[3];
    ALLEGRO_BITMAP *chest1;
    ALLEGRO_BITMAP *chest2;
    ALLEGRO_BITMAP *chest3;
    ALLEGRO_TIMER *expTimer;
    ALLEGRO_TIMER *fightTimer;
    ALLEGRO_SAMPLE *generalMusic;
    ALLEGRO_SAMPLE *battleMusic;

    const unsigned short windowWidth = 919;
    const unsigned short windowHeight = 517; //window properties

    if (!al_init()) {
        al_show_native_message_box(display, "Temos problemas", "O jogo não conseguiu executar","Veja se as instruções foram seguidas corretamente e  tente recompilar com algumas mudanças",nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    }

    //display expTimer and fps creation
    const int FPS = 60;
    expTimer = al_create_timer(1.0 / FPS);
    fightTimer = al_create_timer(1.0 / 5);
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
    al_register_event_source(eventQueue, al_get_timer_event_source(expTimer));
    al_register_event_source(eventQueue, al_get_timer_event_source(fightTimer));
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_primitives_addon();

    // creating the instance of the audio manager
    AudioManger audioManger;

    // creating the instance of the game manager
    GameManager gameManager;

    // creating the instance of the ui manager
    UIManager uiManager;

    // setting player character
    Player player = Player(20, 0, 1.2, "alan");
    player.setDimensions();
    playerSprite = al_load_bitmap(player.spritePath.c_str());
    playerBattleSprite = al_load_bitmap(player.battlePath.c_str());
    float playerSpriteWidth =  player.individualSpriteX;
    float playerSpriteHeight =  player.individualSpriteY;

    // loading images for the 3 maps and chests
    map1 = al_load_bitmap("../assets/sprites/maps/mapa1.png");
    chest1 = al_load_bitmap("../assets/sprites/chest1.png");
    map2 = al_load_bitmap("../assets/sprites/maps/mapa2.png");
    chest2 = al_load_bitmap("../assets/sprites/chest2.png");
    map3 = al_load_bitmap("../assets/sprites/maps/mapa3.png");
    chest3 = al_load_bitmap("../assets/sprites/chest3.png");

    //Loading battle images in an array
    battleBitmaps[0] = al_load_bitmap("../assets/sprites/maps/battlemap1.png");
    battleBitmaps[1] = al_load_bitmap("../assets/sprites/maps/battlemap2.png");
    battleBitmaps[2] = al_load_bitmap("../assets/sprites/maps/battlemap3.png");

    int spriteSheetAnimationRefreshFPS = 0;
    bool running = true;
    int direction; //range from zero to four for changing the image's direction
    float sX = playerSpriteWidth; //the width of each individual sprite in the sprite sheet
    bool isSpriteInNeedToUpdateByKeyInput = false;
    al_start_timer(expTimer);
    al_start_timer(fightTimer);

    srand (time (0));
    gameManager.sortPositions((int) (windowWidth - playerSpriteWidth) - 2, (int) (windowHeight- playerSpriteWidth) - 2); //spreading monsters
    GameManager::gameMode = EXPLORING;
    vector<Monster> currentMonster; //Monsters loaded when entering in battle mode

    bool isOnBattle; //Set to true when an enemy is found
    float sXM1 =  0; //Location to start drawing the first monster image of the spritesheet
    float sXM2 =  0; //Location to start drawing the first monster image of the spritesheet
    float sXM3 =  0; //Location to start drawing the first monster image of the spritesheet
    // main loop
    while (running) {
        int battleBitMapIndex; //Index for the battleBitMapArray which will be a random number

        if (GameManager::gameMode == EXPLORING) {
            //playing the themesong
            if (!audioManger.isPlaying) {
                generalMusic = audioManger.playLoop(generalID);
            }

            // detects whether the player has found a monster and changes the game mode
            if (gameManager.foundMonster(player, &currentMonster) and not player.exempted) {
                GameManager::gameMode = FIGHTING; //boolean to enter the fight (battle) mode
                audioManger.stopPlaying(generalMusic); //stopping the music
            }
            //If player exit monster area he can find another
            else if(not gameManager.foundMonster(player, &currentMonster)){
                player.exempted = false;
            }
            isOnBattle = true;
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
            if (event.type == ALLEGRO_EVENT_TIMER and event.timer.source == expTimer) {
                //call to function for detecting player trying to cross the limits and make sure it doesn't do that
                player.borderCollision();

                // TODO [DEBUG DRAWING]
                for (const auto &enemy: GameManager::enemiesLocalization) {
                    for (auto monster: enemy.first) {
                        //al_draw_filled_circle(enemy.second.first, enemy.second.second, 13, al_map_rgb(255, 255, 255));
                    }
                }

                // TODO [DEBUG DRAWING]
              //  al_draw_filled_circle(player.x, player.y, 4, al_map_rgb(255, 255, 255));

                uiManager.drawLifebar();
                //drawing player
                al_draw_bitmap_region(playerSprite, sX, (float) direction * playerSpriteHeight, playerSpriteWidth, playerSpriteHeight, player.x, player.y, 0);
                al_flip_display();

                // setting keyboard input
                ALLEGRO_KEYBOARD_STATE keyState;
                al_get_keyboard_state(&keyState);
                isSpriteInNeedToUpdateByKeyInput = true;
                if (al_key_down(&keyState, ALLEGRO_KEY_A)) {
                    player.x -= player.moveSpeed;
                    direction = LEFT;
                } else if (al_key_down(&keyState, ALLEGRO_KEY_D)) {
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

        else if (GameManager::gameMode == FIGHTING){ //Starts the FIGHT
            /* battleBitMapIndex will always have a value because
             * isOnBattle is previously set to true
             * */

            //playing the themesong of the battle
            if (!audioManger.isPlaying) {
                battleMusic = audioManger.playLoop(battleId);
            }

            if (isOnBattle){
                battleBitMapIndex = 1; //TODO COLOCAR O RANDO DE 3 QUANDO O ESQUEMA DE FASES ESTIVER PRONTO
                for (auto & m : currentMonster) {
                    m.prepareDrawing();
                }

            }
            al_draw_bitmap(battleBitmaps[battleBitMapIndex], 0.0, 0.0, 0);
            ALLEGRO_EVENT event;
            al_wait_for_event(eventQueue, &event);

            //Important for closing program with the window
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                //TODO descomentar essa parte no final
                //   if (al_show_native_message_box(display, "Confirmação de saída", "Tem certeza que quer sair?", "", nullptr, ALLEGRO_MESSAGEBOX_YES_NO) == 1)
                running = false;
                for (auto & m : currentMonster) {
                    m.clean();
                }
            }
            if (event.type == ALLEGRO_EVENT_TIMER and event.timer.source == fightTimer){
                // Drawing each one of the monster in this for loop
                for (int i = 0; i < currentMonster.size(); i++){
                    auto width = (float) al_get_bitmap_width(currentMonster[i].bitmap);
                    auto height = (float) al_get_bitmap_height(currentMonster[i].bitmap);
                    if (i == 0){
                        if(sXM1 >= (width - width / 3) - 1) sXM1 = 0;
                        else sXM1 += width / 3;
                        al_draw_bitmap_region(currentMonster[i].bitmap, sXM1, 0,  width/ 3, height , 40, 200, 0);
                    }
                    else if(i == 1){
                        if(sXM2 >= (width - width/ 3) - 1) sXM2 = 0;
                        else sXM2 += width / 3;
                        al_draw_bitmap_region(currentMonster[i].bitmap, sXM2, 0, width / 3, height, 80 + (float) al_get_bitmap_width(currentMonster[i-1].bitmap) / 3, 200, 0);
                    }
                    else{
                        if(sXM3 >= (width - width/ 3) - 1) sXM3 = 0;
                        else sXM3 += width / 3;
                        al_draw_bitmap_region(currentMonster[i].bitmap, sXM3, 0, width / 3, height, 120 +  (float) al_get_bitmap_width(currentMonster[i-1].bitmap) / 3 + (float) al_get_bitmap_width(currentMonster[i-2].bitmap) / 3, 200, 0);
                    }
                } //Done drawing monster(s) for this frame

                al_draw_bitmap(playerBattleSprite, 800, 220, 0); //Drawing player's sprite
                al_flip_display();
            }

//            GameManager::gameMode = EXPLORING;
//          player.exempted = true;
           isOnBattle = false;

        }
    }

    audioManger.stopPlaying(battleMusic); // só pra não acabar com minha ram
    // Cleaning garbage
    al_destroy_display(display);
    al_uninstall_keyboard();
    al_uninstall_mouse();
    al_uninstall_audio();
    al_destroy_bitmap(playerSprite);
    al_destroy_bitmap(playerBattleSprite);
    al_destroy_bitmap(map1);
    al_destroy_bitmap(map2);
    al_destroy_bitmap(map3);
    al_destroy_bitmap(chest1);
    al_destroy_bitmap(chest2);
    al_destroy_bitmap(chest3);
    al_destroy_bitmap(battleBitmaps[0]);
    al_destroy_bitmap(battleBitmaps[1]);
    al_destroy_bitmap(battleBitmaps[2]);
    al_destroy_event_queue(eventQueue);
    al_destroy_timer(expTimer);
    al_destroy_timer(fightTimer);

    return 0;
    //END OF CODE
}