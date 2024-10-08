#include "settings.h"

#include "driver/drv_key.h"
#include "driver/drv_disp.h"
#include "driver/drv_time.h"

#include "fonts/fonts.h"

#define DIGIFONT DigitTech16_Regular10pt7b

// typedef union datatype
// {
//     int INT;
//     bool BOOL;

// } datatype;



para_t para[] = {
    {"Year", 2024, 9999},
    {"Month", 5, 12},
    {"Day", 19, 31},
    {"Hour", 10, 23},
    {"Minute", 25, 59},
    {"Second", 35, 59},
    {"Step\r\nTarget", 4000, 99999},
    {"Air\r\nThreshold", 100, 500},
    {"Beep", 0, 1},
    {"RGB\r\nled", 0, 1}};

uint8_t item_index = 0;
void drawItem()
{
    int16_t x1;
    int16_t y1;
    uint16_t w;
    uint16_t h;
    static int16_t TITLE_HEIGHT = 40;

    disp.fillRect(0, 0, DISP_WIDTH, TITLE_HEIGHT, COLOR_GREEN);
    disp.fillRect(0, TITLE_HEIGHT, DISP_WIDTH, DISP_HEIGHT - TITLE_HEIGHT, COLOR_BLACK);

    disp.setFont(&FreeMono9pt7b);
    disp.setTextSize(1);

    disp.getTextBounds(para[item_index].name, 0, 0, &x1, &y1, &w, &h);
    disp.setCursor(0 - x1, 0 - y1 + ((TITLE_HEIGHT - h) / 2));
    disp.setTextColor(COLOR_BLACK);
    disp.print(para[item_index].name);

    disp.setFont(&DIGIFONT);
    disp.setTextSize(1);
    char buf[10];
    sprintf(buf, "%5d", para[item_index].data);

    disp.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
    disp.setCursor(0 - x1, 70 - y1);
    disp.setTextColor(COLOR_BLACK);
    disp.print(buf);
    disp.refresh();
}

// 获取某个数字的各个位的数
void getDigits(int number, int *digits, int *size)
{
    *size = 0;
    if (number == 0)
    {
        digits[0] = 0;
        *size = 1;
        return;
    }

    while (number > 0)
    {
        digits[*size] = number % 10;
        number /= 10;
        (*size)++;
    }

    // 反转数组，使得最高位在前
    for (int i = 0; i < *size / 2; i++)
    {
        int temp = digits[i];
        digits[i] = digits[*size - 1 - i];
        digits[*size - 1 - i] = temp;
    }
}

// 获取某个数字的各个位的数
void getDigits(int number, int *digits, int size)
{

    for (uint8_t i = 0; i < size; i++)
    {
        digits[size - i - 1] = number % 10;
        number /= 10;
    }
}

// 用来获取最大数的位数
int getDigitsSize(int number)
{
    int size = 0;
    while (number > 0)
    {

        number /= 10;
        size++;
    }
    return size;
}

void printNumber(int number, int size, int currentPosition)
{
    disp.fillRect(0, 40, DISP_WIDTH, DISP_HEIGHT - 40, COLOR_WHITE);
    int16_t x1;
    int16_t y1;
    uint16_t w;
    uint16_t h;
    // char buf[10];
    // sprintf(buf, "%05d", para[item_index].max);
    disp.setFont(&DIGIFONT);
    disp.setTextSize(1);

    disp.getTextBounds("0", 0, 0, &x1, &y1, &w, &h);

    uint8_t x[size] = {0};
    for (uint8_t i = 0; i < size; i++)
    {
        x[size - 1 - i] = number % 10;
        number = number / 10;
    }

    disp.setCursor(0 - x1 + w * (6 - size), 70 - y1);

    for (int i = 0; i < size; i++)
    {

        if (i == currentPosition)
        {
            disp.setTextColor(COLOR_RED);
            disp.printf("%01d", x[i]);
        }
        else
        {
            disp.setTextColor(COLOR_BLACK);
            disp.printf("%01d", x[i]);
        }
    }

    disp.setCursor(0 - x1, 100 - y1);
    disp.printf("%d\r\n", size);
    disp.printf("%d", currentPosition);
    disp.refresh();
}

