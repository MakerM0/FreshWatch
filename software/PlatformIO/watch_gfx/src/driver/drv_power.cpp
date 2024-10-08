/*** 
 * @Author: MakerM0
 * @Date: 2024-09-05 16:27:50
 * @LastEditTime: 2024-10-03 19:51:20
 * @FilePath: \jdi_watch_gfx\src\driver\drv_power.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */
#include "drv_power.h"
#include "drv_disp.h"
#include "drv_sensor.h"
#include "drv_key.h"
#include "driver/drv_beep.h"


void power_setup()
{
    // Bluefruit.begin();          // Sleep functions need the softdevice to be active.
}

// enter deep sleep mode
void power_off()
{
    app_key_stopLoop();
    disp_extcominTimer_stop();
    vTaskSuspendAll();
    delay(100);
    // // to reduce power consumption when sleeping, turn off all your LEDs (and other power hungry devices)
    digitalWrite(PIN_NEOPIXEL_PWR, LOW);
    digitalWrite(PIN_NEOPIXEL, LOW);
    digitalWrite(PIN_BEEP, LOW);
    


    
    sgp.heaterOff();
    lsm6ds3trc.enablePedometer(false);
    lsm6ds3trc.setAccelDataRate(LSM6DS_RATE_SHUTDOWN);
    lsm6ds3trc.setGyroDataRate(LSM6DS_RATE_SHUTDOWN);
 
 
    sht31.reset();
    

    digitalWrite(PIN_TFT_EXTCOMIN, LOW);
    disp.displayOff();

    Wire.end();
    SPI.end();

    // setup your wake-up pins.++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    pinMode(PIN_BUTTON1, INPUT_PULLUP_SENSE); // this pin (WAKE_LOW_PIN) is pulled up and wakes up the feather when externally connected to ground.
    // pinMode(WAKE_HIGH_PIN, INPUT_PULLDOWN_SENSE);  // this pin (WAKE_HIGH_PIN) is pulled down and wakes up the feather when externally connected to 3.3v.

    // power down nrf52.
    sd_power_system_off(); // this function puts the whole nRF52 to deep sleep (no Bluetooth).  If no sense pins are setup (or other hardware interrupts), the nrf52 will not wake up.
    delay(1000);
}
