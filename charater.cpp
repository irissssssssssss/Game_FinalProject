#include "charater.h"

// the state of character
enum //角色會出現的行為
{
    STOP = 0,
    MOVE,
    ATK,
    JUMP
};
#define LITTLE_MONSTER_NUM 20
typedef struct character //角色位置
{
    int x, y;                            // the position of image
    int width, height;                   // the width and height of image
    bool dir;                            // left: false, right: true 方向
    int state;                           // the state of character
    ALLEGRO_BITMAP *img_move[2];         //移動 用兩張圖
    ALLEGRO_BITMAP *img_atk[2];          // 攻擊 用兩張圖
    ALLEGRO_BITMAP *img_hurt[2];          // 攻擊 用兩張圖
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;  //跳躍之音效
    ALLEGRO_SAMPLE_INSTANCE *jump_Sound; //跳躍之音效
    ALLEGRO_SAMPLE_INSTANCE *gameover_Sound; //跳躍之音效
    //控制連續動作之動畫時間(以下)
    int anime;      // counting the time of animation
    int anime_time; // indicate how long the animation
    bool hurt = 0;
    int dead_time = 0;
} Character;
Character chara;
Character littleMonster[LITTLE_MONSTER_NUM];
Character bigMonster;
bool dead = false;
bool killmonster = false;
ALLEGRO_BITMAP *gameover_bitmap = NULL;         //移動 用兩張圖

ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_SAMPLE *jump_effectsound = NULL;
ALLEGRO_SAMPLE *gameover_sound = NULL;
const int jump_val_init = 50;
int jump_val;
#define JUMP_VEC 3

