#include <allegro5/allegro5.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <ctime>
using namespace std;

class Monster{
public:
    int attack;
    float life;
    bool alive = true;
    float resistance;
    string spritePath = "../assets/sprites/enemies/";
    ALLEGRO_BITMAP *bitmap;

    void hit(float damage){
        life -= damage / resistance;
        if (life <= 0) alive = false;
    }

    explicit Monster(){
        int number = (int) (1 + random() % 9);
        spritePath = spritePath + to_string(number) + ".png";

        /*
         * lógica para os demais atributos
         * de acordo com número aqui
         */

        attack = 10;
        life = 20;
        resistance = 1.6;
        alive = true;
    }

public:
    void prepareDrawing(){
        bitmap = al_load_bitmap(spritePath.c_str());
    }

    //destroying the image loaded during the game execution
    void clean(){
        al_destroy_bitmap(bitmap);
    }

    bool operator <(const Monster& rhs) const
    {
        return life <= rhs.life;
        //TODO CRIAR UM ID ESPECIAL PARA EVITAR CONFLITO
    }
};
