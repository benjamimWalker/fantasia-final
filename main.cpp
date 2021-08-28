#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include "player.cpp"
#include "audiomanager.cpp"
#include "gamemanager.cpp"
#include "uimanager.cpp"
#include "scoremanager.cpp"

// Global game manager static variables
map<pair<int, int>, vector<Monster>> GameManager::enemiesLocalization;
u_short GameManager::gameMode;
u_short GameManager::numEnemies = 36;
u_short GameManager::level = 1;
string GameManager::selectedHero;

int main() {
    GameManager::selectedHero = "ada";

    // Audio files id
    const u_int prologueId = 0;
    const u_int generalID = 1;
    const u_int battleId = 2;
    const u_int victoryId = 3;
    const u_int loseId = 4;

    // Creating pointer variables
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
    ALLEGRO_TIMER *exploringTimer;
    ALLEGRO_TIMER *fightTimer;
    ALLEGRO_SAMPLE *prologueMusic;
    ALLEGRO_SAMPLE *generalMusic;
    ALLEGRO_SAMPLE *battleMusic;
    ALLEGRO_SAMPLE *victoryMusic;
    ALLEGRO_SAMPLE *loseMusic;
    ALLEGRO_BITMAP *adaDeathScreen;
    ALLEGRO_BITMAP *adaWinScreen;
    ALLEGRO_BITMAP *adaWinRecordScreen;
    ALLEGRO_BITMAP *alanDeathScreen;
    ALLEGRO_BITMAP *alanWinScreen;
    ALLEGRO_BITMAP *alanWinRecordScreen;
    ALLEGRO_BITMAP *heroChoiceScreenAda;
    ALLEGRO_BITMAP *heroChoiceScreenAlan;
    ALLEGRO_BITMAP *startScreen;

    const u_short windowWidth = 919;
    const u_short windowHeight = 517; // Window properties

    if (!al_init()) {
        al_show_native_message_box(display, "Temos problemas", "O jogo não conseguiu executar",
                                   "Veja se as instruções foram seguidas corretamente e  tente recompilar com algumas mudanças",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    }

    // Display exploringTimer and fps creation
    const int FPS = 60;
    exploringTimer = al_create_timer(1.0 / FPS);
    fightTimer = al_create_timer(1.0 / 5);
    display = al_create_display(windowWidth, windowHeight);
    if (!display) {
        al_show_native_message_box(display, "Temos problemas", "A tela não conseguiu carregar",
                                   "Veja se as instruções foram seguidas corretamente e  tente recompilar com algumas mudanças",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    }

    // Getting game settings done
    enum Direction {
        DOWN, LEFT, RIGHT, UP
    };
    enum GameModes {
        EXPLORING, FIGHTING
    };
    enum Commands {
        ATTACK, ESPECIAL, FLEE
    };
    al_set_window_title(display, "Fantasia Final");
    eventQueue = al_create_event_queue();
    al_install_keyboard();
    al_install_audio();
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_timer_event_source(exploringTimer));
    al_register_event_source(eventQueue, al_get_timer_event_source(fightTimer));
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_primitives_addon();

    // Creating the instance of the audio manager
    AudioManger audioManager;

    // Creating the instance of the game manager
    GameManager gameManager;

    // Creating the instance of the ui manager
    UIManager uiManager{};

    // Creating the instance of the score manager
    ScoreManager scoreManager;

    // Loading images for the 3 maps and chests
    map1 = al_load_bitmap("../assets/sprites/maps/mapa1.png");
    chest1 = al_load_bitmap("../assets/sprites/chest1.png");
    map2 = al_load_bitmap("../assets/sprites/maps/mapa2.png");
    chest2 = al_load_bitmap("../assets/sprites/chest2.png");
    map3 = al_load_bitmap("../assets/sprites/maps/mapa3.png");
    chest3 = al_load_bitmap("../assets/sprites/chest3.png");

    // Loading battle images in an array
    battleBitmaps[0] = al_load_bitmap("../assets/sprites/maps/battlemap1.png");
    battleBitmaps[1] = al_load_bitmap("../assets/sprites/maps/battlemap2.png");
    battleBitmaps[2] = al_load_bitmap("../assets/sprites/maps/battlemap3.png");

    // UI screens
    adaDeathScreen = al_load_bitmap("../assets/sprites/ada_death.png");
    adaWinScreen = al_load_bitmap("../assets/sprites/ada_win.png");
    adaWinRecordScreen = al_load_bitmap("../assets/sprites/ada_win_record.png");
    alanDeathScreen = al_load_bitmap("../assets/sprites/alan_death.png");
    alanWinScreen = al_load_bitmap("../assets/sprites/alan_win.png");
    alanWinRecordScreen = al_load_bitmap("../assets/sprites/alan_win_record.png");
    heroChoiceScreenAda = al_load_bitmap("../assets/sprites/playerselectedada.png");
    heroChoiceScreenAlan = al_load_bitmap("../assets/sprites/playerselectedalan.png");

    // UI screens
    adaDeathScreen = al_load_bitmap("../assets/sprites/ada_death.png");
    adaWinScreen = al_load_bitmap("../assets/sprites/ada_win.png");
    adaWinRecordScreen = al_load_bitmap("../assets/sprites/ada_win_record.png");
    alanDeathScreen = al_load_bitmap("../assets/sprites/alan_death.png");
    alanWinScreen = al_load_bitmap("../assets/sprites/alan_win.png");
    alanWinRecordScreen = al_load_bitmap("../assets/sprites/alan_win_record.png");
    heroChoiceScreenAda = al_load_bitmap("../assets/sprites/playerselectedada.png");
    heroChoiceScreenAlan = al_load_bitmap("../assets/sprites/playerselectedalan.png");
    startScreen = al_load_bitmap("../assets/sprites/start_screen.png");

    // Start screen
    prologueMusic = audioManager.playLoop(prologueId);
    ALLEGRO_EVENT initGame;
    do{
        al_draw_bitmap(startScreen, 0, 0, 0);
        al_flip_display();
        al_wait_for_event(eventQueue, &initGame);
    }
    while(initGame.type != ALLEGRO_EVENT_KEY_DOWN);

    // Choosing hero
    int currentKey;
    do{
        if(GameManager::selectedHero == "alan"){
            al_draw_bitmap(heroChoiceScreenAlan, 0, 0, 0);
        }
        else if(GameManager::selectedHero == "ada"){
            al_draw_bitmap(heroChoiceScreenAda, 0, 0, 0);
        }
        al_flip_display();
        ALLEGRO_EVENT preGameEvent;
        al_wait_for_event(eventQueue, &preGameEvent);
        if(preGameEvent.type == ALLEGRO_EVENT_KEY_DOWN){
            currentKey = preGameEvent.keyboard.keycode;
            gameManager.heroChoice(currentKey);
        }
    }while(currentKey != ALLEGRO_KEY_ENTER);

    al_destroy_bitmap(startScreen);
    audioManager.stopPlaying(prologueMusic);


    // Setting player character
    Player player = Player(50, 0, 1.2, GameManager::selectedHero);
    player.setDimensions();
    playerSprite = al_load_bitmap(player.spritePath.c_str());
    playerBattleSprite = al_load_bitmap(player.battlePath.c_str());
    float playerSpriteWidth = player.individualSpriteX;
    float playerSpriteHeight = player.individualSpriteY;
    int spriteSheetAnimationRefreshFPS = 0;
    bool running = true;
    int direction; //range from zero to four for changing the image's direction
    float sX = playerSpriteWidth; //the width of each individual sprite in the sprite sheet
    bool isSpriteInNeedToUpdateByKeyInput = false;
    al_start_timer(exploringTimer);
    al_start_timer(fightTimer);
    ALLEGRO_COLOR attackColor = al_map_rgb(48, 219, 48);
    ALLEGRO_COLOR damageColor = al_map_rgba(217, 58, 43, 220);
    /*
     * Random number generator seeded with a disallowed source of seed value will generate
     * a predictable sequence of values
     * But I couldn't figure out a better solution
     */
    srand(time(nullptr));
    gameManager.sortPositions((int) (windowWidth - playerSpriteWidth) - 2,
                              (int) (windowHeight - playerSpriteWidth) - 2); //spreading monsters
    GameManager::gameMode = EXPLORING;
    vector<Monster> currentMonster; //Monsters loaded when entering in battle mode
    pair<int, int> currentCoordinate;

    bool isOnBattle; // Set to true when an enemy is found
    float sXM1 = 0; // Location to start drawing the first monster image of the spritesheet
    float sXM2 = 0; // Location to start drawing the first monster image of the spritesheet
    float sXM3 = 0; // Location to start drawing the first monster image of the spritesheet
    bool commandsIndicies[3] = {true, false, false}; // Array with the state of the commands attack, especial and flee
    int previousTimeCount; // Used for counting time to an enemy wait a little for attack, or else it would be pretty fast

    uiManager.prepareUI();

    // Main loop
    while (running) {
        int battleBitMapIndex; // Index for the battleBitMapArray which will be a random number
        if (GameManager::gameMode == EXPLORING) {
            //playing the themesong
            if (!audioManager.isPlaying) {
                generalMusic = audioManager.playLoop(generalID);
            }

            // Detects whether the player has found a monster and changes the game mode
            if (gameManager.foundMonster(player, &currentCoordinate) and not player.exempted) {
                GameManager::gameMode = FIGHTING; //boolean to enter the fight (battle) mode
                audioManager.stopPlaying(generalMusic); //stopping the music
            }
                // If player exit monster area he/she can find another
            else if (not gameManager.foundMonster(player, &currentCoordinate)) {
                player.exempted = false;
            }
            isOnBattle = true;
            switch (GameManager::level) {
                case 1:
                    al_draw_bitmap(map1, 0.0, 0.0, 0);
                    break;
                case 2:
                    al_draw_bitmap(map2, 0.0, 0.0, 0);
                    break;
                case 3:
                    al_draw_bitmap(map3, 0.0, 0.0, 0);
                    break;
            }

            // Play victory song
            if (player.foundChest()) {

                // Recover life
                player.life = player.fullLife;
                GameManager::enemiesLocalization.clear(); // Clearing monster with coordinates
                // Setting new monsters with new coordinates
                gameManager.sortPositions((int) (windowWidth - playerSpriteWidth) - 2,
                                          (int) (windowHeight - playerSpriteWidth) - 2);
                if (GameManager::level == 3) {
                    // Won
                    if (audioManager.isPlaying and audioManager.whatIsNowPlaying() == "geral") {
                        audioManager.stopPlaying(generalMusic);
                        audioManager.playLoop(victoryId);

                        switch (GameManager::level) {
                            case 1:
                                al_draw_tinted_bitmap(chest1, al_map_rgb(168, 118, 204), windowWidth - 63, 8, 0);
                                break;
                            case 2:
                                al_draw_tinted_bitmap(chest2, al_map_rgb(168, 118, 204), windowWidth - 63, 8, 0);
                                break;
                            case 3:
                                al_draw_tinted_bitmap(chest3, al_map_rgb(168, 118, 204), windowWidth - 63, 8, 0);
                                break;
                        }

                        al_flip_display();
                        al_rest(0.75);
                        if (player.name == "alan") {
                            // New record
                            if(player.points > scoreManager.getRecord()){
                                scoreManager.setRecord(player.points);
                                al_draw_bitmap(alanWinRecordScreen, 0, 0, 0);
                                uiManager.scoreUI(player.points);
                            }

                            // No new record
                            else{
                                al_draw_bitmap(alanWinScreen, 0, 0, 0);
                                uiManager.recordUI(scoreManager.getRecord());
                                uiManager.scoreUI(player.points);
                            }
                            al_flip_display();
                            al_rest(10);
                            running = false;
                        } else {
                            // New record
                            if(player.points > scoreManager.getRecord()){
                                scoreManager.setRecord(player.points);
                                al_draw_bitmap(adaWinRecordScreen, 0, 0, 0);
                                uiManager.scoreUI(player.points);
                            }

                            // No new record
                            else{
                                al_draw_bitmap(adaWinScreen, 0, 0, 0);
                                uiManager.scoreUI(player.points);
                                uiManager.recordUI(scoreManager.getRecord());
                            }
                            uiManager.scoreUI(player.points);
                            al_flip_display();
                            al_rest(10);
                            running = false;
                        }
                    }
                }
                if (GameManager::level == 2) GameManager::level = 3;
                if(GameManager::level == 1) GameManager::level = 2;
                player.x = 20;
                player.y = windowHeight - 25;
                switch (GameManager::level) {
                    case 1:
                        al_draw_tinted_bitmap(chest1, al_map_rgb(168, 118, 204), windowWidth - 63, 8, 0);
                        break;
                    case 2:
                        al_draw_tinted_bitmap(chest2, al_map_rgb(168, 118, 204), windowWidth - 63, 8, 0);
                        break;
                    case 3:
                        al_draw_tinted_bitmap(chest3, al_map_rgb(168, 118, 204), windowWidth - 63, 8, 0);
                        break;
                }

                al_flip_display();
                al_rest(0.35);

            } else
                switch (GameManager::level) {
                    case 1:
                        al_draw_bitmap(chest1, windowWidth - 63, 8, 0);
                        break;
                    case 2:
                        al_draw_bitmap(chest2, windowWidth - 63, 8, 0);
                        break;
                    case 3:
                        al_draw_bitmap(chest3, windowWidth - 63, 8, 0);
                        break;
                }

            ALLEGRO_EVENT event;
            al_wait_for_event(eventQueue, &event);

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                   if (al_show_native_message_box(display, "Confirmação de saída", "Tem certeza que quer sair?", "", nullptr, ALLEGRO_MESSAGEBOX_YES_NO) == 1)
                running = false;
            }
            if (event.type == ALLEGRO_EVENT_TIMER and event.timer.source == exploringTimer) {
                // Call to function for detecting player trying to cross the limits and make sure it doesn't do that
                player.borderCollision();

                //  [DEBUG DRAWING]
//                for (const auto &enemy: GameManager::enemiesLocalization) {
//                    for (auto monster: enemy.second) {
//                        al_draw_filled_circle((float) enemy.first.first, (float) enemy.first.second, 13, al_map_rgb(255, 255, 255));
//                    }
//                }

                //  [DEBUG DRAWING]
//                al_draw_filled_circle(player.x, player.y, 4, al_map_rgb(255, 255, 255));

                // Drawing player
                al_draw_bitmap_region(playerSprite, sX, (float) direction * playerSpriteHeight, playerSpriteWidth,
                                      playerSpriteHeight, player.x, player.y, 0);
                // Drawing score
                uiManager.scoreIndicator(player.points);
                uiManager.playerLifeBar(player.fullLife, player.life, make_pair(35, 47));
                uiManager.levelIndicator(GameManager::level);

                al_flip_display();

                // Setting keyboard input
                ALLEGRO_KEYBOARD_STATE keyState;
                al_get_keyboard_state(&keyState);
                isSpriteInNeedToUpdateByKeyInput = true;

                // Player movement by user input
                if (al_key_down(&keyState, ALLEGRO_KEY_A)) {
                    // Run
                    if (al_key_down(&keyState, ALLEGRO_KEY_LSHIFT)) player.moveSpeed = 2.1;
                    // Walk
                    else player.moveSpeed = 1.2;
                    player.x -= player.moveSpeed;
                    direction = LEFT;
                } else if (al_key_down(&keyState, ALLEGRO_KEY_D)) {
                    // Run
                    if (al_key_down(&keyState, ALLEGRO_KEY_LSHIFT)) player.moveSpeed = 2.2;
                    // Walk
                    else player.moveSpeed = 1.2;
                    player.x += player.moveSpeed;
                    direction = RIGHT;
                } else if (al_key_down(&keyState, ALLEGRO_KEY_W)) {
                    // Run
                    if (al_key_down(&keyState, ALLEGRO_KEY_LSHIFT)) player.moveSpeed = 2.2;
                    // Walk
                    else player.moveSpeed = 1.2;
                    direction = UP;
                    player.y -= player.moveSpeed;
                } else if (al_key_down(&keyState, ALLEGRO_KEY_S)) {
                    // Run
                    if (al_key_down(&keyState, ALLEGRO_KEY_LSHIFT)) player.moveSpeed = 2.2;
                    // Walk
                    else player.moveSpeed = 1.2;
                    player.y += player.moveSpeed;
                    direction = DOWN;
                } else {
                    isSpriteInNeedToUpdateByKeyInput = false;
                }

                // Handles the characters animation
                if (spriteSheetAnimationRefreshFPS == 10) {
                    if (isSpriteInNeedToUpdateByKeyInput) sX += playerSpriteWidth;
                    else sX = playerSpriteWidth;
                    if (sX >= (float) player.totalSpriteX) sX = 0;
                }
                spriteSheetAnimationRefreshFPS++;
                if (spriteSheetAnimationRefreshFPS == 11) spriteSheetAnimationRefreshFPS = 0;
            }
        } else if (GameManager::gameMode == FIGHTING) { //Starts the FIGHT
            /* battleBitMapIndex will always have a value because
             * isOnBattle is previously set to true
             * */

            // Playing the theme song of the battle
            if (!audioManager.isPlaying) {
                battleMusic = audioManager.playLoop(battleId);
            }

            // Executed once when a monster is found
            if (isOnBattle) {
                player.justAttacked = false;
                battleBitMapIndex = GameManager::level - 1;
                GameManager::enemiesLocalization[currentCoordinate][0].isSelected = true;
                GameManager::enemiesLocalization[currentCoordinate][0].isNextToAttack = true;
                //Load sprites for each monster found
                for (int i = 0; i < GameManager::enemiesLocalization[currentCoordinate].size(); i++) {
                    GameManager::enemiesLocalization[currentCoordinate][i].prepareDrawing();
                }
            }
            al_draw_bitmap(battleBitmaps[battleBitMapIndex], 0.0, 0.0, 0); // Drawing the map for current phase
            ALLEGRO_EVENT event;
            al_wait_for_event(eventQueue, &event);

            // Important for closing program with the window
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                   if (al_show_native_message_box(display, "Confirmação de saída", "Tem certeza que quer sair?", "", nullptr, ALLEGRO_MESSAGEBOX_YES_NO) == 1)
                        running = false;
                   uiManager.clean();
            }
            if (event.type == ALLEGRO_EVENT_TIMER and event.timer.source == fightTimer) {
                // Drawing each one of the monster in this for loop
                for (int i = 0; i < GameManager::enemiesLocalization[currentCoordinate].size(); i++) {
                    auto width = (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i].bitmap);
                    auto height = (float) al_get_bitmap_height(GameManager::enemiesLocalization[currentCoordinate][i].bitmap);
                    if (i == 0) {
                        if (sXM1 >= (width - width / 3) - 1) sXM1 = 0;
                        else sXM1 += width / 3;

                        if (al_get_timer_count(fightTimer) - previousTimeCount <= 3) {
                            if (gameManager.getSelected(GameManager::enemiesLocalization[currentCoordinate]) == i and not player.justFailedFleeing) {
                                al_draw_tinted_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, damageColor, sXM1, 0, width / 3, height, 10, 200, 0);
                            } else {
                                al_draw_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, sXM1, 0, width / 3, height, 10, 200, 0);
                            }
                        } else if (al_get_timer_count(fightTimer) - previousTimeCount > 5 and
                                   al_get_timer_count(fightTimer) - previousTimeCount < 8) {
                            if (gameManager.getNextToAttack(GameManager::enemiesLocalization[currentCoordinate]) == i) {
                                al_draw_tinted_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, attackColor, sXM1, 0, width / 3, height, 720, 200, 0);
                            } else {
                                al_draw_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, sXM1, 0, width / 3, height, 10, 200, 0);
                            }
                        } else {
                            al_draw_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, sXM1, 0, width / 3, height, 10, 200, 0);
                        }

                    } else if (i == 1) {
                        if (sXM2 >= (width - width / 3) - 1) sXM2 = 0;
                        else sXM2 += width / 3;

                        if (al_get_timer_count(fightTimer) - previousTimeCount <= 3) {
                            if (gameManager.getSelected(GameManager::enemiesLocalization[currentCoordinate]) == i and not player.justFailedFleeing) {
                                al_draw_tinted_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, damageColor, sXM2, 0, width / 3, height, 25 + (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i - 1].bitmap) / 3, 200, 0);
                            } else {
                                al_draw_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, sXM2, 0, width / 3, height, 25 + (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i - 1].bitmap) / 3, 200, 0);
                            }
                        } else if (al_get_timer_count(fightTimer) - previousTimeCount > 5 and
                                   al_get_timer_count(fightTimer) - previousTimeCount < 8) {
                            if (gameManager.getNextToAttack(GameManager::enemiesLocalization[currentCoordinate]) == i) {
                                al_draw_tinted_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, attackColor, sXM2, 0, width / 3, height, 720, 200, 0);
                            } else {
                                al_draw_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, sXM2, 0, width / 3, height, 25 + (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i - 1].bitmap) / 3, 200, 0);
                            }
                        } else {
                            al_draw_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, sXM2, 0, width / 3, height, 25 + (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i - 1].bitmap) / 3, 200, 0);
                        }

                    } else {
                        if (sXM3 >= (width - width / 3) - 1) sXM3 = 0;
                        else sXM3 += width / 3;

                        if (al_get_timer_count(fightTimer) - previousTimeCount <= 3) {
                            if (gameManager.getSelected(GameManager::enemiesLocalization[currentCoordinate]) == i and not player.justFailedFleeing) {
                                al_draw_tinted_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, damageColor, sXM3, 0, width / 3, height,
                                                             40 + (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i - 1].bitmap) / 3 + (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i - 2].bitmap) / 3, 200, 0);
                            } else {
                                al_draw_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, sXM3, 0, width / 3, height,
                                                      40 + (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i - 1].bitmap) / 3 + (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i - 2].bitmap) / 3, 200, 0);
                            }
                        } else if (al_get_timer_count(fightTimer) - previousTimeCount > 5 and
                                   al_get_timer_count(fightTimer) - previousTimeCount < 8) {
                            if (gameManager.getNextToAttack(GameManager::enemiesLocalization[currentCoordinate]) == i) {
                                al_draw_tinted_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, attackColor, sXM3, 0, width / 3, height,
                                                             + 720, 200, 0);
                            } else {
                                al_draw_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, sXM3, 0, width / 3, height,
                                                      40 + (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i - 1].bitmap) / 3 + (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i - 2].bitmap) / 3, 200, 0);
                            }
                        } else {
                            al_draw_bitmap_region(GameManager::enemiesLocalization[currentCoordinate][i].bitmap, sXM3, 0, width / 3, height,
                                                  40 + (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i - 1].bitmap) / 3 + (float) al_get_bitmap_width(GameManager::enemiesLocalization[currentCoordinate][i - 2].bitmap) / 3, 200, 0);
                        }
                    }
                } // Done drawing monster(s) for this frame

                // Drawing player, if attacked, attacking or none
                if (player.justAttacked) {
                    if (al_get_timer_count(fightTimer) - previousTimeCount <= 3 and not player.justFailedFleeing) {
                        if(gameManager.getSelected(GameManager::enemiesLocalization[currentCoordinate]) == 0)
                            al_draw_tinted_bitmap(playerBattleSprite, attackColor, 65, 220, 0); //Drawing player's sprite

                        else if(gameManager.getSelected(GameManager::enemiesLocalization[currentCoordinate]) == 1)
                            al_draw_tinted_bitmap(playerBattleSprite, attackColor, 300, 220, 0); //Drawing player's sprite

                        else if(gameManager.getSelected(GameManager::enemiesLocalization[currentCoordinate]) == 2)
                            al_draw_tinted_bitmap(playerBattleSprite, attackColor, 500, 220, 0); //Drawing player's sprite
                    } else if (al_get_timer_count(fightTimer) - previousTimeCount > 5 and
                               al_get_timer_count(fightTimer) - previousTimeCount < 8) {
                        al_draw_tinted_bitmap(playerBattleSprite, damageColor, 800, 220, 0); //Drawing player's sprite
                        int hitValue = GameManager::enemiesLocalization[currentCoordinate][gameManager.getNextToAttack(GameManager::enemiesLocalization[currentCoordinate])].attack;
                        if (hitValue >= 1 and hitValue <= 4)
                            uiManager.damageUiIndicator(hitValue);
                        else{
                            uiManager.damageUiIndicator(0);
                        }
                    } else {
                        al_draw_bitmap(playerBattleSprite, 800, 220, 0);
                    }
                } else {
                    al_draw_bitmap(playerBattleSprite, 800, 220, 0);
                }
                uiManager.playerInfoBackground(player.name, player.fullLife, player.life,
                                               commandsIndicies, player.numberOfEspecialAttack); //player background info
                uiManager.enemiesInfoBackground(GameManager::enemiesLocalization[currentCoordinate]); //monsters background info
                al_flip_display();
            }
            // Input by up, down, left and right keys
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                // Options input [ATTACK, ESPECIAL and FLEE]
                if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                    if (commandsIndicies[ATTACK]) {
                        commandsIndicies[ATTACK] = false;
                        commandsIndicies[ESPECIAL] = true;
                        commandsIndicies[FLEE] = false;
                    } else if (commandsIndicies[ESPECIAL]) {
                        commandsIndicies[ATTACK] = false;
                        commandsIndicies[ESPECIAL] = false;
                        commandsIndicies[FLEE] = true;
                    }
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_UP) {
                    if (commandsIndicies[FLEE]) {
                        commandsIndicies[ATTACK] = false;
                        commandsIndicies[FLEE] = false;
                        commandsIndicies[ESPECIAL] = true;
                    } else if (commandsIndicies[ESPECIAL]) {
                        commandsIndicies[FLEE] = false;
                        commandsIndicies[ESPECIAL] = false;
                        commandsIndicies[ATTACK] = true;
                    }
                }
                // Monster switch with arrow keys
                if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                    try {
                        gameManager.changeSelectedLeft(&GameManager::enemiesLocalization[currentCoordinate]);
                    }
                    catch (logic_error &error) {

                    }
                }
                if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) {
                    gameManager.changeSelectedRight(&GameManager::enemiesLocalization[currentCoordinate]);
                }
                // Check if player entered an option
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    for (int j = 0; j < 3; j++) { // Cycles through the options
                        if (commandsIndicies[j]) { // What option is selected
                            switch (j) {
                                case ATTACK: // Check if the option is attack
                                    // Check if it is player's turn
                                    if (not player.justAttacked) {
                                        audioManager.playOnce("m_hurt", "");
                                        // Player attack monster
                                        GameManager::enemiesLocalization[currentCoordinate][gameManager.getSelected(GameManager::enemiesLocalization[currentCoordinate])].hit(player.attack);
                                        player.justAttacked = true; // Player just attacked, so it is not it's turn
                                        player.justFailedFleeing = false;
                                        al_set_timer_count(fightTimer, 0);
                                        previousTimeCount = (int) al_get_timer_count(fightTimer);
                                    }
                                    break;
                                case ESPECIAL: // Check if the option is especial
                                    if (not player.justAttacked) {
                                        if (player.numberOfEspecialAttack > 0) { // If player has an especial
                                            audioManager.playOnce("m_hurt", "");
                                            // Hit with special attack
                                            GameManager::enemiesLocalization[currentCoordinate][gameManager.getSelected(GameManager::enemiesLocalization[currentCoordinate])].hit(player.attack * 3);
                                            player.justAttacked = true; // Player just attacked, so it is not it's turn
                                            al_set_timer_count(fightTimer, 0);
                                            previousTimeCount = (int) al_get_timer_count(fightTimer);
                                            player.numberOfEspecialAttack--; // Reduce number of especial
                                            player.justFailedFleeing = false; //Player didn't fail
                                        }
                                    }
                                    break;
                                case FLEE: //Check if the option is Flee
                                    int probabilidade = (int) random() % 11;
                                    // It seems it tends to generate bigger numbers, so I divided half to half
                                    if (probabilidade > 5) { // Success
                                        GameManager::gameMode = EXPLORING; //Back to exploring
                                        player.exempted = true; //Now player can't find a monster
                                        audioManager.stopPlaying(battleMusic); // Stop battle song
                                        player.justFailedFleeing = false; // Did not fail
                                        gameManager.deselectAll(&GameManager::enemiesLocalization[currentCoordinate]);
                                        commandsIndicies[FLEE] = false;
                                        commandsIndicies[ATTACK] = true;
                                        // GameManager::enemiesLocalization.erase(currentCoordinate);
                                    } else { // Failed
                                        player.justAttacked = true; // Just for player now be attacked next
                                        player.justFailedFleeing = true;
                                        al_set_timer_count(fightTimer, 0);
                                        previousTimeCount = (int) al_get_timer_count(fightTimer);
                                    }
                            }
                        }
                    }
                }
            }

            if (player.justAttacked) { // Condition for to the current monster to attack, well... Attack!

                // Play damage sound
                if (al_get_timer_count(fightTimer) - previousTimeCount >= 5 and al_get_timer_count(fightTimer) - previousTimeCount < 6)
                    audioManager.playOnce("hurt", player.name);


                if (al_get_timer_count(fightTimer) - previousTimeCount >= 10) {
                    player.justAttacked = false;

                    int hitValue = GameManager::enemiesLocalization[currentCoordinate][gameManager.getNextToAttack(GameManager::enemiesLocalization[currentCoordinate])].attack;
                    if (hitValue >= 2 and hitValue <= 4)
                    player.hit(hitValue);

                    gameManager.changeNextoToAttak(&GameManager::enemiesLocalization[currentCoordinate]);
                }
            }

            int battleStateUpdate = gameManager.battleStateUpdate(&GameManager::enemiesLocalization[currentCoordinate], &player, &currentCoordinate);

            // Player killed all monsters
            if (battleStateUpdate == 1) {
                audioManager.stopPlaying(battleMusic); // Stop battle song
            }

                // Player dead
            else if (battleStateUpdate == 2) {

                if (audioManager.isPlaying and audioManager.whatIsNowPlaying() == "battle") {
                    audioManager.stopPlaying(battleMusic);
                    audioManager.playLoop(loseId);
                }

                if (player.name == "alan") {
                    al_draw_bitmap(alanDeathScreen, 0, 0, 0);
                    uiManager.recordUI(scoreManager.getRecord());
                    uiManager.scoreUI(player.points);
                    al_flip_display();
                    al_rest(10);
                    running = false;
                } else {
                    al_draw_bitmap(adaDeathScreen, 0, 0, 0);
                    uiManager.recordUI(scoreManager.getRecord());
                    uiManager.scoreUI(player.points);
                    al_flip_display();
                    al_rest(10);
                    running = false;
                }

            } else {

            }

            isOnBattle = false;

        }
    }

    // só pra não acabar com a minha ram
    if (audioManager.isPlaying and audioManager.whatIsNowPlaying() == "battle") {

        audioManager.stopPlaying(battleMusic);
    }

    // Cleaning garbage
    al_destroy_display(display);
    al_uninstall_keyboard();
    al_uninstall_audio();
    al_destroy_bitmap(playerSprite);
    al_destroy_bitmap(playerBattleSprite);
    al_destroy_bitmap(alanWinScreen);
    al_destroy_bitmap(alanWinRecordScreen);
    al_destroy_bitmap(alanDeathScreen);
    al_destroy_bitmap(adaWinScreen);
    al_destroy_bitmap(adaWinRecordScreen);
    al_destroy_bitmap(adaDeathScreen);
    al_destroy_bitmap(map1);
    al_destroy_bitmap(map2);
    al_destroy_bitmap(map3);
    al_destroy_bitmap(chest1);
    al_destroy_bitmap(chest2);
    al_destroy_bitmap(chest3);
    al_destroy_bitmap(heroChoiceScreenAlan);
    al_destroy_bitmap(heroChoiceScreenAda);
    al_destroy_bitmap(battleBitmaps[0]);
    al_destroy_bitmap(battleBitmaps[1]);
    al_destroy_bitmap(battleBitmaps[2]);
    al_destroy_event_queue(eventQueue);
    al_destroy_timer(exploringTimer);
    al_destroy_timer(fightTimer);
    for (int i = 0; i < GameManager::enemiesLocalization.size(); i++) {
        for (int j = 0; j < GameManager::enemiesLocalization[currentCoordinate].size(); j++) {
            GameManager::enemiesLocalization[currentCoordinate][j].clean(); // Cleaning bitmaps
        }
    }
    return 0;
}