#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include <vector>
#include "monster.cpp"

using namespace std;

class UIManager {

public:
    // Fonts and colors
    ALLEGRO_FONT *font;
    ALLEGRO_COLOR green;
    ALLEGRO_COLOR red;
    ALLEGRO_COLOR silver;
    ALLEGRO_COLOR orange;
    ALLEGRO_COLOR yellow;
    ALLEGRO_COLOR playerLife;

    void prepareUI() {
        al_init_font_addon();
        al_init_ttf_addon();
        font = al_load_font("../assets/fonts/quicksand.ttf", 18, 0);
        green =  al_map_rgb(146, 247, 99);
        red   =  al_map_rgb(237, 89, 52);
        silver = al_map_rgb(166, 185, 186);
        orange = al_map_rgb(232, 146, 65);
        yellow = al_map_rgb(247, 229, 126);
        playerLife = green;
    }


    void drawLifebar() {
        al_draw_rounded_rectangle(200, 234, 350, 245, 4, 4, al_map_rgb(255, 255, 255), 2);
        al_draw_filled_rounded_rectangle(200, 234, 280, 245, 4, 4, al_map_rgb(255, 255, 255));
    }

    //Draw a rectangle in which the info will be displayed
    void playerInfoBackground(const string &playerName, int fullLife, int life, const bool commands[3]) const {
        int attackIndicatorLocation = 0;
        float x2; //Second x coordinate for filled rectangle, for displaying current life
        //Background
        al_draw_filled_rounded_rectangle(695, 405, 905, 505, 6, 6, al_map_rgb(5, 55, 135));
        //Name
        al_draw_text(font, silver, 696, 405, 0, playerName.c_str());
        playerLifeBar(fullLife, life, make_pair(737, 409));
        //Divisor
        al_draw_line(697, 430, 903, 430, al_map_rgb(255, 255, 255), 1);
        //Action texts
        al_draw_text(font, silver, 895, 432, ALLEGRO_ALIGN_RIGHT, "Atacar");
        al_draw_text(font, silver, 895, 456, ALLEGRO_ALIGN_RIGHT, "Especial");
        al_draw_text(font, silver, 895, 480, ALLEGRO_ALIGN_RIGHT, "Fugir");

        //Change where to draw the indicator based on which action is true
        if(commands[0]){
            attackIndicatorLocation = 444;
        }
        else if(commands[1]){
            attackIndicatorLocation = 468;
        }
        else if (commands[2]){
            attackIndicatorLocation = 492;
        }

        //Selected action indicator
        al_draw_filled_circle(720, (float) attackIndicatorLocation, 8, red);
    }

    void playerLifeBar(int fullLife, int life, pair<int, int> where) const{
        float x2; //Second x coordinate for filled rectangle, for displaying current life
        //Unfilled rect
        al_draw_rounded_rectangle((float) where.first, (float) where.second, (float) where.first + 163, (float) where.second + 14, 5, 5, silver, 2);
        x2 = (float) (life * 163) / (float) fullLife + (float) where.first;
        //Filled rect
        al_draw_filled_rounded_rectangle((float) where.first, (float) where.second, x2, (float) where.second + 14, 4.5, 4.5, playerLife);
        // [DEBUG] SHOW LIFE IN NUMBERS
        // lifeNumericIndicator(life);
    }

    //Display information about monsters in the battle
    void enemiesInfoBackground(const vector<Monster>& monsters) const {
        int monsterIndicatorLocation = 0;
        //Background
        al_draw_filled_rounded_rectangle(10, 405, 670, 505, 6, 6, al_map_rgb(5, 55, 135));

        for (int i = 0; i < monsters.size(); i++) {
            // Choice of X coordinate based on which monster was selected by input
            if(monsters[i].isSelected){
                if(i == 0){
                    monsterIndicatorLocation = 107;
                }
                else if(i == 1){
                    monsterIndicatorLocation = 330;
                }
                else if(i == 2){
                    monsterIndicatorLocation = 565;
                }
            }
            if(i == 0){
                float x2; //Second x coordinate for filled rectangle, for displaying current life
                //Name
                al_draw_text(font, silver, 15, 410, 0, monsters[i].name.c_str());
                //Unfilled rect
                al_draw_rounded_rectangle(15, 450, 205, 464, 5, 5, silver, 2);
                x2 = (float) (monsters[i].life * (205 - 15)) / (float) monsters[i].fullLife + 15;
                //Filled rect
                al_draw_filled_rounded_rectangle(15, 450, x2, 464, 4.5, 4.5, playerLife); //TODO REPLACE ALL COLORS
            }
            if(i == 1){
                float x2; //Second x coordinate for filled rectangle, for displaying current life
                //Name
                al_draw_text(font, silver, 260, 410, 0, monsters[i].name.c_str());
                //Unfilled rect
                al_draw_rounded_rectangle(245, 450, 430, 464, 5, 5, silver, 2);
                x2 = (float) (monsters[i].life * (430 - 245)) / (float) monsters[i].fullLife + 245;
                //Filled rect
                al_draw_filled_rounded_rectangle(245, 450, x2, 464, 4.5, 4.5, playerLife);
            }
            if(i == 2){
                float x2; //Second x coordinate for filled rectangle, for displaying current life
                //Name
               // al_draw_text(font, silver, 480, 410, 0, monsters[i].name.c_str());
                //Unfilled rect
                al_draw_rounded_rectangle(470, 450, 660, 464, 5, 5, silver, 2);
                x2 = (float) (monsters[i].life * (660 - 470)) / (float) monsters[i].fullLife + 470;
                //Filled rect
                al_draw_filled_rounded_rectangle(470, 450, x2, 464, 4.5, 4.5, playerLife);
            }
            if (monsterIndicatorLocation != 0)
            al_draw_filled_circle((float) monsterIndicatorLocation, 485, 12, red);
        }
    }

    //Show in text the damage made by a monster in player
    void damageUiIndicator(int damage) const{
        al_draw_text(font, al_map_rgb(255, 255, 255), 780, 350, 0, ("-" + to_string(damage) ).c_str());
    }

    // Show the player's life in numbers
    void lifeNumericIndicator(int life) const{
        al_draw_text(font, al_map_rgb(255, 255, 255), 780, 380, 0, to_string(life).c_str());
    }

    // The name is enough for understanding
    void clean() {
        al_destroy_font(font);
    }
};