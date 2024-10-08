/***
 * @Author: MakerM0
 * @Date: 2024-10-04 13:43:33
 * @LastEditTime: 2024-10-04 13:45:55
 * @FilePath: \jdi_watch_gfx\src\pages\template\template.cpp
 * @Description:
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved.
 */

#include "template.h"

#include "driver/drv_key.h"
#include "driver/drv_disp.h"

static void init(void *data)
{
}

static void enter(void *data)
{
}

static void loop(void *data)
{
    KEY_TYPE key = app_key_get();
    switch (key)
    {
    case KEY0_SHORT:
        break;
    case KEY0_LONG: // 长按 退出
        page_exit();
        break;
    default:
        break;
    }
}

static void exit(void *data)
{
}
#include "img.h"
Page page_template = {
    .init = init,
    .enter = enter,
    .exit = exit,
    .loop = loop,
    .title_en = "page_template",
    .title_cn = " 模板",
    .icon = img_bits,
    .icon_width = img_width,
    .icon_height = img_height,

};