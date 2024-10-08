/*** 
 * @Author: MakerM0
 * @Date: 2024-09-05 15:34:24
 * @LastEditTime: 2024-10-03 19:51:36
 * @FilePath: \jdi_watch_gfx\src\driver\drv_time.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */
#include "drv_time.h"

SoftwareTimer clockTimer;

static struct tm time_struct, m_tm_return_time;
static time_t newtime;


const char *week[]={"SUN","MON","TUE","WED","THU","FRI","SAT"};

void time_set(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second)
{

    time_struct.tm_year = year - 1900;// 距离 1900 的年数
    time_struct.tm_mon = month-1;// 月份 [0, 11]
    time_struct.tm_mday = day; // 天数 [1, 31]
    time_struct.tm_hour = hour;// 小时 [0, 23]
    time_struct.tm_min = minute; // 分钟 [0, 59]
    time_struct.tm_sec = second;// 秒数 [0, 60]
    time_struct.tm_isdst=-1,  // 夏令时
    newtime = mktime(&time_struct);
}

struct tm *time_get(void)
{
    time_t return_time = newtime;
    m_tm_return_time = *localtime(&return_time);
    return &m_tm_return_time;
}

void clock_timer_callback(TimerHandle_t xTimerID)
{
    // freeRTOS timer ID, ignored if not used
    (void)xTimerID;
    newtime++;
}


void time_setup()
{
        // Configure the timer with 1000 ms interval, with our callback
    clockTimer.begin(1000, clock_timer_callback);

    // Start the timer
    clockTimer.start();

}