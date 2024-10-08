/*** 
 * @Author: MakerM0
 * @Date: 2024-09-08 14:23:19
 * @LastEditTime: 2024-10-03 19:51:07
 * @FilePath: \jdi_watch_gfx\src\driver\drv_key.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */
#include "drv_key.h"

#define PIN_SW01 PIN_BUTTON1 

QueueHandle_t Key_Queue;
BaseType_t err;




Button2 key0 = Button2(PIN_SW01, INPUT_PULLUP, true);

SoftwareTimer keyTimer;
SoftwareTimer longTimer; //长按提示

// extern SoftwareTimer timeoutTimer;

static KEY_TYPE key_type;

static void handler(Button2 &btn);
static void key_callback(void);

/////////////////////////////////////////////////////////////////

static void pressed(Button2 &btn)
{
  // Serial.println("pressed");
  if (btn == key0)
  {
    key_type = KEY0_DOWN;
  }

  // // app_motor_once(60);
  // app_timeout_reset();
  longTimer.start();

  if (Key_Queue != 0)
  {
    err = xQueueSend(Key_Queue, &key_type, 10);
    if (err == errQUEUE_FULL)
    {
      // 队列已满，数据发送失败
    }
  }
}
static void released(Button2 &btn)
{
  // Serial.print("released: ");
  // Serial.println(btn.wasPressedFor());
  
  if (btn == key0)
  {
    key_type = KEY0_UP;
  }

  longTimer.stop();

  if (Key_Queue != 0)
  {
    err = xQueueSend(Key_Queue, &key_type, 10);
    if (err == errQUEUE_FULL)
    {
      // 队列已满，数据发送失败
    }
  }
}
static void changed(Button2 &btn)
{
  // Serial.println("changed");
}
static void click(Button2 &btn)
{
  // Serial.println("click\n");
  if (btn == key0)
  {
    key_type = KEY0_SHORT;
  }

  if (Key_Queue != 0)
  {
    err = xQueueSend(Key_Queue, &key_type, 10);
    if (err == errQUEUE_FULL)
    {
      // 队列已满，数据发送失败
    }
  }
}
static void longClick(Button2 &btn)
{
  // Serial.println("long click\n");
  if (btn == key0)
  {
    key_type = KEY0_LONG;
  }

  if (Key_Queue != 0)
  {
    err = xQueueSend(Key_Queue, &key_type, 10);
    if (err == errQUEUE_FULL)
    {
      // 队列已满，数据发送失败
    }
  }
}
static void doubleClick(Button2 &btn)
{
  // Serial.println("double click\n");
  // Serial.println("long click\n");
  if (btn == key0)
  {
    key_type = KEY0_DOUBLE;
  }

  if (Key_Queue != 0)
  {
    err = xQueueSend(Key_Queue, &key_type, 10);
    if (err == errQUEUE_FULL)
    {
      // 队列已满，数据发送失败
    }
  }
}
static void tripleClick(Button2 &btn)
{
  // Serial.println("triple click\n");
}
static void tap(Button2 &btn)
{
  // Serial.println("tap");
}
/////////////////////////////////////////////////////////////////

static void key_timer_callback(TimerHandle_t xTimerID)
{
  // freeRTOS timer ID, ignored if not used
  (void)xTimerID;
  key0.loop();
}

static void long_timer_callback(TimerHandle_t xTimerID)
{
  // freeRTOS timer ID, ignored if not used
  (void)xTimerID;
  // app_motor_once(70);
}

void app_key_init(void)
{

  pinMode(PIN_SW01, INPUT_PULLUP);
  key0.setDebounceTime(10);
  key0.setLongClickTime(1500);
  

  // key0.setChangedHandler(changed);
  key0.setPressedHandler(pressed);
  key0.setReleasedHandler(released);

  // key0.setTapHandler(tap);
  key0.setClickHandler(click);
  key0.setLongClickHandler(longClick);
  key0.setDoubleClickHandler(doubleClick);
  // key0.setTripleClickHandler(tripleClick);

  // ISR_DEFERRED flag cause the callback to be deferred from ISR context
  // and invoked within a callback thread.
  // It is required to use ISR_DEFERRED if callback function take long time
  // to run e.g Serial.print() or using any of Bluefruit API() which will
  // potentially call rtos API
  //   attachInterrupt(interruptPin, digital_callback, ISR_DEFERRED | CHANGE);
  // attachInterrupt(PIN_KEY0, key_callback,  CHANGE);

  //创建按键消息队列
  Key_Queue = xQueueCreate(30, sizeof(uint8_t));
  if (Key_Queue == 0)
  {
    // Queue was not created and must not be used.
  }

  // tpkey.setLongClickHandler(handler);
  // // Configure the timer with 1000 ms interval, with our callback
  keyTimer.begin(20, key_timer_callback);

  // // Start the timer
  if (!keyTimer.start())
  {
    // tft.println("timer start fail");
  }

  // tpkey.setLongClickHandler(handler);
  // // Configure the timer with 1000 ms interval, with our callback
  longTimer.begin(1000, long_timer_callback, NULL, false);
}

void app_key_deinit(void)
{

  keyTimer.~SoftwareTimer();

  nrf_gpio_cfg_default(PIN_SW01);
}

void app_key_config_wakeup(void)
{
  pinMode(PIN_SW01, INPUT_PULLUP_SENSE); // this pin (WAKE_HIGH_PIN) is pulled down and wakes up the feather when externally connected to 3.3v.
}

KEY_TYPE app_key_get()
{
  key_type = KEY_NONE;
  if (Key_Queue != 0)
  {
    // if(xQueueReceive(Key_Queue, &key_type,portMAX_DELAY))    //  10/portTICK_PERIOD_MS
    if (xQueueReceive(Key_Queue, &key_type, 0))
    {
      return key_type;
    }
    return key_type;
  }

  return key_type;
}

//清楚多余按键信息
void app_key_clean()
{
  if (Key_Queue != 0)
  {
    xQueueReset(Key_Queue);
  }
}

//停止loop
void app_key_stopLoop()
{
  keyTimer.stop();
  longTimer.stop();
}

//开始loop
void app_key_startLoop()
{
  keyTimer.reset();
  keyTimer.start();
}