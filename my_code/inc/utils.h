/*工具函数声明*/
#ifndef UTILS_H
#define UTILS_H

#include "lvgl/lvgl.h"
#include "ui_manager.h"

// 播放音乐
void play_music(const char *path);

// 停止音乐
void stop_music();

// 隐藏所有界面
void hide_all_screens();

#endif