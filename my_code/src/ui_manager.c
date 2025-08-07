/*ui_manager.c*/
#include "ui_manager.h"    // UI管理头文件
#include "utils.h"         // 工具函数头文件
#include "game_logic.h"    // 游戏逻辑头文件
#include <stdio.h>         // 标准输入输出头文件
#include "lv_mygec_font.h" // 自定义字体头文件

// 每个页面的 lv_obj_t结构体
lv_obj_t *menu_screen = NULL;   // 显示主菜单的屏幕对象指针
lv_obj_t *select_screen = NULL; // 显示选择音乐界面的屏幕对象指针
lv_obj_t *game_screen = NULL;   // 显示游戏界面的屏幕对象指针
lv_obj_t *result_screen = NULL; // 显示结算页面的屏幕对象指针

// 标签对象指针
lv_obj_t *score_label = NULL;  // 游戏界面显示分数的标签
lv_obj_t *result_label = NULL; // 结果界面显示结果的标签

// UI创建函数：初始化所有界面元素
void ui_create_all()
{
    // 样式定义部分

    // 1. 定义文本样式（使用自定义字体）
    static lv_style_t text_style;
    lv_style_init(&text_style);                          // 初始化样式
    lv_style_set_text_font(&text_style, &lv_mygec_font); // 设置自定义字体

    // 2. 定义标题样式
    static lv_style_t title_style;
    lv_style_init(&title_style);                                   // 初始化样式
    lv_style_set_text_font(&title_style, &lv_mygec_font);          // 设置自定义字体
    lv_style_set_text_color(&title_style, lv_color_hex(0x000000)); // 设置文本颜色为黑色
    lv_style_set_text_opa(&title_style, 255);                      // 设置完全不透明
    lv_style_set_text_letter_space(&title_style, 2);               // 字间距

    // ========== 主菜单页面创建 ==========
    menu_screen = lv_obj_create(lv_screen_active()); // 创建主菜单屏幕
    lv_obj_set_size(menu_screen, 800, 480);          // 设置屏幕尺寸800x480

    // 1. 添加主菜单标题"钢琴块"
    lv_obj_t *title_label = lv_label_create(menu_screen);          // 创建标签
    lv_label_set_text(title_label, "钢琴块");                      // 设置标签文本
    lv_obj_add_style(title_label, &title_style, LV_STATE_DEFAULT); // 应用标题样式
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);            // 对齐到顶部中间，向下偏移20像素

    // 2. 创建"选择音乐"按钮
    lv_obj_t *btn_music = lv_button_create(menu_screen);     // 创建按钮
    lv_obj_set_size(btn_music, 200, 80);                     // 设置按钮尺寸200x80
    lv_obj_align(btn_music, LV_ALIGN_CENTER, 0, -50);        // 居中并向上偏移50像素
    lv_obj_t *label1 = lv_label_create(btn_music);           // 创建按钮标签
    lv_label_set_text(label1, "选择音乐");                   // 设置标签文本
    lv_obj_add_style(label1, &text_style, LV_STATE_DEFAULT); // 应用文本样式
    // 添加点击事件回调函数，点击时显示选择音乐界面
    lv_obj_add_event_cb(btn_music, show_select_screen, LV_EVENT_CLICKED, NULL);

    // 3. 创建"退出程序"按钮
    lv_obj_t *btn_other = lv_button_create(menu_screen);     // 创建按钮
    lv_obj_set_size(btn_other, 200, 80);                     // 设置按钮尺寸200x80
    lv_obj_align(btn_other, LV_ALIGN_CENTER, 0, 50);         // 居中并向下偏移50像素
    lv_obj_t *label2 = lv_label_create(btn_other);           // 创建按钮标签
    lv_label_set_text(label2, "退出程序");                   // 设置标签文本
    lv_obj_add_style(label2, &text_style, LV_STATE_DEFAULT); // 应用文本样式
    // 添加点击事件回调函数，点击时退出程序
    lv_obj_add_event_cb(btn_other, exit_program_cb, LV_EVENT_CLICKED, NULL);

    // ========== 音乐选择页面创建 ==========
    select_screen = lv_obj_create(lv_screen_active()); // 创建选择音乐屏幕
    lv_obj_set_size(select_screen, 800, 480);          // 设置屏幕尺寸800x480

    // 音乐选择按钮配置
    const char *music_names[] = {"音乐1", "音乐2", "返回"}; // 按钮文本数组
    // 按钮回调函数数组
    void (*callbacks[])(lv_event_t *) = {game_start_music1_cb, game_start_music2_cb, show_menu_screen};

    // 创建三个音乐选择按钮
    for (int i = 0; i < 3; i++)
    {
        lv_obj_t *btn = lv_button_create(select_screen);                // 创建按钮
        lv_obj_set_size(btn, 200, 60);                                  // 设置按钮尺寸200x60
        lv_obj_align(btn, LV_ALIGN_CENTER, 0, -100 + i * 100);          // 垂直排列，间隔100像素
        lv_obj_t *label = lv_label_create(btn);                         // 创建按钮标签
        lv_label_set_text(label, music_names[i]);                       // 设置标签文本
        lv_obj_add_style(label, &text_style, LV_STATE_DEFAULT);         // 应用文本样式
        lv_obj_add_event_cb(btn, callbacks[i], LV_EVENT_CLICKED, NULL); // 添加点击事件回调
    }

    // ========== 游戏页面创建 ==========
    game_screen = lv_obj_create(lv_screen_active()); // 创建游戏屏幕
    lv_obj_set_size(game_screen, 800, 480);          // 设置屏幕尺寸800x480

    // 分数标签
    score_label = lv_label_create(game_screen);                   // 创建分数标签
    lv_label_set_text(score_label, "分数为0");                    // 初始化分数文本
    lv_obj_add_style(score_label, &text_style, LV_STATE_DEFAULT); // 应用文本样式
    lv_obj_align(score_label, LV_ALIGN_TOP_LEFT, 10, 10);         // 对齐到左上角，偏移(10,10)

    // 创建钢琴块（调用游戏逻辑中的函数）
    create_piano_block();

    // ========== 结果页面创建 ==========
    result_screen = lv_obj_create(lv_screen_active()); // 创建结果屏幕
    lv_obj_set_size(result_screen, 800, 480);          // 设置屏幕尺寸800x480

    // 结果文字标签
    result_label = lv_label_create(result_screen);                 // 创建结果标签
    lv_label_set_text(result_label, "游戏结束\n得分为0");          // 初始化结果文本
    lv_obj_add_style(result_label, &text_style, LV_STATE_DEFAULT); // 应用文本样式
    lv_obj_align(result_label, LV_ALIGN_CENTER, 0, -50);           // 居中并向上偏移50像素

    // 返回按钮
    lv_obj_t *btn_back = lv_button_create(result_screen);        // 创建返回按钮
    lv_obj_set_size(btn_back, 150, 60);                          // 设置按钮尺寸150x60
    lv_obj_align(btn_back, LV_ALIGN_CENTER, 0, 50);              // 居中并向下偏移50像素
    lv_obj_t *label_back = lv_label_create(btn_back);            // 创建按钮标签
    lv_label_set_text(label_back, "返回菜单");                   // 设置标签文本
    lv_obj_add_style(label_back, &text_style, LV_STATE_DEFAULT); // 应用文本样式
    // 添加点击事件回调函数，点击时返回主菜单
    lv_obj_add_event_cb(btn_back, show_menu_screen, LV_EVENT_CLICKED, NULL);

    // 初始化隐藏所有页面
    hide_all_screens();
}

