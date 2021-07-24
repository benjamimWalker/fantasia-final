#include <allegro5/allegro5.h>
#include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include "monster.cpp"

using namespace std;

class GameManager{
public:
    static int gameMode;
    static bool isGameFinished;
    static unsigned short numEnemies; // 18

    static map<vector<Monster>, pair<int, int>> enemiesLocalization;

    vector<Monster> populateEnemies(){
        vector<Monster> vec;
        for (int i = 0; i < (random() % 3) + 1; i++){
            vec.emplace_back(Monster("path" + to_string(i)));
        }
        return vec;
    }

    void sortPositions(int width, int height){
        for (int j = 0; j < numEnemies; j++) {
            cout << enemiesLocalization.size() << endl;
            vector<Monster> v = populateEnemies();
           // srand (time (0));
              enemiesLocalization.insert(pair<vector<Monster>, pair<int, int>> (v, pair<int, int>(random() % width, random() % height)));
        }
    }

    //TODO CHECAR ESSE BOOL
    bool foundMonster(int playerXCoord, int playerYCoord){
        bool pReturner = false;
        for (auto & enemy : enemiesLocalization){
            if(enemy.second.first == playerXCoord && enemy.second.second == playerXCoord){
                pReturner = true;
            }
        }
        return pReturner;
    }
};

map<vector<Monster>, pair<int, int>> GameManager::enemiesLocalization;
unsigned short GameManager::numEnemies = 18;

int main(){
    GameManager gameManager;

    gameManager.sortPositions(1280, 720);

    for(const auto& enemy: GameManager::enemiesLocalization){
        for (auto monster: enemy.first){
            cout << monster.spritePath << " " << enemy.second.first << " " << enemy.second.second << endl;
        }
        cout << endl;
    }
    return 0;
}
