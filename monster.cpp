#include <allegro5/allegro5.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <ctime>
using namespace std;

class Monster{
public:
    int attack;
    int life;
    bool alive = true;
    int resistance;
    string spritePath = "../assets/sprites/enemies/";

    void hit(int damage){
        life -= damage / resistance;
        if (life < 0) alive = false;
    }
    explicit Monster(){
        int number = (int) (1 + random() % 24);
        spritePath = spritePath + to_string(number) + ".png";

        /*
         * lógica para os demais atributos
         * de acordo com número aqui
         */

        attack = 10;
        life = 20;
        alive = true;
        resistance = 15;
    }

    bool operator <(const Monster& rhs) const
    {
        return life <= rhs.life;
        //TODO CRIAR UM ID ESPECIAL PARA EVITAR CONFLITO
    }
};
