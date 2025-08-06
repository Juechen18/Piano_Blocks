/*UI页面声明*/
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "lvgl/lvgl.h"

// 每个页面的 lv_obj_t结构体
extern lv_obj_t *menu_screen;   // 显示主菜单
extern lv_obj_t *select_screen; // 显示选择音乐界面
extern lv_obj_t *game_screen;   // 显示游戏界面
extern lv_obj_t *result_screen; // 显示结算页面

extern lv_obj_t *score_label;
extern lv_obj_t *result_label;

void ui_create_all();               // 创建所有页面
void show_menu_screen();            // 显示主菜单
void show_select_screen();          // 显示选择音乐界面
void show_game_screen();            // 显示游戏界面
void show_result_screen(int score); // 显示结算页面

#endif
