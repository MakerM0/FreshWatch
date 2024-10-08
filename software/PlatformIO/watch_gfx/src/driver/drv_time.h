#ifndef __DRV_TIME_H
#define __DRV_TIME_H

#include <time.h>
#include <Arduino.h>

extern const char *week[];

void time_setup();
void time_set(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second);

struct tm *time_get(void);

#endif
