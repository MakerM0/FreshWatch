/*** 
 * @Author: MakerM0
 * @Date: 2024-09-05 16:33:32
 * @LastEditTime: 2024-10-03 19:51:14
 * @FilePath: \jdi_watch_gfx\src\driver\drv_neopixel.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */
#include "drv_neopixel.h"

 
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NEOPIXEL_NUM, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels


void neopixel_setup()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(PIN_NEOPIXEL_PWR, OUTPUT);
    digitalWrite(PIN_NEOPIXEL_PWR, 0);//power off 

    pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

    neopixel_setColor(pixels.Color(0, 255, 0));
    delay(100);
    neopixel_powerOff();
}

void neopixel_powerOn()
{
    digitalWrite(PIN_NEOPIXEL_PWR, 1);//power off 
}

void neopixel_powerOff()
{
    digitalWrite(PIN_NEOPIXEL_PWR, 0);//power off 
}

void neopixel_alert()
{
    neopixel_powerOn();
    delay(2);
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.show(); // Send the updated pixel colors to the hardware.
}

void neopixel_clear()
{
    neopixel_powerOff();
    pixels.clear(); // Set all pixel colors to 'off'
    // pixels.show();  // Send the updated pixel colors to the hardware.
    

}

void neopixel_setColor(uint32_t c)
{
    neopixel_powerOn();
 
    delay(2);
    pixels.setPixelColor(0, c);
    pixels.show();
}










