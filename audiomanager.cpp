#include <iostream>
#include <allegro5/allegro_audio.h>

using std::string;

class AudioManger{
public:
    bool isPlaying = false;
    string whatIsPlaying;
private:
    string prologue = "../assets/audios/músicas/prologue.ogg";
    string geral = "../assets/audios/músicas/geral.ogg";
    string battle = "../assets/audios/músicas/batalha.ogg";
    string victory = "../assets/audios/músicas/vitoria.ogg";
    string lose = "../assets/audios/músicas/derrota.ogg";
    string alan_hurt = "../assets/audios/efeitos/alan_hurt.ogg";
    string ada_hurt = "../assets/audios/efeitos/ada_hurt.ogg";
    string monster_hurt = "../assets/audios/efeitos/monster_hurt.ogg";

    public:
    void playOnce(const string& effect, const string& playerName){
        string sampleToPlay;
        if(effect == "hurt"){
            if(playerName == "ada"){
                sampleToPlay = ada_hurt;
            }
            else{
                sampleToPlay = alan_hurt;
            }
        };

        if (effect == "m_hurt"){
            sampleToPlay = monster_hurt;
        }
        ALLEGRO_SAMPLE *pr = al_load_sample(sampleToPlay.c_str());
        al_reserve_samples(2);
        if(playerName == "ada")
            al_play_sample(pr, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
        else al_play_sample(pr, 0.4, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);

    }

    //Receives a level number, plays the corresponding song and return its sample
    ALLEGRO_SAMPLE * playLoop(int level){
        string sampleToPlay;
        switch (level) {
            case 0:
                sampleToPlay = prologue;
                whatIsPlaying = "prologue";
                break;
            case 1:
                sampleToPlay = geral;
                whatIsPlaying = "geral";
                break;
            case 2:
                sampleToPlay = battle;
                whatIsPlaying = "battle";
                break;
            case 3:
                sampleToPlay = victory;
                whatIsPlaying = "victory";
                break;
            case 4:
                sampleToPlay = lose;
                whatIsPlaying = "lose";
        }
        ALLEGRO_SAMPLE *pr = al_load_sample(sampleToPlay.c_str());
        al_reserve_samples(2);
        if(whatIsPlaying == "prologue")
            al_play_sample(pr, 0.15, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, nullptr);
        else
            al_play_sample(pr, 0.06, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, nullptr);

        isPlaying = true;
        return pr;
    }

    void stopPlaying(ALLEGRO_SAMPLE *sample){
        al_destroy_sample(sample);
        isPlaying = false;
    }

    string whatIsNowPlaying() const{
        return whatIsPlaying;
    }
};