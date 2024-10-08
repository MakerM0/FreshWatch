/*** 
 * @Author: MakerM0
 * @Date: 2024-09-11 16:51:55
 * @LastEditTime: 2024-10-03 23:00:54
 * @FilePath: \jdi_watch_gfx\src\pages\home\home.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */
 
#include "home.h"

#include "../settings/settings.h"

#include "driver/drv_key.h"
#include "driver/drv_disp.h"
#include "driver/drv_battery.h"
#include "driver/drv_time.h"
#include "driver/drv_sensor.h"
#include "driver/drv_beep.h"

#include "fonts/fonts.h"

#include "ui/tool.h"

#define FPS 1

#define FPS_DELAY (1000 / FPS)

uint32_t fps_delay;

uint32_t lastFpsMillis_fps = 0;
const uint8_t week_reamp[] = {6, 0, 1, 2, 3, 4, 5};

// uint32_t steps[7] = {100,300,400,5000,2500,3000,200};
uint32_t steps[7] = {0};

uint32_t step_goal = 4000;
uint32_t step_cur;
int week_cur = 0;

typedef struct air_quality_t
{
    int threshold;
    int color_bg;
    char name[10];
    int color;

} air_quality_t;

const air_quality_t air_quality[] =
    {
        {50, COLOR_GREEN, "EXCELLENT", COLOR_BLACK},
        {100, COLOR_YELLOW, "GOOD", COLOR_BLACK},
        {150, COLOR_CYAN, "LIGHT", COLOR_BLACK},
        {200, COLOR_BLUE, "MODERATE", COLOR_WHITE},
        {300, COLOR_MAGENTA, "HEAVY", COLOR_WHITE},
        {500, COLOR_RED, "SEVERE", COLOR_WHITE}};

static void ui_step_update(int32_t x, int32_t y)
{

    int16_t x1;
    int16_t y1;
    uint16_t w;
    uint16_t h;
    step_cur = sensor.getSteps();

    disp.fillScreen(COLOR_BLACK);

    disp.setFont();
    disp.setTextSize(2);
    disp.setTextColor(COLOR_WHITE);
    disp.getTextBounds("STEP", 0, 0, &x1, &y1, &w, &h);
    disp.setCursor(x - x1 + (72 - w) / 2, y - y1 + 0);
    disp.print("STEP");

    disp.fillRoundRect(x + 2, y + 22, 68, 30, 3, COLOR_YELLOW);
    disp.drawRoundRect(x + 2, y + 22, 68, 30, 3, COLOR_WHITE);

    disp.setFont(&DigitTech7_Regular8pt7b);
    disp.setTextSize(1);
    disp.setTextColor(COLOR_BLACK);
    disp.getTextBounds("00000", 0, 0, &x1, &y1, &w, &h);
    disp.setCursor(x - x1 + (72 - w) / 2, y - y1 + 30);
    disp.printf("%5d", step_cur);

    disp.setFont();
    disp.setTextSize(1);
    disp.setTextColor(COLOR_YELLOW);
    disp.getTextBounds("TARGET:40000", 0, 0, &x1, &y1, &w, &h);
    disp.setCursor(x - x1 + (72 - w) / 2, y - y1 + 60);
    disp.printf("TARGET:%5d", step_goal);

    disp.drawFastHLine(x + 1, y + 130, 72, COLOR_YELLOW);

    uint16_t color;
    const uint32_t height = 55;

    for (uint8_t i = 0; i < 7; i++)
    {
        // steps[i] = random(height);
        if (week_reamp[week_cur] == i)
        {
            color = COLOR_GREEN;
            steps[i] = step_cur;
        }
        else
        {
            color = COLOR_YELLOW;
        }
        disp.drawRect(x + 3 + 10 * i, y + 130 - height, 6, height, COLOR_BLUE);

        uint16_t height_step = map(steps[i], 0, step_goal, 0, height);
        // if (height_step>height)
        // {
        //     height_step = height;
        // }
        height_step = constrain(height_step, 0, height);
        disp.fillRect(x + 3 + 10 * i, y + 130 - (height_step), 6, height_step, color);

        disp.setFont();
        disp.setTextSize(1);

        if (week_reamp[week_cur] == i)
        {
            disp.setTextColor(COLOR_BLACK, color);
        }
        else
        {
            disp.setTextColor(color);
        }

        disp.getTextBounds("0", 0, 0, &x1, &y1, &w, &h);
        disp.setCursor(x - x1 + 4 + 10 * i, y - y1 + 135);
        disp.printf("%d", i + 1);
    }

    disp.refresh(); // transfer internal memory to the display
}

