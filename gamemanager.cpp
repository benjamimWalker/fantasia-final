#include <allegro5/allegro5.h>
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include "player.cpp"
#include "monster.cpp"

using namespace std;

class GameManager{
public:
    static u_short gameMode;
    static bool isGameFinished;
    static u_short numEnemies; // 18
    static u_short level;

   /* One of the most important piece of data
    * it has a link between a vector with 1 to 3 monsters
    * and a random coordinate
    */
   static map<pair<int, int>, vector<Monster>> enemiesLocalization;

   // Put Monsters on their place
    vector<Monster> populateEnemies(){
        vector<Monster> vec;
        for (int i = 0; i < 3; i++){ //(random() % 3) + 1
            vec.emplace_back(Monster());
        }
        return vec;
    }

    // Random coordinates for monsters
    void sortPositions(int width, int height){
        for (int j = 0; j < numEnemies; j++) {
            vector<Monster> v = populateEnemies();
           // srand (time (0));
           enemiesLocalization.insert(pair<pair<int, int>, vector<Monster>> (pair<int, int>(random() % width, random() % height), v));
        }
    }

    //Check if player is in a monster area and set the current coordinate
    bool foundMonster(const Player& p, pair<int, int>  *coordinates) {
        for (auto &enemy : enemiesLocalization) {
            if (p.distanceOf(enemy.first.first, enemy.first.second) < 16) {
                *coordinates = enemy.first;
                return true;
            }
        }

         return false;
    }
//
//    void updateEnemiesLocalization(vector<Monster> monster){
//
//    }
    /*
     * Returns the index of the monster that is selected to be attacked
     */
    int getSelected(vector<Monster> currentMonster){
        for (int i = 0; i < currentMonster.size(); i++) {
            if (currentMonster[i].isSelected) {
                return i;
            }
        }
    }

    // Deselect all monsters
    void deselectAll(vector<Monster> *currentMonster){
        for (int i = 0; i < currentMonster->size(); i++) {
            if (currentMonster->at(i).isSelected) {
                currentMonster->at(i).isSelected = false;
            }
        }
    }

    //Changes the selected monster from right to left
    void changeSelectedRight(vector<Monster> *currentMonster){
        if (currentMonster->size() == 2) {
            if (currentMonster->at(0).isSelected) {
                currentMonster->at(0).isSelected = false;
                currentMonster->at(1).isSelected = true;
            }
        }
        if (currentMonster->size() == 3) {
            if (currentMonster->at(1).isSelected) {
                currentMonster->at(0).isSelected = false;
                currentMonster->at(1).isSelected = false;
                currentMonster->at(2).isSelected = true;
            } else if (currentMonster->at(0).isSelected) {
                currentMonster->at(0).isSelected = false;
                currentMonster->at(1).isSelected = true;
                currentMonster->at(2).isSelected = false;
            }
        }
    }

    //Changes the selected monster from left to right
    void changeSelectedLeft(vector<Monster> *currentMonster){
        if (currentMonster->size() == 2) {
            if (currentMonster->at(1).isSelected) {
                currentMonster->at(0).isSelected = true;
                currentMonster->at(1).isSelected = false;
            }
            else{
                throw logic_error("Can't go to the left");
            }
        }
        if (currentMonster->size() == 3) {
            if (currentMonster->at(1).isSelected) {
                currentMonster->at(0).isSelected = true;
                currentMonster->at(1).isSelected = false;
                currentMonster->at(2).isSelected = false;
            } else if (currentMonster->at(2).isSelected) {
                currentMonster->at(0).isSelected = false;
                currentMonster->at(1).isSelected = true;
                currentMonster->at(2).isSelected = false;
            }
            else{
                throw logic_error("Can't go to the left");
            }
        }
    }


    /*
     * Returns the index of the monster that will be the next to attack
     */
    int getNextToAttack(vector<Monster> currentMonster){
        for (int i = 0; i < currentMonster.size(); i++) {
            if (currentMonster[i].isNextToAttack) {
                cout << "Atacado por " << i << endl;
                return i;
            }
        }

    }

    //Changes the next monster that will attack
    void changeNextoToAttak(vector<Monster> *currentMonster){
        if (currentMonster->size() == 2) {
            if (currentMonster->at(0).isNextToAttack) {
                currentMonster->at(1).isNextToAttack = true;
                currentMonster->at(0).isNextToAttack = false;
            }
            else if(currentMonster->at(1).isNextToAttack){
                currentMonster->at(0).isNextToAttack = true;
                currentMonster->at(1).isNextToAttack = false;
            }
        }
        if (currentMonster->size() == 3) {
            if(currentMonster->at(0).isNextToAttack){
                currentMonster->at(0).isNextToAttack = false;
                currentMonster->at(1).isNextToAttack = true;
                currentMonster->at(2).isNextToAttack = false;
            }
            else if (currentMonster->at(1).isNextToAttack) {
                currentMonster->at(0).isNextToAttack = false;
                currentMonster->at(1).isNextToAttack = false;
                currentMonster->at(2).isNextToAttack = true;
            } else if (currentMonster->at(2).isNextToAttack) {
                currentMonster->at(0).isNextToAttack = true;
                currentMonster->at(1).isNextToAttack = false;
                currentMonster->at(2).isNextToAttack = false;
            }
        }
    }

    // Do certain actions based on state of the battle
    int battleStateUpdate(vector<Monster> *currentMonster, Player *player, pair<int, int> *currentCoordinate){

        // Player Died
        if(not player->alive){
            return 2;
        }

        // ALL MONSTERS DIED
        if(currentMonster->empty()){
            player->exempted = true;
            player->points += 10; // TODO Give the player his points for defeating all monsters;
            enemiesLocalization.erase(*currentCoordinate);
            gameMode = 0;
            return true;
        }


        for (int i = 0; i < currentMonster->size(); i++) {
            // ONE MONSTER DIED
            if(not currentMonster->at(i).alive){

                // Select other monster to attack
                try{
                    changeSelectedLeft(currentMonster);
                }
                catch(logic_error &logicError){
                    changeSelectedRight(currentMonster);
                }
                currentMonster->at(i).clean(); // Clean bitmap
                currentMonster->erase(currentMonster->begin() + i); // Erase monster
            }
        }

        return false;
    }

    void exploringStateUpdate(){
        //TODO CAREFUL WITH LEVELS!! THIS IS FOR PASSING THEN
    }
};