void character_init(CHARATER charater)
{
    // load character images
    for (int i = 1; i <= 2; i++)
    {
        char temp[50];
        if (charater == BOY)
            sprintf(temp, "./image/boy_%d.png", i); //用迴圈 一張張把圖片load進去
        else if (charater == GIRL)
            sprintf(temp, "./image/girl_%d.png", i); //用迴圈 一張張把圖片load進去
        chara.img_move[i - 1] = al_load_bitmap(temp);
    }
    for (int i = 1; i <= 2; i++)
    {
        char temp[50];
        sprintf(temp, "./image/char_atk%d.png", i);
        chara.img_atk[i - 1] = al_load_bitmap(temp);
    }
    // load effective sound
    sample = al_load_sample("./sound/GetCoin.wav");
    chara.atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(chara.atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.atk_Sound, al_get_default_mixer());
    al_set_sample_instance_gain(chara.atk_Sound, 2);

    jump_effectsound = al_load_sample("./sound/jump.wav");
    chara.jump_Sound = al_create_sample_instance(jump_effectsound);
    al_set_sample_instance_playmode(chara.jump_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.jump_Sound, al_get_default_mixer());
    //加下面兩行音量調整 會說找不到函式
    al_set_sample_instance_gain(chara.jump_Sound, 0.5);

    gameover_sound = al_load_sample("./sound/GameFail.wav");
    chara.gameover_Sound = al_create_sample_instance(gameover_sound);
    al_set_sample_instance_playmode(chara.gameover_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.gameover_Sound, al_get_default_mixer());
    al_set_sample_instance_gain(chara.gameover_Sound, 2);

    // initial the geometric information of character
    chara.width = al_get_bitmap_width(chara.img_move[0]);
    chara.height = al_get_bitmap_height(chara.img_move[0]);
    chara.x = 0;
    chara.y = 0;
    chara.dir = false;

    // initial the animation component
    chara.state = STOP;
    chara.anime = 0;
    chara.anime_time = 30; //一動作xx幀
    gameover_bitmap = al_load_bitmap("./image/gameover.png");
}
void charater_process(ALLEGRO_EVENT event)
{
    // process the animation 畫動畫
    if (event.type == ALLEGRO_EVENT_TIMER)
    {
        if (event.timer.source == fps)
        {
            chara.anime++;
            chara.anime %= chara.anime_time; //％是為了知道 到最後一幀ex:29時下一個要結束
            for (int i = 0; i < LITTLE_MONSTER_NUM; ++i) {
                littleMonster[i].anime_time = chara.anime_time;
                littleMonster[i].anime = chara.anime; //％是為了知道 到最後一幀ex:29時下一個要結束
            }
        }
        // process the keyboard event
    }
    else if (event.type == ALLEGRO_EVENT_KEY_DOWN) //按 往下 執行動作
    {
        key_state[event.keyboard.keycode] = true;
        chara.anime = 0;
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP)
    {
        key_state[event.keyboard.keycode] = false;
    }
}
void charater_update()
{
    if (chara.y != 0)
    {
        chara.y -= jump_val;
        jump_val -= JUMP_VEC;
        if (chara.y >= 0 && !dead)
        {
            chara.y = 0;
            jump_val = jump_val_init;
        }
    }

    if (key_state[ALLEGRO_KEY_SPACE] || killmonster || dead)
    {
        if (!dead && !killmonster && chara.y == 0)
        {
            al_play_sample_instance(chara.jump_Sound); //跳躍之音效
            chara.y -= jump_val;
            jump_val -= JUMP_VEC;
        }
        else if (dead) 
        {
            chara.y -= jump_val;
            jump_val -= JUMP_VEC;
        }
        else if (killmonster)
        {
            jump_val -= 10;
            chara.y -= jump_val;
            jump_val -= JUMP_VEC;
            killmonster = false;
        }
    }

    if (key_state[ALLEGRO_KEY_LEFT])
    {
        chara.dir = false;
        chara.x -= 5;
        chara.state = MOVE;
    }
    /*else if (key_state[ALLEGRO_KEY_S])
    {
        chara.y += 5;
        chara.state = MOVE;
    }*/
    else if (key_state[ALLEGRO_KEY_RIGHT])
    {
        chara.dir = true;
        chara.x += 5;
        chara.state = MOVE;
    }
    else if (chara.anime == chara.anime_time - 1) //30幀結束
    {
        chara.anime = 0;
        chara.state = STOP;
    }
    else if (chara.anime == 0)
    {
        chara.state = STOP;
    }
}
void character_draw()
{
    if (dead)
        al_draw_bitmap(gameover_bitmap, 600, 200, 0);
    if (chara.state == STOP) //角色什麼都不做時 在stop
    {
        if (chara.dir) //角色面對方向
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, 0);
        else
            al_draw_bitmap(chara.img_move[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
    }
    else if (chara.state == MOVE) //角色移動之動畫
    {
        if (chara.dir)
        {
            if (chara.anime < chara.anime_time / 2) //移動之第一張圖
            {
                al_draw_bitmap(chara.img_move[0], chara.x, chara.y, 0);
            }
            else //大於 移動之第二張圖
            {
                al_draw_bitmap(chara.img_move[1], chara.x, chara.y, 0);
            }
        }
        else
        {
            if (chara.anime < chara.anime_time / 2)
            {
                al_draw_bitmap(chara.img_move[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }
            else
            {
                al_draw_bitmap(chara.img_move[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }
        }
    }
    else if (chara.state == ATK) //攻擊動作 之連續動畫
    {
        if (chara.dir)
        {
            if (chara.anime < chara.anime_time / 2) //攻擊舉劍動作 維持時間
            {
                al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
            }
            else //大於時 做 攻擊揮劍動作
            {
                al_draw_bitmap(chara.img_atk[1], chara.x, chara.y, ALLEGRO_FLIP_HORIZONTAL);
                al_play_sample_instance(chara.atk_Sound); //揮劍之音效
            }
        }
        else
        {
            if (chara.anime < chara.anime_time / 2)
            {
                al_draw_bitmap(chara.img_atk[0], chara.x, chara.y, 0);
            }
            else
            {
                al_draw_bitmap(chara.img_atk[1], chara.x, chara.y, 0);
                al_play_sample_instance(chara.atk_Sound);
            }
        }
    }
}
void character_destory()
{
    al_destroy_bitmap(chara.img_atk[0]);
    al_destroy_bitmap(chara.img_atk[1]);
    al_destroy_bitmap(chara.img_move[0]);
    al_destroy_bitmap(chara.img_move[1]);
    al_destroy_sample_instance(chara.atk_Sound);
}

void littleMonster_init() 
{
    for (int i = 0; i < LITTLE_MONSTER_NUM; ++i) {
        for (int j = 1; j <= 2; ++j) {
            char temp[50];
            sprintf(temp, "./image/littleMonster_%d.png", j); //用迴圈 一張張把圖片load進去
            littleMonster[i].img_move[j-1] = al_load_bitmap(temp);
            char temp1[50];
            sprintf(temp1, "./image/hurt.png", j); //用迴圈 一張張把圖片load進去
            littleMonster[i].img_hurt[j-1] = al_load_bitmap(temp1);
        }
    }

    for (int i = 0; i < LITTLE_MONSTER_NUM; ++i) {
        littleMonster[i].width = al_get_bitmap_width(littleMonster[i].img_move[0]);
        littleMonster[i].height = al_get_bitmap_height(littleMonster[i].img_move[0]);
        littleMonster[i].x = 1500 + 300 * i;
        littleMonster[i].y = 370;
        littleMonster[i].dir = false;
    }
}

void littleMonster_process(ALLEGRO_EVENT event)
{
    // process the animation 畫動畫
    for (int i = 0; i < LITTLE_MONSTER_NUM; ++i) {
        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            if (event.timer.source == fps)
            {
                littleMonster[i].anime++;
                littleMonster[i].anime %= littleMonster[i].anime_time; //％是為了知道 到最後一幀ex:29時下一個要結束
            }
            // process the keyboard event
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) //按 往下 執行動作
        {
            key_state[event.keyboard.keycode] = true;
            littleMonster[i].anime = 0;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP)
        {
            key_state[event.keyboard.keycode] = false;
        }
    }
}
void littleMonster_update() 
{
    if (dead)
        return;
    int diffx, diffy;
    for (int i = 0; i < LITTLE_MONSTER_NUM; ++i) {
        if (!littleMonster[i].hurt) {
            littleMonster[i].x -= 2;
            diffx = littleMonster[i].x > chara.x ? littleMonster[i].x - chara.x: chara.x - littleMonster[i].x;
            diffy = littleMonster[i].y >chara.y ? littleMonster[i].y - chara.y:chara.y - littleMonster[i].y;
            if (diffx < 450 && diffx >= 400 && diffy < 420) {
                dead = true;
                al_play_sample_instance(chara.gameover_Sound);
            } else if (diffx < 400  && diffy < 420) {
                littleMonster[i].hurt = true;
                killmonster = true;
                al_play_sample_instance(chara.atk_Sound);
            }
        }
    }
}
void littleMonster_draw() 
{
    // with the state, draw corresponding image
    for (int i = 0; i < LITTLE_MONSTER_NUM; ++i) {
        if (littleMonster[i].anime < littleMonster[i].anime_time / 2) //移動之第一張圖
            if (!littleMonster[i].hurt)
                al_draw_bitmap(littleMonster[i].img_move[0], littleMonster[i].x, littleMonster[i].y, 0);
            else
                al_draw_bitmap(littleMonster[i].img_hurt[0], littleMonster[i].x, littleMonster[i].y, 0);
        else
            if (!littleMonster[i].hurt)
                al_draw_bitmap(littleMonster[i].img_move[1], littleMonster[i].x, littleMonster[i].y, 0);
            else
                al_draw_bitmap(littleMonster[i].img_hurt[1], littleMonster[i].x, littleMonster[i].y, 0);
    }
}
void littleMonster_destroy() 
{
    for (int i = 0; i < LITTLE_MONSTER_NUM; ++i) {
        al_destroy_bitmap(littleMonster[i].img_move[0]);
        al_destroy_bitmap(littleMonster[i].img_move[1]);
    }
}

void bigMonster_init() {}
void bigMonster_update() {}
void bigMonster_draw() {}
void bigMonster_destroy() {}
