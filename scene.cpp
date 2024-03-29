#include "scene.h"

ALLEGRO_FONT *font = NULL;
ALLEGRO_FONT *font_ins = NULL;
ALLEGRO_FONT *font_chose = NULL;
ALLEGRO_BITMAP *menu_background = NULL;
ALLEGRO_BITMAP *chose_background = NULL;
ALLEGRO_BITMAP *boygirl_background = NULL;
ALLEGRO_BITMAP *game_background = NULL;
ALLEGRO_BITMAP *setting_background = NULL;
ALLEGRO_BITMAP *setting_volume = NULL;
ALLEGRO_BITMAP *help_background = NULL;
ALLEGRO_BITMAP *press = NULL;
ALLEGRO_BITMAP *heart = NULL;
ALLEGRO_BITMAP *coin_count = NULL;
ALLEGRO_BITMAP *help_frame = NULL;
ALLEGRO_SAMPLE *menu_song = NULL;
ALLEGRO_SAMPLE_INSTANCE *menu_sample_instance = NULL;
ALLEGRO_SAMPLE *game_song = NULL;
ALLEGRO_SAMPLE_INSTANCE *game_sample_instance = NULL;
float volume_value = 1.0;

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
    song_setting(menu_song, menu_sample_instance, "./sound/game_3.wav");
    font = al_load_ttf_font("./font/Malapropism.ttf", 28, 0);
    font_ins = al_load_ttf_font("./font/pirulen.ttf", 15, 0);
    menu_background = al_load_bitmap("./image/background.jpeg");
    press = al_load_bitmap("./image/press.png");
}
void menu_process(ALLEGRO_EVENT event)
{
    if (event.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_ENTER:
            judge_next_window = CHOSE_CHAR;
            break;
        case ALLEGRO_KEY_S:
            judge_next_window = SETTING;
            break;
        case ALLEGRO_KEY_H:
            judge_next_window = HELP;
            break;
        default:
            break;
        }
    }
#if 0
    if (event.type == ALLEGRO_EVENT_KEY_UP)
        if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) //按下enter進入遊戲
            judge_next_window = START;
    if (event.type == ALLEGRO_EVENT_KEY_UP)
        if (event.keyboard.keycode == ALLEGRO_KEY_S) //按下S進入Setting
            judge_next_window = SETTING;
    if (event.type == ALLEGRO_EVENT_KEY_UP)
        if (event.keyboard.keycode == ALLEGRO_KEY_H) //按下H進入Help
            judge_next_window = HELP;
#endif
}
void menu_draw() //第一畫面內容
{
#if 0
    al_clear_to_color(al_map_rgb(100, 100, 100));
    al_draw_text(font, al_map_rgb(255, 255, 255), WIDTH / 2, 85, ALLEGRO_ALIGN_CENTRE, "START"); //正方形 內有文字
    al_draw_rectangle(WIDTH / 2 - 50, 72, WIDTH / 2 + 50, 108, al_map_rgb(255, 255, 255), 0);
#endif
    al_draw_bitmap(menu_background, 0, 0, 0);
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
    //al_destroy_font(font);
    al_destroy_bitmap(menu_background);
    al_destroy_bitmap(press);
    //al_destroy_sample(menu_song);
    //al_destroy_sample_instance(menu_sample_instance);
}

// function of game_scene
void game_scene_init(CHARATER charater)
{
    //song_setting(game_song, game_sample_instance, "./sound/game_2.wav");
    song_setting(menu_song, menu_sample_instance, "./sound/game_2.wav");
    character_init(charater);
    littleMonster_init(); //小怪物
    //bigMonster_init(bigMonster); //魔王
    game_background = al_load_bitmap("./image/background.jpeg"); //背景動圖
    heart = al_load_bitmap("./image/heart.png");
    coin_count = al_load_bitmap("./image/m_icon.png");
}
void game_scene_draw() //第二畫面內容 騎士
{
    al_draw_bitmap(game_background, 0, 0, 0); //背景圖
    al_draw_bitmap(heart, 10, 10, 0);
    al_draw_bitmap(heart, 60, 10, 0);
    al_draw_bitmap(heart, 110, 10, 0);
    al_draw_bitmap(coin_count, 10, 60, 0);
    character_draw();
    littleMonster_draw();
}
void game_scene_destroy()
{
    al_destroy_bitmap(game_background);
    al_destroy_bitmap(heart);
    al_destroy_bitmap(coin_count);
    character_destory();
    littleMonster_destroy();
}

