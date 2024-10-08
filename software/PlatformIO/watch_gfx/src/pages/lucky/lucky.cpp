/*** 
 * @Author: MakerM0
 * @Date: 2024-09-11 13:47:30
 * @LastEditTime: 2024-10-03 19:44:44
 * @FilePath: \jdi_watch_gfx\src\pages\lucky\lucky.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */

#include "lucky.h"

#include "driver/drv_key.h"
#include "driver/drv_disp.h"

static uint8_t num = 0;
static bool flag_start = false;

static TaskHandle_t handle;

static void dispNum(void *para)
{
    char buffer[4];

    while (1)
    {
        if (flag_start == true)
        {

            num = random(10);
            snprintf(buffer, 4, "%d", num);
            disp.setFont();
            disp.setTextSize(5);
            disp.setCursor(27, 60);
            disp.setTextColor(COLOR_WHITE, COLOR_BLACK);
            disp.print(buffer);

            disp.refresh();
        }
        else
        {
        }

        delay(50);
    }
}

static void init(void *data)
{
    flag_start = false;
    randomSeed(analogRead(PIN_VBAT));
    // 创建定时获取任务
    BaseType_t res = xTaskCreate(dispNum, "lucky", SCHEDULER_STACK_SIZE_DFLT / 2, (void *)NULL, TASK_PRIO_LOW, &handle);
    if (res == pdPASS)
    {
    }
}

static void enter(void *data)
{
    init(NULL);
    disp.fillScreen(COLOR_BLACK);
    disp.fillRect(0, 0, DISP_WIDTH, 30, COLOR_GREEN); 
    disp.setFont();
    disp.setTextSize(2);
    disp.setCursor(5, 10);
    disp.setTextColor(COLOR_BLACK);
    disp.print("LUCKY");

    disp.setFont();
    disp.setTextSize(5);
    disp.setCursor(27, 60);
    disp.setTextColor(COLOR_WHITE, COLOR_BLACK);
    disp.print("?");

    disp.refresh();

    // manager_setBusy(false);
}

static void event()
{
    flag_start = !flag_start;
}

static void loop(void *data)
{
    KEY_TYPE key = app_key_get();
    switch (key)
    {
    case KEY0_SHORT:
        event();
        break;      // 计数开始
    case KEY0_LONG: // 长按
        // manager_switchToParent(); // 进入父项目 //退出
        page_exit();
        break;
    default:
        break;
    }
    // dispNum();
}

static void exit(void *data)
{
    vTaskDelete(handle);
    // manager_setBusy(true);
}
#include "img.h"
Page page_lucky = {
    .init = init,
    .enter = enter,
    .exit = exit,
    .loop = loop,
    .title_en = "Lucky",
    .title_cn = " 幸运数",
    .icon = img_bits,
    .icon_width = img_width,
    .icon_height = img_height,

};