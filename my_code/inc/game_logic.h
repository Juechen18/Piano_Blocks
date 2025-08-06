/*游戏逻辑声明*/
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "lvgl/lvgl.h"

// 游戏开始回调
void game_start_music1_cb(lv_event_t *e);
void game_start_music2_cb(lv_event_t *e);

// 处理方块点击
void handle_block_click(lv_event_t *e);
void handle_long_press(lv_event_t *e);
void handle_wrong_click();

// 游戏控制
void reset_game();
void create_piano_block();
void moving_piano_blocks(lv_timer_t *timer);

#endif