// x position, y position and size of metaballs
int metaball_xpos[3];
int metaball_ypos[3];
int metaball_size[3];
float pixel_value; // calculated pixel value

float time_anim = 0.0; // time variable controls all the movements

static void ui_batt_update(int32_t x, int32_t y)
{
    uint8_t percent = battery_mvToPercent(battery_mv);

    uint8_t bat_color = 0;
    if (percent < 20)
    {
        bat_color = COLOR_RED;
    }
    else if (percent < 50)
    {
        bat_color = COLOR_YELLOW;
    }
    else
    {
        bat_color = COLOR_GREEN;
    }

    int width = 40;
    int height = 60;
    int x_start = 16;
    int y_start = 30;
    time_anim = time_anim + 0.05; // increase the time animation

    // set a new position for metaballs
    for (int i = 0; i < 3; i++)
    {
        metaball_xpos[i] = sin(time_anim * (i + 1.4)) * 10 + width / 2 + x_start + x;
        metaball_ypos[i] = cos(time_anim * (i + 2.6) + 0.9) * 26 + height / 2 + y_start + y;
        metaball_size[i] = 25; // constant size
    }

    disp.fillScreen(COLOR_BLACK);
    // disp.setBitmapMode(1); // draw transparent images
    // disp.drawXBitmap(0, 0, epd_bitmap_battery_outline, 70, 40, COLOR_WHITE); // draw the image of the outline of the battery icon
    // set color to white color
    disp.fillRoundRect(x + x_start + width / 4, y + y_start - 5, width / 2, 6, 1, COLOR_WHITE);
    disp.drawRoundRect(x + x_start, y + y_start, width, height, 3, COLOR_WHITE);

    // calculate pixel value for metaballs
    for (int xpos = x_start + 3; xpos < x_start + width - 3; xpos++)
    {
        // for (int ypos = y_start + 3; ypos < y_start + height-3; ypos++)
        for (int ypos = y_start + 3; ypos < y_start + 2 + map(percent % 101, 0, 100, height - 4, 0); ypos++)

        {

            pixel_value = metaball_size[0] / sqrt(pow(xpos - metaball_xpos[0], 2) + pow(ypos - metaball_ypos[0], 2)) +
                          metaball_size[1] / sqrt(pow(xpos - metaball_xpos[1], 2) + pow(ypos - metaball_ypos[1], 2)) +
                          metaball_size[2] / sqrt(pow(xpos - metaball_xpos[2], 2) + pow(ypos - metaball_ypos[2], 2));

            // draw white pixel based on some threshold value
            if (pixel_value > 6)
            {

                disp.drawBufferedPixel(x + xpos, y + ypos, bat_color);
            }
        }
    }

    disp.fillRoundRect(x + x_start + 2, y + y_start + 2 + map(percent % 101, 0, 100, height - 4, 0), width - 4, map(percent % 101, 0, 100, 0, height - 4), 2, bat_color);

    // disp.setFont();
    // disp.setTextSize(1);
    // disp.setCursor(x + 10, y + 0);
    // disp.printf("%d", battery_getChargeState());

    disp.setFont();
    disp.setTextSize(2);
    // disp.getTextBounds(week[time_now->tm_wday], 0, 0, &x1, &y1, &w, &h);
    disp.setTextColor(bat_color);
    disp.setCursor(x + 18, y + 100);
    disp.printf("%d%%", percent);

    disp.setFont();
    disp.setTextSize(1);
    disp.setTextColor(COLOR_MAGENTA);
    disp.setCursor(x + 20, y + 130);
    disp.printf("%1.2fV", battery_mv / 1000.0);

    // percent++;
    disp.refresh(); // transfer internal memory to the display
}

