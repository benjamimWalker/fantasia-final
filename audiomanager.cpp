#include <iostream>
#include <allegro5/allegro_audio.h>
using namespace std;

class AudioManger{

public:
    bool isPlaying = false;
private:
    string prologue = "../assets/audios/músicas/prologue.ogg";
    string geral = "../assets/audios/músicas/geral.ogg";
    string batalha = "../assets/audios/músicas/batalha.ogg";
    string vitoria = "../assets/audios/músicas/vitoria.ogg";
    string passo = "../assets/audios/efeitos /passo.ogg";
    public:

    void playOnce(const string& efecct){
        string sampleToPlay;
       if (efecct == "passo"){
           sampleToPlay = passo;
       }
        ALLEGRO_SAMPLE *pr = al_load_sample(sampleToPlay.c_str());
        al_reserve_samples(1);
        al_play_sample(pr, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    }
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
                sampleToPlay = batalha;
                break;
            case 3:
                sampleToPlay = vitoria;
                break;
            default:
                cout << "";
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