// ========== 页面切换函数 ==========

// 显示主菜单界面
void show_menu_screen()
{
    hide_all_screens();                                  // 先隐藏所有界面
    lv_obj_remove_flag(menu_screen, LV_OBJ_FLAG_HIDDEN); // 显示主菜单
}

// 显示音乐选择界面
void show_select_screen()
{
    hide_all_screens();                                    // 先隐藏所有界面
    lv_obj_remove_flag(select_screen, LV_OBJ_FLAG_HIDDEN); // 显示音乐选择界面
}

// 显示游戏界面
void show_game_screen()
{
    hide_all_screens();                                  // 先隐藏所有界面
    lv_obj_remove_flag(game_screen, LV_OBJ_FLAG_HIDDEN); // 显示游戏界面
}

// 显示结果界面
void show_result_screen(int score)
{
    // 临时设置结果页面的文本样式
    static lv_style_t text_style;
    lv_style_init(&text_style);                          // 初始化样式
    lv_style_set_text_font(&text_style, &lv_mygec_font); // 设置自定义字体

    hide_all_screens(); // 先隐藏所有界面
    // 动态设置结果文本，显示传入的分数
    lv_label_set_text_fmt(result_label, "游戏结束\n得分为%d", score);
    lv_obj_add_style(result_label, &text_style, LV_STATE_DEFAULT); // 应用文本样式
    lv_obj_remove_flag(result_screen, LV_OBJ_FLAG_HIDDEN);         // 显示结果界面
}

/**
 * 退出程序的回调函数（处理"退出程序"按钮点击事件）
 * @param event LVGL事件对象（包含触发事件的控件信息）
 */
void exit_program_cb(lv_event_t *event)
{
    // 1. 停止当前播放的音乐（避免程序退出后音乐继续）
    stop_music();

    // 2. 销毁所有LVGL UI资源（递归销毁根屏幕下的所有对象）
    // LVGL中，销毁根屏幕对象（menu_screen/select_screen等）会自动销毁其所有子对象（按钮、标签、钢琴块等）
    if (menu_screen != NULL)
    {
        lv_obj_delete(menu_screen); // 销毁主菜单屏幕（含子对象）
        menu_screen = NULL;         // 置空避免野指针
    }
    if (select_screen != NULL)
    {
        lv_obj_delete(select_screen); // 销毁音乐选择屏幕
        select_screen = NULL;
    }
    if (game_screen != NULL)
    {
        lv_obj_delete(game_screen); // 销毁游戏屏幕（含钢琴块）
        game_screen = NULL;
    }
    if (result_screen != NULL)
    {
        lv_obj_delete(result_screen); // 销毁结果屏幕
        result_screen = NULL;
    }

    // 3. 释放LVGL全局资源
    lv_deinit(); // 关键：释放LVGL分配的内存、线程等资源

    // 4. 彻底终止程序（0表示成功，非0表示错误）
    exit(0);
}