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
    static unsigned short gameMode;
    static bool isGameFinished;
    static unsigned short numEnemies; // 18
    static unsigned short level;

   /* One of the most important piece of data
    * it has a link between a vector with 1 to 3 monsters
    * and a random coordinate
    */
    static map<vector<Monster>, pair<int, int>> enemiesLocalization;

    vector<Monster> populateEnemies(){
        vector<Monster> vec;
        for (int i = 0; i < (random() % 3) + 1; i++){
            vec.emplace_back(Monster());
        }
        return vec;
    }

    void sortPositions(int width, int height){
        for (int j = 0; j < numEnemies; j++) {
            vector<Monster> v = populateEnemies();
           // srand (time (0));
              enemiesLocalization.insert(pair<vector<Monster>, pair<int, int>> (v, pair<int, int>(random() % width, random() % height)));
        }
    }

     bool foundMonster(const Player& p, vector<Monster> * monsters) {
        for (auto &enemy : enemiesLocalization) {
            if (p.distanceOf(enemy.second.first, enemy.second.second) < 16) {
                *monsters = enemy.first;
                return true;
            }
        }
         return false;
    }
};


//map<vector<Monster>, pair<int, int>> GameManager::enemiesLocalization;
//unsigned short GameManager::numEnemies = 10;
//int main(){
//    srand (time (0));
//    GameManager gameManager;
//
//    gameManager.sortPositions(1280, 720);
//
//    for(const auto& enemy: GameManager::enemiesLocalization){
//        for (auto monster: enemy.first){
//            cout << monster.spritePath << " " << enemy.second.first << " " << enemy.second.second << endl;
//        }
//        cout << endl;
//    }
//    return 0;
//}
