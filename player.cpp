#include <cmath>
#include <iostream>
#pragma once

using std::string;

class Player{
public:
    string spritePath = "../assets/sprites/characters/";
    string battlePath = "../assets/sprites/characters/";
    int life;
    int fullLife;
    int points;
    float attack;
    float moveSpeed;
    string name;
    int totalSpriteX;
    int totalSpriteY;
    float individualSpriteX;
    float individualSpriteY;
    bool alive = true;
    bool exempted = false;
    bool justFailedFleeing = false;
    bool justAttacked = false;
    int numberOfEspecialAttack = 15;
    float x = 40, y = 40; //coordinates for drawing each image of the player

    Player(int pLife, int pPoints, float pMoveSpeed, string pName){
        life = pLife;
        fullLife = life;
        points = pPoints;
        moveSpeed = pMoveSpeed;
        name = move(pName);
        attack = 5.7;
    }

    // Calculates the distance between the player and some object by its coordinates
    float distanceOf(int sourceX, int sourceY) const{
        return (float) sqrt(pow((float) sourceX - x, 2) + pow((float)sourceY - y, 2));
    }

    // Function for adjusting sprites dimensions according to the characters
    void setDimensions() {
        spritePath = spritePath + name + ".png";
        battlePath = battlePath + name + "b.png";
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

    // Function for detecting player trying to cross the limits and make sure it doesn't do that
    void borderCollision(){
        if (x <= 0.0) {
            x = 0.0;
        }
        if(x + individualSpriteX >= 919.0){
            x = 919.0f - individualSpriteX;
        }
        if (y <= 0.0) {
            y = 0.0;
        }
        if(y + individualSpriteY >= 517.0){
            y = 517.0f - individualSpriteY;
        }
    }

    bool foundChest() const{
        return x > 863 and y < 8;
     }

     void hit(int damage){
         life -= damage;
         if (life <= 0) alive = false;
     }
};
