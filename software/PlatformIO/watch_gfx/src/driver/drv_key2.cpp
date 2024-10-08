
#include "drv_key.h"
#include <Arduino.h>

const int BUTTON_PIN = PIN_BUTTON1;           // 按钮连接的引脚
const unsigned long DEBOUNCE_DELAY = 10;      // 消抖延时（毫秒）
const unsigned long DOUBLE_CLICK_DELAY = 250; // 双击间隔（毫秒）
const unsigned long LONG_PRESS_DELAY = 1000;  // 长按时间（毫秒）

volatile unsigned long lastDebounceTime = 0;
volatile unsigned long lastClickTime = 0;
volatile int buttonState = HIGH;
volatile int lastButtonState = HIGH;
volatile int clickCount = 0;

static KEY_TYPE key_type;
BaseType_t xHigherPriorityTaskWoken;

void handleSingleClick()
{
    //   Serial.println("单击");
    key_type = KEY0_SHORT;

    if (Key_Queue != 0)
    {
        BaseType_t err = xQueueSendFromISR(Key_Queue, &key_type, &xHigherPriorityTaskWoken);
        if (err == errQUEUE_FULL)
        {
            // 队列已满，数据发送失败
        }
    }
}

void handleDoubleClick()
{
    //   Serial.println("双击");
    key_type = KEY0_DOUBLE;

    if (Key_Queue != 0)
    {
        BaseType_t err = xQueueSendFromISR(Key_Queue, &key_type, &xHigherPriorityTaskWoken);
        
        if (err == errQUEUE_FULL)
        {
            // 队列已满，数据发送失败
        }
    }
}

void handleLongPress()
{
    //   Serial.println("长按");
    key_type = KEY0_LONG;

    if (Key_Queue != 0)
    {
        BaseType_t err = xQueueSendFromISR(Key_Queue, &key_type, &xHigherPriorityTaskWoken);
        if (err == errQUEUE_FULL)
        {
            // 队列已满，数据发送失败
        }
    }
}

void buttonInterrupt()
{
    unsigned long currentTime = millis();

    if (currentTime - lastDebounceTime > DEBOUNCE_DELAY)
    {
        int reading = digitalRead(BUTTON_PIN);

        if (reading != lastButtonState)
        {
            lastButtonState = reading;

            if (reading == LOW)
            {
                // 按钮被按下
                if (currentTime - lastClickTime < DOUBLE_CLICK_DELAY)
                {
                    clickCount++;
                }
                else
                {
                    clickCount = 1;
                }
                lastClickTime = currentTime;
            }
            else
            {
                // 按钮被释放
                unsigned long pressDuration = currentTime - lastClickTime;

                if (pressDuration < LONG_PRESS_DELAY)
                {
                    if (clickCount == 1)
                    {
                        // 单击
                        handleSingleClick();
                    }
                    else if (clickCount == 2)
                    {
                        // 双击
                        handleDoubleClick();
                    }
                }
                else
                {
                    // 长按
                    handleLongPress();
                }
            }
        }

        lastDebounceTime = currentTime;
    }
}
void key2_init()
{
    // 创建按键消息队列
    Key_Queue = xQueueCreate(30, sizeof(uint8_t));
    if (Key_Queue == 0)
    {
        // Queue was not created and must not be used.
    }

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // ISR_DEFERRED flag cause the callback to be deferred from ISR context
    // and invoked within a callback thread.
    // It is required to use ISR_DEFERRED if callback function take long time
    // to run e.g Serial.print() or using any of Bluefruit API() which will
    // potentially call rtos API
    // attachInterrupt(BUTTON_PIN, buttonInterrupt, ISR_DEFERRED | CHANGE);
    attachInterrupt(BUTTON_PIN, buttonInterrupt, CHANGE);
}