bool flag_clearStep = false;

static void ui_time_update(int32_t xpos, int32_t ypos)
{
    int16_t x1 = 0;
    int16_t y1 = 0;
    uint16_t w;
    uint16_t h;

    struct tm *time_now = time_get();

    disp.fillScreen(COLOR_BLACK);
    disp.setTextWrap(false);

    disp.setFont();
    disp.setTextSize(1);
    disp.setTextColor(COLOR_WHITE);
    // disp.getTextBounds("00", 0, 0, &x1, &y1, &w, &h);
    disp.setCursor(xpos + 0 - x1 + 0, ypos + 0 - y1);
    // disp.printf("%04d-%02d-%02d", time_now->tm_year + 1900, time_now->tm_mon + 1, time_now->tm_mday);
    disp.printf("%02d-%02d", time_now->tm_mon + 1, time_now->tm_mday);
    week_cur = time_now->tm_wday;

    if (time_now->tm_hour == 0 && time_now->tm_min == 0 && time_now->tm_sec == 0)
    {
        if (flag_clearStep == false)
        {
            week_cur = time_now->tm_wday;

            // 保存上一个日期的
            if (week_cur > 0)
            {
                steps[week_cur - 1] = sensor.getSteps();
            }
            else
            {
                // 重置一周的数据
                memset(steps, 0, 7);
            }

            sensor.resetSteps();
            flag_clearStep = true;
        }
    }
    else
    {
        flag_clearStep = false;
    }

    disp.setFont(&DigitTech7_Regular24pt7b);

    disp.setTextSize(1);
    // uint8_t height = DigitTech7_Regular24pt7b.glyph->height;
    // uint8_t yoff = DigitTech7_Regular24pt7b.glyph->yOffset;

    disp.getTextBounds("00", 0, 0, &x1, &y1, &w, &h);

    // disp.setCursor(xpos + 0 - x1 + ((72 - w) / 2), ypos + 0 - y1 + (72 - h) - 5);
    // disp.setTextColor(COLOR_BLUE);
    // disp.print("88");
    disp.setCursor(xpos + 0 - x1 + ((72 - w) / 2), ypos + 0 - y1 + (72 - h) - 5);
    disp.setTextColor(COLOR_CYAN);
    disp.printf("%02d", time_now->tm_hour);

    // disp.setCursor(xpos + 0 - x1 + ((72 - w) / 2), ypos + 72 - y1 + 5);
    // disp.setTextColor(COLOR_BLUE);
    // disp.print("88");
    disp.setCursor(xpos + 0 - x1 + ((72 - w) / 2), ypos + 72 - y1 + 5);
    disp.setTextColor(COLOR_MAGENTA);
    disp.printf("%02d", time_now->tm_min);

    disp.setFont();
    disp.setTextSize(1);
    disp.setTextColor(COLOR_YELLOW);
    disp.setCursor(xpos + 60, ypos + 135);
    disp.printf("%02d", time_now->tm_sec);

    disp.setFont(&EDIX6pt7b);
    disp.setTextSize(1);
    disp.setTextColor(COLOR_YELLOW);
    disp.getTextBounds(week[time_now->tm_wday], 0, 0, &x1, &y1, &w, &h);
    disp.setCursor(xpos + 0 - x1 + ((72 - w) / 2), ypos + 144 - y1 - h-3);
    disp.printf("%s", week[time_now->tm_wday]);

    disp.setFont();
    disp.setTextSize(1);
    disp.setTextColor(COLOR_YELLOW);
    // disp.getTextBounds(week[time_now->tm_wday], 0, 0, &x1, &y1, &w, &h);
    disp.setCursor(xpos + 48, ypos + 0);
    // disp.printf("%.2f", battery_mv / 1000.0);
    disp.printf("%3d%%", battery_mvToPercent(battery_mv));

    disp.refresh(); // Actually updates the display
}

