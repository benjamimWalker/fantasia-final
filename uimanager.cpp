#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

class UIManager{
public:
    void drawLifebar(){
        al_draw_rounded_rectangle(200, 234, 350, 245, 4, 4, al_map_rgb(255, 255, 255), 2);
        al_draw_filled_rounded_rectangle(200, 234, 280, 245, 4, 4, al_map_rgb(255, 255, 255));
    }
};