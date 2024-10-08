#ifndef __DRV_DISP_H
#define __DRV_DISP_H

#include <Arduino.h>
#include <JDI_MIP_Display.h>

extern JDI_MIP_Display disp;
extern const uint16_t colors[];
void disp_setup();
void disp_extcominTimer_stop();

#endif