void printNumber(int *digits, int size, int currentPosition)
{
    static bool invert = false;
    invert = !invert;
    disp.fillRect(0, 40, DISP_WIDTH, DISP_HEIGHT - 40, COLOR_BLACK);

    int16_t x1;
    int16_t y1;
    uint16_t w;
    uint16_t h;
    // char buf[10];
    // sprintf(buf, "%05d", para[item_index].max);
    disp.setFont(&DIGIFONT);
    disp.setTextSize(1);

    disp.getTextBounds("0", 0, 0, &x1, &y1, &w, &h);
    static int16_t WIDTH = 14;
    if (para[item_index].max == 1)
    {
        disp.setCursor((5 - 3) * WIDTH, 70 - y1);
        
        if (digits[0] % 2)
        {
            disp.setTextColor(COLOR_GREEN);
            disp.print(" ON");
        }
        else
        {
            disp.setTextColor(COLOR_RED);
            disp.print("OFF");
        }
    }
    else
    {

        for (int i = 0; i < size; i++)
        {
            disp.setCursor(WIDTH * i + ((5 - size) * WIDTH), 70 - y1);
            if (i == currentPosition)
            {
                if (invert)
                {
                    disp.fillRoundRect(WIDTH * i + ((5 - size) * WIDTH), 70 - 5, w + 4, h + 10, 3, COLOR_WHITE);
                    disp.setTextColor(COLOR_BLACK);
                    disp.printf("%01d", digits[i]);
                }
                else
                {
                    disp.drawRoundRect(WIDTH * i + ((5 - size) * WIDTH), 70 - 5, w + 4, h + 10, 3, COLOR_WHITE);
                    disp.setTextColor(COLOR_WHITE);
                    disp.printf("%01d", digits[i]);
                }
            }
            else
            {
                disp.setTextColor(COLOR_WHITE);
                disp.printf("%01d", digits[i]);
            }
        }
    }

    // disp.fillRect(0,130,DISP_WIDTH,DISP_HEIGHT-130,COLOR_RED);
    disp.drawFastHLine(0, 125, DISP_WIDTH, COLOR_RED);
    disp.setFont();
    disp.setTextSize(1);
    disp.setCursor(10, 130);
    disp.setTextColor(COLOR_WHITE);
    disp.printf("MAX: %d\r\n", (para[item_index].max));
    // disp.printf("%d", currentPosition);
    disp.refresh();
}
int getNumber(int *digits, int size)
{
    int result = 0;
    for (int i = 0; i < size; i++)
    {
        result = result * 10 + digits[i];
    }
    return result;
}
typedef struct setting
{
    int step_target;
    bool flag_backlight;

} setting;

#define MAX_DIGITS 5
int currentPosition = 0; // 所修改的数字在哪个位置

int digits[MAX_DIGITS] = {0};
int size = 1;
static uint32_t lastFpsMillis_acc = 0;
static void enter(void *data)
{
    struct tm *time_now = time_get();
    para[0].data = time_now->tm_year + 1900;
    para[1].data = time_now->tm_mon + 1;
    para[2].data = time_now->tm_mday;
    para[3].data = time_now->tm_hour;
    para[4].data = time_now->tm_min;
    para[5].data = time_now->tm_sec;

    // init(NULL);
    disp.refresh();
    disp.fillScreen(COLOR_BLACK);
    drawItem();
    size = getDigitsSize(para[item_index].max);

    getDigits(para[item_index].data, digits, size);
    currentPosition = size - 1;
    printNumber(digits, size, currentPosition);

    disp.refresh();

    // manager_setBusy(false);
}

static void loop(void *data)
{
    KEY_TYPE key = app_key_get();
    switch (key)
    {
    case KEY0_SHORT:

        para[item_index].data = getNumber(digits, size);

        item_index++;
        item_index = item_index % ARRAY_SIZE(para);

        drawItem();

        size = getDigitsSize(para[item_index].max);

        getDigits(para[item_index].data, digits, size);
        currentPosition = size - 1;
        printNumber(digits, size, currentPosition);

        break;
    case KEY0_LONG: // 长按//退出
        para[item_index].data = getNumber(digits, size);
        for (uint8_t i = 0; i < ARRAY_SIZE(para); i++)
        {
            if (para[i].max == 1)
            {
                para[i].data = para[i].data % 2;
            }
        }
        time_set(para[0].data,para[1].data,para[2].data,para[3].data,para[4].data,para[5].data);
        page_exit();
        break;
    default:
        break;
    }
    if (millis() - lastFpsMillis_acc >= 500)
    {

        lastFpsMillis_acc = millis();

        switch (getAccAction())
        {
        case ACTION_DOWN:

            digits[currentPosition] = (digits[currentPosition] + 1) % 10;
            // printNumber(digits, size, currentPosition);
            break;
        case ACTION_UP:
            digits[currentPosition] = (digits[currentPosition] + 9) % 10;
            // printNumber(digits, size, currentPosition);
            break;
        case ACTION_RIGHT:
            if (currentPosition > 0)
            {
                currentPosition--;
                // printNumber(digits, size, currentPosition);
            }
            break;
        case ACTION_LEFT:

            if (currentPosition < size - 1)
            {
                currentPosition += 1;
                // printNumber(digits, size, currentPosition);
            }
            break;

        default:
            break;
        }
        printNumber(digits, size, currentPosition);
    }
}

static void exit(void *data)
{

    // manager_setBusy(true);
}
#include "img.h"
Page page_settings = {
    .init = NULL,
    .enter = enter,
    .exit = exit,
    .loop = loop,
    .title_en = "settings",
    .title_cn = " 设置",
    .icon = img_bits,
    .icon_width = img_width,
    .icon_height = img_height,

};