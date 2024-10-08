#ifndef __SETTINGS_H
#define __SETTINGS_H
#include "ui/page.h"
#include "ui/menu.h"
#include <stdint.h>

#include <Arduino.h>

typedef struct para_t
{
    /* data */
    const char name[20];
    int data;
    int max; // 最大值

} para_t;


#define PARA_STEP 6

extern Page page_settings;
extern para_t para[];

#endif