// GFXcanvas1 canvas(128, 32); // 128x32 pixel canvas
static void ui_env_update(int32_t xpos, int32_t ypos)
{

    int16_t x1;
    int16_t y1;
    uint16_t w;
    uint16_t h;
    int voc = sensor.getVocIndex();
    disp.fillScreen(COLOR_BLACK);
    disp.setTextWrap(false);

    disp.setTextSize(1);

    disp.setTextColor(COLOR_WHITE);
    disp.setFont(&DigitTech7_Regular16pt7b);
    disp.setCursor(xpos + 0, ypos + 30);
    disp.printf("%3d", voc);

    disp.setTextColor(COLOR_WHITE);
    disp.setFont();
    disp.setTextSize(1);
    disp.setCursor(xpos + 66, ypos + 7);
    disp.print("V");
    disp.setCursor(xpos + 66, ypos + 15);
    disp.print("O");
    disp.setCursor(xpos + 66, ypos + 23);
    disp.print("C");

    disp.setFont();
    disp.setTextSize(1);
    disp.setTextColor(COLOR_WHITE);
    disp.getTextBounds("AIR QUALITY", 0, 0, &x1, &y1, &w, &h);
    disp.setCursor(xpos + 0 - x1 + 36 - (w / 2), ypos + 43 - y1 + ((20 - h) / 2));
    disp.print("AIR QUALITY");

    uint8_t air_index = 0;
    if (voc <= air_quality[0].threshold)
    {
        air_index = 0;
    }
    else if (voc <= air_quality[1].threshold)
    {
        air_index = 1;
    }
    else if (voc <= air_quality[2].threshold)
    {
        air_index = 2;
    }
    else if (voc <= air_quality[3].threshold)
    {
        air_index = 3;
    }
    else if (voc <= air_quality[4].threshold)
    {
        air_index = 4;
    }
    else
    {
        air_index = 5;
    }

    disp.fillRoundRect(xpos + 0, 60, 72, 20, 4, air_quality[air_index].color_bg);
    disp.drawRoundRect(xpos + 0, 60, 72, 20, 4, COLOR_WHITE);
    disp.setFont();
    disp.setTextSize(1);
    disp.setTextColor(air_quality[air_index].color);
    disp.getTextBounds(air_quality[air_index].name, 0, 0, &x1, &y1, &w, &h);
    disp.setCursor(xpos + 0 - x1 + 36 - (w / 2), ypos + 60 - y1 + ((20 - h) / 2));
    disp.print(air_quality[air_index].name);

    // temp
    disp.setFont();
    disp.setTextSize(1);
    disp.setTextColor(COLOR_WHITE);
    disp.setCursor(xpos + 10, ypos + 95);
    disp.print("Temp");

    disp.setCursor(xpos + 36 + 10, ypos + 95);
    disp.print("Humi");

    float temp = sensor.getTemperature();
    float humi = sensor.getHumidity();
    disp.setTextColor(COLOR_WHITE);
    disp.setFont(&DigitTech7_Regular12pt7b);
    disp.setCursor(xpos + 5, ypos + 125);
    disp.printf("%2d", int(temp));

    disp.setFont(&DigitTech7_Regular12pt7b);
    disp.setCursor(xpos + 5 + 36, ypos + 125);
    disp.printf("%2d", int(humi));

    disp.setFont();
    disp.setTextSize(1);
    disp.setTextColor(COLOR_WHITE);

    disp.setCursor(xpos + 36 - 16, ypos + 130);
    disp.printf("%dC", int((temp - int(temp)) * 10));

    disp.setCursor(xpos + 72 - 16, ypos + 130);
    disp.printf("%d%%", int((humi - int(humi)) * 10));

    disp.refresh();
}

