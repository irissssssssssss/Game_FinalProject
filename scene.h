#include "global.h"
#include "charater.h"

void menu_init();
void menu_process(ALLEGRO_EVENT event);
void menu_draw();
void menu_destroy();

void game_scene_init(CHARATER charater);
void game_scene_draw();
void game_scene_destroy();

void chose_init();
void chose_process(ALLEGRO_EVENT event);
void chose_draw();
void chose_destroy();

void setting_init();
void setting_process(ALLEGRO_EVENT event);
void setting_draw();
void setting_destroy();

void help_init();
void help_process(ALLEGRO_EVENT event);
void help_draw();
void help_destroy();
