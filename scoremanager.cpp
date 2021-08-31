#include <fstream>
#include <iostream>

using std::ifstream;
using std::ofstream;

class ScoreManager{
public:
    // Creating file to store a score
    void createFile(){
        ofstream file;
        file.open("records.txt");
        file << "0";
        file.close();
    }

    // Returns the current record
    int getRecord(){
        int record;
        ifstream file;
        file.open("records.txt");
        if (not file.is_open()){
            createFile();
            file.open("records.txt");
        }
        file >> record;
        file.close();
        return record;
    }

    //set a new score
    void setRecord(int score){
        ofstream file;
        file.open("records.txt");
        file << score;
        file.close();
    }
};
