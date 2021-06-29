#include "GameWindow.h"

bool draw = false;

const char *title = "Final Project 10603124550 詹敦潔 / 1092169S 陳思妤";

// ALLEGRO Variables
ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_SAMPLE *song = NULL;
ALLEGRO_SAMPLE_INSTANCE *sample_instance;

int Game_establish()
{
    int msg = 0;

    game_init(); //init基礎參數
    game_begin();

    while (msg != GAME_TERMINATE)
    {
        msg = game_run();
        if (msg == GAME_TERMINATE)
            printf("Game Over\n");
    }

    game_destroy();
    return 0;
}

void game_init()
{
    printf("Game Initializing...\n");
    al_init();
    // init audio
    al_install_audio();
    al_init_acodec_addon();
    // Create display
    display = al_create_display(WIDTH, HEIGHT);
    // create event queue
    event_queue = al_create_event_queue();
    // Initialize Allegro settings
    al_set_window_position(display, 0, 0);
    al_set_window_title(display, title);
    al_init_primitives_addon();
    al_init_font_addon();   // initialize the font addon
    al_init_ttf_addon();    // initialize the ttf (True Type Font) addon
    al_init_image_addon();  // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon
    al_install_keyboard();  // install keyboard event
    al_install_mouse();     // install mouse event
    al_install_audio();     // install audio event

    // Register event
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    /*以下為Timer*/
    fps = al_create_timer(1.0 / FPS); //FP定義在global.cc
    al_register_event_source(event_queue, al_get_timer_event_source(fps));
    // initialize the icon on the display 遊戲畫面左上角的icon
    ALLEGRO_BITMAP *icon = al_load_bitmap("./image/icon.jpg");
    al_set_display_icon(display, icon);
}

void game_begin()
{
#if 0
    // Load sound 片頭音樂
    song = al_load_sample("./sound/MENU.wav");
    al_reserve_samples(20);
    sample_instance = al_create_sample_instance(song);
    // Loop the song until the display closes
    al_set_sample_instance_playmode(sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(sample_instance, 1);
    al_play_sample_instance(sample_instance);
#endif
    al_start_timer(fps);
    // initialize the menu before entering the loop
    menu_init(); //主畫面內容 在scene.cpp
}
void game_update()
{
    switch(judge_next_window) {
    case NONE:
        break;
    case MENU:
        window = MENU;
        break;
    case START:
        switch (window) {
        case START:
            charater_update();
            littleMonster_update();
            break;
        case CHOSE_CHAR:
            chose_destroy();
            menu_destroy();
            game_scene_init(chose_charater);
            window = START;
            break;
        default:
            break;
        }
        break;
    case CHOSE_CHAR:
        chose_init();
        judge_next_window = NONE;
        window = CHOSE_CHAR;
        break;
    case SETTING:
        setting_init();
        judge_next_window = NONE;
        window = SETTING;
        break;
    case HELP:
        help_init();
        judge_next_window = NONE;
        window = HELP;
        break;
    default:
        break;
    }
#if 0
    if (judge_next_window == START)
    {
        if (window == MENU)
        {
            // not back menu anymore, therefore destroy it
            menu_destroy();
            // initialize next scene
            game_scene_init();
            window = START;
        }
        if (window == START)
        {
            charater_update();
        }
    }
    else if (judge_next_window == SETTING)
    {
        game_setting_init();
        window = SETTING;
        judge_next_window = NONE;
    }
    else if (judge_next_window == HELP)
    {
        game_help_init();
    }
#endif
}
int process_event()
{
    // Request the event
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    // process the event of other component
    switch (window) {
        case MENU:
            menu_process(event);
            break;
        case CHOSE_CHAR:
            chose_process(event);
            break;
        case START:
            charater_process(event);
            break;
        case SETTING:
            setting_process(event);
            break;
        case HELP:
            help_process(event);
            break;
        default:
            break;
    }
#if 0
    if (window == 1)
    {
        menu_process(event);
    }
    else if (window == 2)
    {
        charater_process(event);
    }
#endif

    // Shutdown our program 遊戲畫面右上角的叉叉
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        return GAME_TERMINATE;
    else if (event.type == ALLEGRO_EVENT_TIMER)
        if (event.timer.source == fps)
            draw = true;
    if (draw)
        game_update();
    return 0;
}
void game_draw() //範例遊戲有兩個畫面 所以開兩個window
{
    switch(window) {
        case MENU:
            menu_draw();
            break;
        case CHOSE_CHAR:
            chose_draw();
            break;
        case START:
            game_scene_draw();
            break;
        case SETTING:
            setting_draw();
            break;
        case HELP:
            help_draw();
            break;
    }
#if 0
    if (window == 1) // 定義在本頁第一行
    {
        menu_draw();
    }
    else if (window == 2)
    {
        game_scene_draw();
    }
#endif
    al_flip_display();
}
int game_run()
{
    int error = 0;
    if (draw)
    {
        game_draw();
        draw = false;
    }
    if (!al_is_event_queue_empty(event_queue))
    {
        error = process_event();
    }
    return error;
}

void game_destroy()
{
    // Make sure you destroy all things
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    game_scene_destroy();
}
