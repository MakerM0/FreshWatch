
#ifndef __DRV_BEEP_H
#define __DRV_BEEP_H

#include <Arduino.h>
#define PIN_BEEP 15

// #define MODE_LIGHT
#define MODE_BEEP

void beep_setup();
void beep_start();
void beep_stop();

 


#endif
