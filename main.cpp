#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>

int main()
{
    ALLEGRO_DISPLAY *tela;

    tela = al_create_display(720,480);
    al_show_native_message_box(NULL, "Title", "Heading", "Text", "Button", ALLEGRO_MESSAGEBOX_ERROR);
    return 0;
}
