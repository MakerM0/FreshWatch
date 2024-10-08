/*** 
 * @Author: MakerM0
 * @Date: 2024-09-05 16:18:21
 * @LastEditTime: 2024-10-03 19:51:02
 * @FilePath: \jdi_watch_gfx\src\driver\drv_disp.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */

#include "drv_disp.h"
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>


JDI_MIP_Display disp;
 
const uint16_t colors[] = {COLOR_MAGENTA, COLOR_GREEN, COLOR_CYAN, COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE};

SoftwareTimer extcominTimer;
/**
 * Software Timer callback is invoked via a built-in FreeRTOS thread with
 * minimal stack size. Therefore it should be as simple as possible. If
 * a periodically heavy task is needed, please use Scheduler.startLoop() to
 * create a dedicated task for it.
 *
 * More information http://www.freertos.org/RTOS-software-timer.html
 */
void extcomin_timer_callback(TimerHandle_t xTimerID)
{
    // freeRTOS timer ID, ignored if not used
    (void)xTimerID;

    digitalToggle(PIN_TFT_EXTCOMIN);
}

void disp_extcominTimer_stop()
{
    extcominTimer.stop();
}

void disp_setup()
{
    pinMode(PIN_TFT_EXTCOMIN, OUTPUT);
    digitalWrite(PIN_TFT_EXTCOMIN,LOW);
    disp.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV8);
    delay(1);    
    // disp.frontlightOn(); // Optional depending on the display model
    
    //全清会产生1mA的电流，待改进
    disp.clearScreen(); // Clear the screen
    delay(1);    

    disp.displayOn();
    disp.fillScreen(COLOR_BLACK);
    disp.refresh();
 
  

    

    // NRF_SPIM0->ENABLE = 0;
    // Configure the timer with 1000 ms interval, with our callback
    extcominTimer.begin(500, extcomin_timer_callback);

    // Start the timer
    extcominTimer.start();

}