// function of game_setting
void chose_init()
{
    //song_setting(game_song, game_sample_instance, "./sound/game_1.mp3");
    font_chose = al_load_ttf_font("./font/pirulen.ttf", 30, 0);
    chose_background = al_load_bitmap("./image/background.jpeg"); //背景動圖
    boygirl_background = al_load_bitmap("./image/boygirl.png");   //背景動圖
}
void chose_process(ALLEGRO_EVENT event)
{
    if (event.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_B:
            chose_charater = BOY;
            judge_next_window = START;
            break;
        case ALLEGRO_KEY_G:
            chose_charater = GIRL;
            judge_next_window = START;
            break;
        default:
            break;
        }
    }
}
void chose_draw()
{
    al_draw_bitmap(chose_background, 0, 0, 0);      //背景圖
    al_draw_bitmap(boygirl_background, 480, 10, 0); //背景圖
    al_draw_text(font_chose, al_map_rgb(255, 255, 255), WIDTH / 2, 250, ALLEGRO_ALIGN_CENTRE, "Press 'B' to chose 'BOY'.");
    al_draw_text(font_chose, al_map_rgb(255, 255, 255), WIDTH / 2, 290, ALLEGRO_ALIGN_CENTRE, "Press 'G' to chose 'Girl'.");
}
void chose_destroy()
{
    al_destroy_bitmap(chose_background);
}

void setting_init()
{
    //song_setting(game_song, game_sample_instance, "./sound/game_1.mp3");
    setting_background = al_load_bitmap("./image/background.jpeg"); //背景動圖
    setting_volume = al_load_bitmap("./image/volume.png");          //背景動圖
}
void setting_process(ALLEGRO_EVENT event)
{
    if (event.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_BACKSPACE:
            judge_next_window = MENU;
            break;
        /*無法支援動態修改*/
        case ALLEGRO_KEY_PAD_MINUS:
        {
            //al_set_sample_instance_gain(menu_sample_instance, volume_value);
        }
        break;
        case ALLEGRO_KEY_PAD_PLUS:
        {
            //al_set_sample_instance_gain(menu_sample_instance, volume_value);
        }
        break;
        }
    }
}
void setting_draw()
{
    al_draw_bitmap(setting_background, 0, 0, 0); //背景圖
    al_draw_bitmap(setting_volume, 600, 0, 0);   //背景圖
    al_draw_text(font_ins, al_map_rgb(255, 255, 255), WIDTH / 2, 580, ALLEGRO_ALIGN_CENTRE, "Press 'BACKSPACE' to MENU.");
}
void setting_destroy()
{
    al_destroy_bitmap(setting_background);
    al_destroy_bitmap(setting_volume);
}

//function of game_help
void help_init()
{
    //song_setting(game_song, game_sample_instance, "./sound/game_1.mp3");
    help_background = al_load_bitmap("./image/background.jpeg"); //背景動圖
    help_frame = al_load_bitmap("./image/help.png");             //背景動圖
}
void help_process(ALLEGRO_EVENT event)
{
    if (event.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch (event.keyboard.keycode)
        {
        case ALLEGRO_KEY_BACKSPACE:
            judge_next_window = MENU;
            break;
        default:
            break;
        }
    }
}
void help_draw()
{
    al_draw_bitmap(help_background, 0, 0, 0); //背景圖
    al_draw_bitmap(help_frame, 600, 0, 0);    //背景圖
    al_draw_text(font_ins, al_map_rgb(255, 255, 255), WIDTH / 2, 580, ALLEGRO_ALIGN_CENTRE, "Press 'BACKSPACE' to MENU.");
}
void help_destroy()
{
    al_destroy_bitmap(help_background);
    al_destroy_bitmap(help_frame);
}
