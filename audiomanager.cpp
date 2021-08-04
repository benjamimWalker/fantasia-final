#include <iostream>
#include <allegro5/allegro_audio.h>
using namespace std;

class AudioManger{

public:
    bool isPlaying = false;
private:
    string prologue = "../assets/audios/músicas/prologue.ogg";
    string geral = "../assets/audios/músicas/geral.ogg";
    string battle = "../assets/audios/músicas/batalha.ogg";
    string victory = "../assets/audios/músicas/vitoria.ogg";
    public:

    void playOnce(const string& efecct){
        string sampleToPlay;
        ALLEGRO_SAMPLE *pr = al_load_sample(sampleToPlay.c_str());
        al_reserve_samples(1);
        al_play_sample(pr, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }

    //Receives a level number, plays the corresponding song and return its sample
    ALLEGRO_SAMPLE * playLoop(int level){
        string sampleToPlay;
        switch (level) {
            case 0:
                sampleToPlay = prologue;
                break;
            case 1:
                sampleToPlay = geral;
                break;
            case 2:
                sampleToPlay = battle;
                break;
            case 3:
                sampleToPlay = victory;
                break;
        }
        ALLEGRO_SAMPLE *pr = al_load_sample(sampleToPlay.c_str());
        al_reserve_samples(1);
        al_play_sample(pr, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, nullptr);
        isPlaying = true;
        return pr;
    }

    void stopPlaying(ALLEGRO_SAMPLE *sample){
        al_destroy_sample(sample);
        isPlaying = false;
    }
};