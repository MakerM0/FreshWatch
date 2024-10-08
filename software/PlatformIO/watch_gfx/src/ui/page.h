/*** 
 * @Author: MakerM0
 * @Date: 2024-09-09 16:09:37
 * @LastEditTime: 2024-10-03 20:26:57
 * @FilePath: \jdi_watch_gfx\src\ui\page.h
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */



#ifndef __MANAGER_H
#define __MANAGER_H
#include <stdint.h>

typedef void (*init_cb_t)(void *data);
typedef void (*enter_cb_t)(void *data);
typedef void (*exit_cb_t)(void *data);
typedef void (*loop_cb_t)(void *data);

typedef struct Page
{
    init_cb_t init; //初始化相关变量
    enter_cb_t enter;
    exit_cb_t exit;
    loop_cb_t loop;

    char *title_en;

    char *title_cn;

    const uint8_t *icon;
    uint16_t icon_width;
    uint16_t icon_height;

} Page;




#endif