#include <Easing.h>
// typedef void (*ui_update)(int,int);
static void easyout(void (*ui_update)(int32_t, int32_t))
{
    EasingFunc<Ease::Back> ci;
    float duration = 300.; // ms
    float scale = 65.0;    // position
    float start_sec;
    int32_t x;

    ci.duration(duration);
    ci.scale(scale);
    start_sec = millis() / 1.0;

    while (1)
    {

        float now = millis() / 1.0;
        if (now <= start_sec + duration)
        {
            x = int(ci.get(now - start_sec));
            ui_update(x, 0);
            yield();
        }
        else
        {
            break;
        }
    }
}

static void easyinout(void (*ui_out)(int32_t, int32_t), void (*ui_in)(int32_t, int32_t))
{
    EasingFunc<Ease::Back> ci;
    float duration = 300.; // ms
    float scale = 65.0;    // position
    float start_sec;
    int32_t x;

    ci.duration(duration);
    ci.scale(scale);
    start_sec = millis() / 1.0;

    while (1)
    {

        float now = millis() / 1.0;
        if (now <= start_sec + duration)
        {
            x = int(ci.get(now - start_sec));
            ui_out(x, 0);
            ui_in(70 - x, 0);
            yield();
        }
        else
        {
            break;
        }
    }
}

void (*ui_update[])(int32_t, int32_t) = {ui_time_update, ui_env_update, ui_step_update};

uint8_t PAGE_NUM = ARRAY_SIZE(ui_update);
uint8_t page_id = 0;

static void switchPage()
{
    easyout(ui_update[page_id]);
    // easyinout(ui_update[page_id],ui_update[(page_id+1)%PAGE_NUM]);
    page_id++;
    page_id = page_id % PAGE_NUM;

    ui_update[page_id](0, 0);
}

extern TaskHandle_t env_handle;
extern TaskHandle_t motion_handle;

static void enter(void *data)
{
    step_goal = para[PARA_STEP].data;
    fps_delay = FPS_DELAY;
    vTaskResume(env_handle);

    vTaskResume(motion_handle);

    struct tm *time_now = time_get();
    week_cur = time_now->tm_wday;

    page_id = 0;
    ui_update[page_id](0, 0);
}

bool flag_backlight = false;
static void loop(void *data)
{
    // while (1)
    {
        KEY_TYPE key = KEY_NONE;
        key = app_key_get();
        switch (key)
        {
        case KEY0_SHORT:
            switchPage();
            break;
        case KEY0_LONG: // 长按
            page_exit();
            break;

        case KEY0_DOUBLE:
            if (flag_backlight == false)
            {
                beep_start();
            }
            else
            {
                beep_stop();
            }
            flag_backlight = !flag_backlight;

            break;
        default:
            break;
        }
        if (millis() - lastFpsMillis_fps >= fps_delay)
        {
            lastFpsMillis_fps = millis();

            charge_state = battery_getChargeState();
            switch (charge_state)
            {
            case CHARGING:
                ui_batt_update(0, 0);
                fps_delay = 1;
                delay(1);
                break;
            case FULL:

            case UNCHARGED:

            default:
                ui_update[page_id](0, 0);
                fps_delay = FPS_DELAY;
                delay(200);
                break;
            }
        }
    }
}

static void exit(void *data)
{

    vTaskSuspend(env_handle);
    vTaskSuspend(motion_handle);
}

#include "img.h"
Page page_home = {
    .init = NULL,
    .enter = enter,
    .exit = exit,
    .loop = loop,
    .title_en = "home",
    .title_cn = "主页",
    .icon = img_bits,
    .icon_width = img_width,
    .icon_height = img_height,

};