
#ifndef __DRV_NEOPIXEL_H
#define __DRV_NEOPIXEL_H

#include <Adafruit_NeoPixel.h>

void neopixel_setup();;

void neopixel_powerOn();
void neopixel_powerOff();

void neopixel_alert();

void neopixel_clear();

void neopixel_setColor(uint32_t c);


extern Adafruit_NeoPixel pixels;


#endif
