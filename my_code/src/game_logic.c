/*game_logic.c*/
#include "game_logic.h" // 游戏逻辑头文件
#include "ui_manager.h" // UI管理头文件
#include "utils.h"      // 工具函数头文件
#include <stdio.h>      // 标准输入输出头文件
#include <stdlib.h>     // 标准库头文件
#include <time.h>       // 时间相关头文件

// 游戏常量定义
#define BLOCK_HEIGHT 100  // 钢琴块高度
#define BLOCK_WIDTH 200   // 钢琴块宽度
#define NUM_TRACKS 4      // 轨道数量（游戏区域分为4列）
#define BLOCK_SPEED 5     // 方块下落速度
#define SCREEN_HEIGHT 480 // 屏幕高度
#define MAX_BLOCKS 10     // 最大同时存在的方块数量

// 游戏状态变量
static int score = 0;                // 当前游戏分数
static lv_obj_t *blocks[MAX_BLOCKS]; // 存储所有方块的数组
static int block_count = 0;          // 当前存在的方块数量
static bool game_active = false;     // 游戏是否处于活动状态
static bool music_playing = false;   // 音乐是否正在播放

// 初始化游戏
void init_game()
{
    srand(time(NULL)); // 使用当前时间初始化随机数种子
}

// 游戏开始回调函数（音乐1）
void game_start_music1_cb(lv_event_t *e)
{
    play_music("music1.mp3"); // 播放音乐1
    music_playing = true;     // 设置音乐播放状态
    show_game_screen();       // 显示游戏界面
    reset_game();             // 重置游戏状态
}

// 游戏开始回调函数（音乐2）
void game_start_music2_cb(lv_event_t *e)
{
    play_music("music2.mp3"); // 播放音乐2
    music_playing = true;     // 设置音乐播放状态
    show_game_screen();       // 显示游戏界面
    reset_game();             // 重置游戏状态
}

// 处理方块点击（黑色块）
void handle_block_click(lv_event_t *e)
{
    if (!game_active) // 如果游戏未激活则直接返回
        return;

    score++; // 增加1分
    // 更新分数显示
    lv_label_set_text_fmt(score_label, "分数为%d", score);
    // 隐藏被点击的方块
    lv_obj_add_flag(e->current_target, LV_OBJ_FLAG_HIDDEN);
    // 创建新方块
    create_piano_block();
}

// 处理长按（灰色块）
void handle_long_press(lv_event_t *e)
{
    if (!game_active) // 如果游戏未激活则直接返回
        return;

    score += 3; // 增加3分
    // 更新分数显示
    lv_label_set_text_fmt(score_label, "分数为%d", score);
    // 隐藏被长按的方块
    lv_obj_add_flag(e->current_target, LV_OBJ_FLAG_HIDDEN);
    // 创建新方块
    create_piano_block();
}

// 游戏结束处理
void handle_wrong_click()
{
    if (!game_active) // 如果游戏未激活则直接返回
        return;

    game_active = false;       // 设置游戏为非活动状态
    music_playing = false;     // 设置音乐停止状态
    stop_music();              // 停止音乐播放
    show_result_screen(score); // 显示结果界面并传递当前分数
    score = 0;                 // 重置分数
}

// 重置游戏状态
void reset_game()
{
    // 清除所有现有的方块
    for (int i = 0; i < block_count; i++)
    {
        if (blocks[i]) // 如果方块存在
        {
            lv_obj_del(blocks[i]); // 删除方块对象
        }
    }

    // 重置游戏状态变量
    score = 0;          // 重置分数
    block_count = 0;    // 重置方块计数
    game_active = true; // 设置游戏为活动状态
    // 重置分数显示
    lv_label_set_text(score_label, "分数为0");

    // 创建初始方块（2个）
    create_piano_block();
    create_piano_block();

    // 创建并启动方块移动定时器（30ms间隔）
    lv_timer_create(moving_piano_blocks, 30, NULL);
}

// 创建新钢琴块
void create_piano_block()
{
    if (block_count >= MAX_BLOCKS) // 如果已达到最大方块数量则返回
        return;

    // 创建新的方块对象
    lv_obj_t *block = lv_obj_create(game_screen);
    lv_obj_set_size(block, BLOCK_WIDTH, BLOCK_HEIGHT); // 设置方块尺寸

    // 随机选择轨道（0-3）
    int lane = rand() % NUM_TRACKS;
    // 计算x坐标（根据轨道位置）
    int x_pos = lane * (800 / NUM_TRACKS);
    // 初始y坐标（屏幕上方）
    int y_pos = -100;

    lv_obj_set_pos(block, x_pos, y_pos); // 设置方块位置

    // 随机决定方块类型（50%概率）
    if (rand() % 2 == 0)
    {
        // 黑色方块 - 点击得分
        lv_obj_set_style_bg_color(block, lv_color_hex(0x000000), LV_PART_MAIN);
        // 添加点击事件回调
        lv_obj_add_event_cb(block, handle_block_click, LV_EVENT_CLICKED, NULL);
    }
    else
    {
        // 灰色方块 - 长按得分
        lv_obj_set_style_bg_color(block, lv_color_hex(0x808080), LV_PART_MAIN);
        // 添加长按事件回调
        lv_obj_add_event_cb(block, handle_long_press, LV_EVENT_LONG_PRESSED, NULL);
    }

    // 将新方块添加到方块数组
    blocks[block_count++] = block;
}

// 方块移动逻辑（定时器回调函数）
void moving_piano_blocks(lv_timer_t *timer)
{
    if (!game_active) // 如果游戏未激活则直接返回
        return;

    // 检查音乐是否结束
    if (!music_playing)
    {
        handle_wrong_click(); // 音乐结束则游戏结束
        return;
    }

    // 更新所有方块位置
    for (int i = 0; i < block_count; i++)
    {
        if (blocks[i]) // 如果方块存在
        {
            int x = lv_obj_get_x(blocks[i]); // 获取当前x坐标
            int y = lv_obj_get_y(blocks[i]); // 获取当前y坐标
            y += BLOCK_SPEED;                // 计算新y坐标（下落）

            if (y > SCREEN_HEIGHT) // 如果方块超出屏幕底部
            {
                // 方块未被点击到达底部，游戏结束
                handle_wrong_click();
                return;
            }
            else
            {
                // 更新方块位置
                lv_obj_set_pos(blocks[i], x, y);
            }
        }
    }

    // 清理已隐藏的方块
    /*
        bool lv_obj_has_flag(const lv_obj_t * obj, lv_obj_flag_t f)
        LV_OBJ_FLAG_HIDDEN 检查对象是否被隐藏
    */
    int j = 0;
    for (int i = 0; i < block_count; i++)
    {
        if (blocks[i] && !lv_obj_has_flag(blocks[i], LV_OBJ_FLAG_HIDDEN))
        {
            // 方块未被隐藏则保留
            blocks[j++] = blocks[i];
        }
        else if (blocks[i])
        {
            // 方块已隐藏则删除
            lv_obj_del(blocks[i]);
        }
    }
    block_count = j; // 更新方块计数

    // 保持至少2个方块在屏幕上
    if (block_count < 2)
    {
        create_piano_block(); // 创建新方块
    }
}