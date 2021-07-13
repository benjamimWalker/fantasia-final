#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>

int main()
{
    al_init();
    ALLEGRO_DISPLAY *tela;

    tela = al_create_display(720,480);
    al_show_native_message_box(NULL, "Title", "Heading", "Text", "Button", NULL);

    return 0;
}
