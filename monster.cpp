#include <allegro5/allegro5.h>
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

class Monster{
public:
    int attack;
    int life;
    bool alive = true;
    int resistance;
    string spritePath;

    void hit(int damage){
        life -= damage / resistance;
        if (life < 0) alive = false;
    }
    explicit Monster(string _spritePath){
        spritePath = _spritePath;
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
