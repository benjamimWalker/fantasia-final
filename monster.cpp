#include <allegro5/allegro5.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <ctime>
using namespace std;

#pragma once;
class Monster{
public:
    int attack;
    float life;
    float fullLife;
    bool alive = true;
    float resistance;
    string name;
    string spritePath = "../assets/sprites/enemies/";
    ALLEGRO_BITMAP *bitmap;
    bool isSelected;
    bool isNextToAttack = false;
public:
    void hit(float damage){
        life -= damage / resistance;
        if (life <= 0) alive = false;
    }

     Monster(){
        int number = (int) (1 + random() % 16);
        spritePath = spritePath + to_string(number) + ".png";
        isSelected = false;
        switch (number) {
            case 1:
                name = "Bichão Fortão";
                attack = 4;
                life = 18;
                fullLife = life;
                resistance = 1.1;
                alive = true;
                break;
            case 2:
                name = "Chiclete Roxo";
                attack = 2;
                life = 11;
                fullLife = life;
                resistance = 1.0;
                alive = true;
                break;
            case 3:
                name = "Chupa Cabra";
                attack = 3;
                life = 19;
                fullLife = life;
                resistance = 1.7;
                alive = true;
                break;
            case 4:
                name = "Lady Foguinhescu";
                attack = 2;
                life = 14;
                fullLife = life;
                resistance = 1.1;
                alive = true;
                break;
            case 5:
                name = "Demônho Brabu";
                attack = 4;
                life = 20;
                fullLife = life;
                resistance = 2.0;
                alive = true;
                break;
            case 6:
                name = "El Corvo";
                attack = 3;
                life = 16;
                fullLife = life;
                resistance = 1.3;
                alive = true;
                break;
            case 7:
                name = "Cupido Ressentido";
                attack = 1;
                life = 7;
                fullLife = life;
                resistance = 1.0;
                alive = true;
                break;
            case 8:
                name = "Robô";
                attack = 3;
                life = 19;
                fullLife = life;
                resistance = 2.0;
                alive = true;
                break;
            case 9:
                name = "Cleópatra com Asa";
                attack = 2;
                life = 15;
                fullLife = life;
                resistance = 1.2;
                alive = true;
                break;
            case 10:
                name = "Criatura Insana";
                attack = 4;
                life = 20;
                fullLife = life;
                resistance = 2.0;
                alive = true;
                break;
            case 11:
                name = "Algodão Doce";
                attack = 3;
                life = 13;
                fullLife = life;
                resistance = 1.1;
                alive = true;
                break;
            case 12:
                name = "Hell Boy da Deep Web";
                attack = 3;
                life = 18;
                fullLife = life;
                resistance = 1.9;
                alive = true;
                break;
            case 13:
                name = "Mosquito de Fogo";
                attack = 4;
                life = 18;
                fullLife = life;
                resistance = 1.8;
                alive = true;
                break;
            case 14:
                name = "Shenlong Amarelo";
                attack = 4;
                life = 19;
                fullLife = life;
                resistance = 1.5;
                alive = true;
                break;
            case 15:
                name = "Chifrudo";
                attack = 3;
                life = 17;
                fullLife = life;
                resistance = 1.3;
                alive = true;
                break;
            case 16:
                name = "Aracnídeo";
                attack = 2;
                life = 11;
                fullLife = life;
                resistance = 1.0;
                alive = true;
                break;
        }
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
