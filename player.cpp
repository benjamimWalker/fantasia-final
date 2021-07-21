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
    int width;
    int height;

    Player(int pLife, int pPoints, float pMoveSpeed, string pName){
        life = pLife;
        points = pPoints;
        moveSpeed = pMoveSpeed;
        name = std::move(pName);
    }

    //function for adjusting sprites dimensions according to the characters
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
    //function for detecting player trying to cross the limits and make sure it doesn't do that
    void collision(float *x, float *y) const{
        if (*x <= 0.0) {
            *x = 0.0;
        }
        if(*x + individualSpriteX >= 919.0){
            *x = 919.0f - individualSpriteX;
        }
        if (*y <= 0.0) {
            *y = 0.0;
        }
        if(*y + individualSpriteY >= 517.0){
            *y = 517.0f - individualSpriteY;
        }
    }
};
