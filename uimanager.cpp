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
    void playerInfoBackground(const string &playerName, int fullLife, int life) const {
        float x2; //Second x coordinate for filled rectangle, for displaying current life
        //Background
        al_draw_filled_rounded_rectangle(695, 405, 905, 505, 6, 6, al_map_rgb(5, 55, 135));
        //Name
        al_draw_text(font, silver, 696, 405, 0, playerName.c_str());
        //Unfilled rect
        al_draw_rounded_rectangle(737, 409, 900, 423, 5, 5, silver, 2);
        x2 = (float) (life * (900 - 737)) / (float) fullLife + 737;
        //Filled rect
        al_draw_filled_rounded_rectangle(737, 409, x2, 423, 4.5, 4.5, playerLife);
        //Divisor
        al_draw_line(697, 430, 903, 430, al_map_rgb(255, 255, 255), 1);
        //Action texts
        al_draw_text(font, silver, 895, 432, ALLEGRO_ALIGN_RIGHT, "Atacar");
        al_draw_text(font, silver, 895, 456, ALLEGRO_ALIGN_RIGHT, "Especial");
        al_draw_text(font, silver, 895, 480, ALLEGRO_ALIGN_RIGHT, "Fugir");
    }

    void enemiesInfoBackground(const vector<Monster>& monsters) const {
        //Background
        al_draw_filled_rounded_rectangle(10, 405, 670, 505, 6, 6, al_map_rgb(5, 55, 135));

        for (int i = 0; i < monsters.size(); i++) {
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
                al_draw_text(font, silver, 500, 410, 0, monsters[i].name.c_str());
                //Unfilled rect
                al_draw_rounded_rectangle(470, 450, 660, 464, 5, 5, silver, 2);
                x2 = (float) (monsters[i].life * (660 - 470)) / (float) monsters[i].fullLife + 470;
                //Filled rect
                al_draw_filled_rounded_rectangle(470, 450, x2, 464, 4.5, 4.5, playerLife);
            }
        }

    }

    void clean() {
        al_destroy_font(font);
    }
};