#include "scene.h"

ALLEGRO_FONT *font = NULL;
ALLEGRO_FONT *font_ins = NULL;
ALLEGRO_BITMAP *game_background = NULL;
ALLEGRO_BITMAP *manu_background = NULL;
ALLEGRO_BITMAP *press = NULL;
ALLEGRO_SAMPLE *manu_song = NULL;
ALLEGRO_SAMPLE_INSTANCE *manu_sample_instance;
ALLEGRO_SAMPLE *game_song = NULL;
ALLEGRO_SAMPLE_INSTANCE *game_sample_instance;

static void song_setting(ALLEGRO_SAMPLE *song, ALLEGRO_SAMPLE_INSTANCE *instance, char *path)
{
    song = al_load_sample(path);
    al_reserve_samples(20);
    instance = al_create_sample_instance(song);
    // Loop the song until the display close
    al_set_sample_instance_playmode(instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(instance, al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(instance, 1);
    al_play_sample_instance(instance);
}
// function of menu
void menu_init()
{ //放主畫面會有的物件
    song_setting(manu_song, manu_sample_instance, "./sound/manu.wav");
    font = al_load_ttf_font("./font/Malapropism.ttf", 28, 0);
    font_ins = al_load_ttf_font("./font/pirulen.ttf", 15, 0);
    manu_background = al_load_bitmap("./image/background.gif");
    press = al_load_bitmap("./image/press.png");
}
void menu_process(ALLEGRO_EVENT event)
{
    if (event.type == ALLEGRO_EVENT_KEY_UP)
        if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) //按下enter進入遊戲
            judge_next_window = true;
    if (event.type == ALLEGRO_EVENT_KEY_UP)
        if (event.keyboard.keycode == ALLEGRO_KEY_S) //按下S進入Setting
            judge_next_window = true;
    if (event.type == ALLEGRO_EVENT_KEY_UP)
        if (event.keyboard.keycode == ALLEGRO_KEY_H) //按下H進入Help
            judge_next_window = true;
}
void menu_draw() //第一畫面內容
{
#if 0
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, 85, ALLEGRO_ALIGN_CENTRE, "START"); //正方形 內有文字
    al_draw_rectangle(WIDTH / 2 - 50, 72, WIDTH / 2 + 50, 108, al_map_rgb(255, 255, 255), 0);
#endif
    al_draw_bitmap(manu_background, 0, 0, 0);
    al_draw_bitmap(press, 650, 80, 0);
    al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, 200, ALLEGRO_ALIGN_CENTRE, "START"); //文字
    al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, 315, ALLEGRO_ALIGN_CENTRE, "SETTING");
    al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, 435, ALLEGRO_ALIGN_CENTRE, "HELP");
    al_draw_text(font_ins, al_map_rgb(255, 255, 255), WIDTH / 2, 560, ALLEGRO_ALIGN_CENTRE, "Press 'ENTER' to START.");
    al_draw_text(font_ins, al_map_rgb(255, 255, 255), WIDTH / 2, 580, ALLEGRO_ALIGN_CENTRE, "Press 'S' to SETTING.");
    al_draw_text(font_ins, al_map_rgb(255, 255, 255), WIDTH / 2, 600, ALLEGRO_ALIGN_CENTRE, "Press 'H' to HELP.");
}
void menu_destroy()
{
    al_destroy_font(font);
    al_destroy_bitmap(manu_background);
    al_destroy_bitmap(press);
    al_destroy_sample(manu_song);
    al_destroy_sample_instance(manu_sample_instance);
}

// function of game_scene
void game_scene_init()
{
    song_setting(game_song, game_sample_instance, "./sound/game_1.mp3");
    character_init();
    game_background = al_load_bitmap("./image/background.gif"); //背景動圖
}
void game_scene_draw() //第二畫面內容 騎士
{
    al_draw_bitmap(game_background, 0, 0, 0); //背景圖
    character_draw();
}
void game_scene_destroy()
{
    al_destroy_bitmap(game_background);
    character_destory();
}

// function of game_setting
void game_setting_init()
{
    song_setting(game_song, game_sample_instance, "./sound/game_1.mp3");
}

void game_settint_draw()
{
    al_draw_bitmap(game_background, 0, 0, 0); //背景圖
}
void game_setting_destroy()
{
    al_destroy_bitmap(game_background);
}

//function of game_help
void game_help_init()
{
    song_setting(game_song, game_sample_instance, "./sound/game_1.mp3");
}

void game_help_draw()
{
    al_draw_bitmap(game_background, 0, 0, 0); //背景圖
    al_draw_text(font_ins, al_map_rgb(255, 255, 255), WIDTH / 2, 560, ALLEGRO_ALIGN_CENTRE, "Press 'ENTER' to START.");
    al_draw_text(font_ins, al_map_rgb(255, 255, 255), WIDTH / 2, 580, ALLEGRO_ALIGN_CENTRE, "Press 'S' to SETTING.");
    al_draw_text(font_ins, al_map_rgb(255, 255, 255), WIDTH / 2, 600, ALLEGRO_ALIGN_CENTRE, "Press 'H' to HELP.");
}
void game_help_destroy()
{
    al_destroy_bitmap(game_background);
}
