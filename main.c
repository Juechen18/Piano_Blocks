#include "ui_manager.h"
#include "utils.h"
#include "game_logic.h"
#include <unistd.h>

int main()
{
    //  1、设备初始化
    lv_init();

    /*Linux frame buffer device init*/
    lv_display_t *disp = lv_linux_fbdev_create();
    lv_linux_fbdev_set_file(disp, "/dev/fb0");
    lv_indev_t *indev = lv_evdev_create(LV_INDEV_TYPE_POINTER, "/dev/input/event0");

    //  2、主要操作
    ui_create_all();
    show_menu_screen();

    //  3、检查事件链表是否有变化
    /*Handle LVGL tasks*/
    while (1)
    {
        lv_timer_handler();
        usleep(5000);
    }
}
