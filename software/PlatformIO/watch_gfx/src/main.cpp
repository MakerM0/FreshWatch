/***
 * @Author: MakerM0
 * @Date: 2024-09-05 15:20:36
 * @LastEditTime: 2024-10-03 20:28:54
 * @FilePath: \jdi_watch_gfx\src\main.cpp
 * @Description:
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved.
 */

#include <Arduino.h>
// #include <Adafruit_TinyUSB.h> // for Serial
#include <bluefruit.h>

#include "driver/drv_sensor.h"
#include "driver/drv_time.h"
#include "driver/drv_ble.h"
#include "driver/drv_disp.h"
#include "driver/drv_neopixel.h"
#include "driver/drv_beep.h"
#include "driver/drv_battery.h"
#include "driver/drv_key.h"

// #include <Adafruit_LittleFS.h>
// #include <InternalFileSystem.h>

#include "ui/ui.h"
#include "fonts/fonts.h"

// void flash_init()
// {

//     InternalFS.begin();
// }

// 对于weak函数getAction的重新定义
action getAction()
{
    action ac = ACTION_NONE;
    KEY_TYPE key = app_key_get();
    switch (key)
    {
    case KEY0_SHORT:
        ac = ACTION_DOWN;
        break;
    case KEY0_DOUBLE:
        ac = ACTION_UP;
        break;
    case KEY0_LONG:
        ac = ACTION_ENTER;
        break;
    default:
        break;
    }
    return ac;
}

static void date_init()
{
    const char *compile_date = __DATE__;
    const char *compile_time = __TIME__;

    // 解析日期
    char month_str[4];
    int day, year;
    sscanf(compile_date, "%s %d %d", month_str, &day, &year);

    // 将月份转换为数字
    const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int month = 0;
    for (int i = 0; i < 12; i++)
    {
        if (strcmp(month_str, months[i]) == 0)
        {
            month = i + 1;
            break;
        }
    }
    // 解析时间
    int hour, minute, second;
    sscanf(compile_time, "%d:%d:%d", &hour, &minute, &second);

    time_set(year, month, day, hour, minute, second);
}

BaseType_t res;

TaskHandle_t env_handle;

static void env_task(void *para)
{

    while (1)
    {
        sensor.updateEnv();
        delay(1000); // wait for a half second
    }
}

TaskHandle_t alart_handle;
static void alart_task(void *para)
{

    while (1)
    {
        if (sensor.getVocIndex() > 300)
        {

            neopixel_setColor(0xff0000);
            delay(50);
            neopixel_powerOff();
            delay(1000);
        }
        else if (sensor.getVocIndex() > 200)
        {

            neopixel_setColor(0xffa500);
            delay(50);
            neopixel_powerOff();
            delay(4000);
        }
        else
        {
            neopixel_powerOff();
            delay(20000);
        }
    }
}

TaskHandle_t motion_handle;
static void motion_task(void *para)
{

    while (1)
    {
        sensor.updateSteps();
        delay(5000);
    }
}
static TaskHandle_t battery_handle;
static void battery_task(void *para)
{
    uint32_t cnt = 0;
    while (1)
    {
        battery_mv = battery_readmv();
        delay(15000);
    }
}

static void enterUF2()
{
    pinMode(PIN_BUTTON1, INPUT_PULLUP);

    if (digitalRead(PIN_BUTTON1) == 0)
    {
        // 进入ota模式
        disp.fillScreen(COLOR_BLACK);
        disp.setFont();
        disp.setTextSize(3);
        disp.setCursor(10, DISP_HEIGHT / 2);
        disp.setTextColor(COLOR_WHITE);
        disp.print("UF2");
        disp.refresh();
        delay(1000);
        enterUf2Dfu();
    }
}
const char *welcome_str = "MakerM0";
 
static void welcome()
{
    for (uint8_t i = 10; i < 36; i++)
    {
        disp.fillScreen(COLOR_BLACK);

        disp.setFont(&EDIX8pt7b);
        disp.setTextSize(1);
        for (uint8_t j = 0; j < 7; j++)
        {
            disp.setCursor(i, j * 15 + 30);
            disp.setTextColor(colors[j]);
            disp.println(welcome_str[j]);
        }

        disp.fillRect(36, 0, 36, DISP_HEIGHT, COLOR_BLACK);
        disp.drawFastVLine(36, 10, DISP_HEIGHT - 20, COLOR_WHITE);

        disp.refresh();
        delay(1);
    }

    for (uint8_t i = 0; i < 62; i++)
    {
        disp.drawBufferedPixel(36, 10 + i, COLOR_BLACK);

        disp.drawBufferedPixel(36, DISP_HEIGHT - 10 - i, COLOR_BLACK);

        disp.refresh();
        delay(1);
    }
 
}

void setup()
{

    ble_setup();
    neopixel_setup();
    disp_setup();
    welcome();
    battery_setup();
    battery_mv = battery_readmv();
    beep_setup();
    beep_start();
    delay(100);
    beep_stop();
    time_setup();
    // enterUF2();

    app_key_init();
    // key2_init();

    sensor.begin(-2.0);

    res = xTaskCreate(env_task, "env", SCHEDULER_STACK_SIZE_DFLT, (void *)NULL, TASK_PRIO_LOW, &env_handle);
    res = xTaskCreate(motion_task, "motion", SCHEDULER_STACK_SIZE_DFLT, (void *)NULL, TASK_PRIO_LOW, &motion_handle);
    res = xTaskCreate(battery_task, "battery", SCHEDULER_STACK_SIZE_DFLT, (void *)NULL, TASK_PRIO_LOW, &battery_handle);
    res = xTaskCreate(alart_task, "alart", SCHEDULER_STACK_SIZE_DFLT, (void *)NULL, TASK_PRIO_LOW, &alart_handle);

    // if ((readResetReason() & (1 << 16)) != 0)
    // {
    //     time_set(2024, 9, 12, 21, 27, 50);
    // }

    // 固件编译时间
    date_init();

    ui_init();

    // clear reset reson
    if (NRF_POWER->RESETREAS & POWER_RESETREAS_RESETPIN_Msk)
    {
        NRF_POWER->RESETREAS = ~POWER_RESETREAS_RESETPIN_Msk;
    }

    NRF_USBD->ENABLE = 0; // 可降低1mA以上的电流

    // suspendLoop();
    // vTaskSuspend(env_handle);
    // vTaskSuspend(motion_handle);
}

void loop()
{
    menu_loop();
    delay(10);
}
