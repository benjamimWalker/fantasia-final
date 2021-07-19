#include <cmath>
#include <iostream>
#include <cstring>
#include <utility>

//TODO REMOVE THIS CLION UNNECESSARY STUFF
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
using namespace std;

class Player{

public:
    string spritePath = "../assets/sprites/characters/";
    int life;
    int points = 0;
    float moveSpeed;
    string name;
    int totalSpriteX;
    int totalSpriteY;
    float individualSpriteX;
    float individualSpriteY;
    bool alive = true;

    Player(int pLife, int pPoints, float pMoveSpeed, string pName){
        life = pLife;
        points = pPoints;
        moveSpeed = pMoveSpeed;
        name = std::move(pName);
    }

    void setDimensions() {
        spritePath = spritePath + name + ".png";
        if (name == "alan"){
            totalSpriteX = 192;
            totalSpriteY = 166;

            individualSpriteX = (float) totalSpriteX / 6;
            individualSpriteY = (float) totalSpriteY / 4;
        }
        else if (name == "ada"){
            totalSpriteX = 94;
            totalSpriteY = 161;

            individualSpriteX = (float) totalSpriteX / 3;
            individualSpriteY = (float) totalSpriteY / 4;
        }
    }
};
