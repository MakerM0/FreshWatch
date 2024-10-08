#ifndef __DRV_KEY_H
#define __DRV_KEY_H
#include "Button2.h"
#include <Arduino.h>



extern Button2 key0;
extern QueueHandle_t Key_Queue;
typedef enum
{
    KEY_NONE = 0,

    KEY0_DOWN,   //  按下
    KEY0_UP,     //  释放
    KEY0_SHORT,  //  短按
    KEY0_LONG,   //  长按
    KEY0_DOUBLE, //  双击

    // KEY1_DOWN,  //  按下
    // KEY1_UP,    //  释放
    // KEY1_SHORT, //  短按
    // KEY1_LONG,  //  长按

} KEY_TYPE;

void app_key_init(void);

void app_key_config_wakeup(void);

KEY_TYPE app_key_get();
void app_key_clean();


void app_key_stopLoop();
void app_key_startLoop();



void key2_init();
#endif
