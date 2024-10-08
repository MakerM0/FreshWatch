
#ifndef __DRV_BATTEY_H
#define __DRV_BATTEY_H

#include <Arduino.h>

typedef enum
{
    CHARGING = 0,
    FULL,
    UNCHARGED,
    MAXNUM

} Battery_State_t;

extern float battery_mv;

void battery_setup();

float battery_readmv(void);

uint8_t battery_mvToPercent(float mvolts);

Battery_State_t battery_getChargeState();

extern Battery_State_t charge_state;

#endif
