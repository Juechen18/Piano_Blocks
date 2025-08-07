/*utils.c*/
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// 播放音乐
void play_music(const char *path)
{
    // 先停止当前播放的音乐
    stop_music();

    // 获取当进程前工作路径
    char *ret;
    char buf[128];
    ret = getcwd(buf, sizeof(buf));
    if (ret == NULL)
    {
        perror("getcwd() error");
        return;
    }
    char command[200];
    snprintf(command, sizeof(command), "madplay -a -55 -r %s/assets/music/%s &", ret, path);

    // 执行命令
    int result = system(command);
}

void stop_music()
{
    system("killall -KILL madplay"); // 停止正在播放的音频
}

// 隐藏所有界面
void hide_all_screens()
{
    lv_obj_add_flag(menu_screen, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(select_screen, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(game_screen, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(result_screen, LV_OBJ_FLAG_HIDDEN);
}