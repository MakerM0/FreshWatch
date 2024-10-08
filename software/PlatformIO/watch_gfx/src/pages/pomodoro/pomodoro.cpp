/*** 
 * @Author: MakerM0
 * @Date: 2024-10-03 15:22:28
 * @LastEditTime: 2024-10-03 20:33:28
 * @FilePath: \jdi_watch_gfx\src\pages\pomodoro\pomodoro.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */
#include "pomodoro.h"
#include "driver/drv_key.h"
#include "driver/drv_disp.h"
#include "driver/drv_power.h"
#include "driver/drv_neopixel.h"
#include "driver/drv_beep.h"
#include "fonts/fonts.h"

static bool flag_start = false;

static TaskHandle_t handle;

#define TIME_POMODORO (25 * 60)
#define TIME_BREAK (5 * 60)

TimerHandle_t xTimer_onesec, xTimer_mode;
static uint32_t cnt = 0;
typedef enum Mode
{
    POMODORO = 0,
    BREAK = 1,

} Mode;

static Mode mode;

static void colorstate(void *para)
{
    Mode mode = *(Mode *)(para);

    while (1)
    {

        // for (int i = 0; i < led.numPixels(); i++)
        { // For each pixel...

            // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
            // Here we're using a moderately bright green color:
            if (mode == BREAK)
                neopixel_setColor(pixels.Color(0, 255, 0));
            else
                neopixel_setColor(pixels.Color(17, 100, 100));

            delay(200); // Pause before next pass through loop
        }

        // beep_start();
        // delay(400);
        // beep_stop();
        delay(400);
        neopixel_clear();

        vTaskDelete(NULL);
    }
}

static void dispNum(uint32_t t, bool state, Mode mode)
{
    disp.fillScreen(COLOR_BLACK);

    disp.setFont();
    disp.setTextSize(1);
    disp.setCursor(0, 10);
    disp.setTextColor(COLOR_WHITE);
    switch (mode)
    {
    case POMODORO:
        disp.print("Podomoro");
        break;
    case BREAK:
        disp.print("Break");
        break;
    default:
        break;
    }

    disp.fillRoundRect(0, 30, DISP_WIDTH, 90, 4, COLOR_YELLOW);
    disp.setFont(&DigitTech7_Regular20pt7b);
    disp.setTextSize(1);
    disp.setCursor(10, 70);
    disp.setTextColor(COLOR_BLACK);
    char buffer[10];
    sprintf(buffer, "%02d", t / 60);
    disp.print(buffer);

    disp.setCursor(10, 110);
    disp.setTextColor(COLOR_MAGENTA);

    sprintf(buffer, "%02d", t % 60);
    disp.print(buffer);

    disp.setFont(&EDIX6pt7b);
    disp.setTextSize(1);

    if (state == false)
    {
        disp.setTextColor(COLOR_GREEN);
        disp.setCursor(2, 135);
        disp.print("START");
    }
    else
    {
        disp.setTextColor(COLOR_RED);
        disp.setCursor(8, 135);
        disp.print("STOP");
    }
    disp.refresh();
}

static void onesecCallback(TimerHandle_t xTimer)
{
    if (flag_start)
        cnt--;

    if (cnt == 0)
    {

        // 任务

        if (mode == POMODORO)
        {
            flag_start = false;
            cnt = TIME_BREAK;
            mode = BREAK;
        }
        else
        {
            flag_start = false;
            cnt = TIME_POMODORO;
            mode = POMODORO;
        }

        if (handle != NULL)
        {
            vTaskDelete(handle);
            handle = NULL;
            neopixel_clear();
        }
        BaseType_t res = xTaskCreate(colorstate, "color", SCHEDULER_STACK_SIZE_DFLT, &mode, TASK_PRIO_LOW, &handle);
        if (res == pdPASS)
        {
        }
    }

    dispNum(cnt, flag_start, mode);
}

static void init(void *data)
{
    randomSeed(analogRead(PIN_VBAT));
    // 创建定时获取任务
    xTimer_onesec = xTimerCreate("onesec", 1000, pdTRUE, (void *)0, onesecCallback);
    cnt = TIME_POMODORO;
    mode = POMODORO;
    flag_start = false;
}

static void enter(void *data)
{
    init(NULL);
    dispNum(TIME_POMODORO, false, POMODORO);

    disp.refresh();

    // manager_setBusy(false);
}

static void event()
{
    flag_start = !flag_start;
    if (flag_start)
    {
        xTimerStart(xTimer_onesec, 0);
    }
    else
    {
        xTimerStop(xTimer_onesec, 0);
    }
    dispNum(cnt, flag_start, mode);
}

static void loop(void *data)
{

    KEY_TYPE key = app_key_get();
    switch (key)
    {
    case KEY0_DOUBLE:
        if (mode == POMODORO)
        {
            cnt = TIME_BREAK;
            mode = BREAK;
        }
        else
        {
            cnt = TIME_POMODORO;
            mode = POMODORO;
        }

        dispNum(cnt, false, mode);
    case KEY0_SHORT:

        event();

        break;      // 计数开始
    case KEY0_LONG: // 长按
        page_exit();
        break;
    default:
        break;
    }
}

static void exit(void *data)
{
    xTimerStop(xTimer_onesec, 0);
    xTimerDelete(xTimer_onesec, 0);
    if (handle != NULL)
    {
        vTaskDelete(handle);
        handle = NULL;
    }

    neopixel_clear();
    app_key_startLoop();

    // manager_setBusy(true);
}
#include "img.h"
Page page_pomodoro = {
    .init = init,
    .enter = enter,
    .exit = exit,
    .loop = loop,
    .title_en = "Podomoro",
    .title_cn = "番茄钟",
    .icon = img_bits,
    .icon_width = img_width,
    .icon_height = img_height,

};