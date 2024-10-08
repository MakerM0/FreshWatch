/*** 
 * @Author: MakerM0
 * @Date: 2024-09-22 18:24:59
 * @LastEditTime: 2024-10-03 19:45:54
 * @FilePath: \jdi_watch_gfx\src\pages\enterUF2\enterUF2.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */
#include "enterUF2.h"

#include "driver/drv_key.h"
#include "driver/drv_disp.h"
#include "driver/drv_power.h"

static void init(void *data)
{
    // flag_start = false;
    // randomSeed(analogRead(PIN_VBAT));
    // // 创建定时获取任务
    // BaseType_t res = xTaskCreate(dispNum, "lucky", SCHEDULER_STACK_SIZE_DFLT/2, (void *)NULL, TASK_PRIO_LOW, &handle);
    // if (res == pdPASS)
    // {
    // }
}

static void enter(void *data)
{
    init(NULL);
    disp.fillScreen(COLOR_BLACK);
    disp.setFont();
    disp.setTextSize(3);
    disp.setCursor(10, DISP_HEIGHT / 2);
    disp.setTextColor(COLOR_WHITE);
    disp.print("UF2");
    disp.refresh();
    delay(500);

    enterUf2Dfu();

    // manager_setBusy(false);
}

static void loop(void *data)
{
    KEY_TYPE key = app_key_get();
    switch (key)
    {
    case KEY0_SHORT:

        break;      // 计数开始
    case KEY0_LONG: // 长按

 
        break;
    default:
        break;
    }
    // dispNum();
}

static void exit(void *data)
{

    // manager_setBusy(true);
}
#include "img.h"
Page page_enteruf2 = {
    .init = init,
    .enter = enter,
    .exit = exit,
    .loop = loop,
    .title_en = "uf2",
    .title_cn = " uf2",
    .icon = img_bits,
    .icon_width = img_width,
    .icon_height = img_height,

};