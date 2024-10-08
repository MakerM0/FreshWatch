/*** 
 * @Author: MakerM0
 * @Date: 2024-09-06 12:56:50
 * @LastEditTime: 2024-10-03 19:50:45
 * @FilePath: \jdi_watch_gfx\src\driver\drv_battey.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */
#include "drv_battery.h"

#define PIN_CHARGE 26
#define PIN_5V 13

float battery_mv;
Battery_State_t charge_state;

// from library
uint32_t vbat_pin = PIN_VBAT;         // A7 for feather nRF52832, A6 for nRF52840
#define VBAT_MV_PER_LSB (0.73242188F) // 3.0V ADC range and 12-bit ADC resolution = 3000mV/4096

#ifdef NRF52840_XXAA
#define VBAT_DIVIDER (0.5F)      // 150K + 150K voltage divider on VBAT
#define VBAT_DIVIDER_COMP (2.0F) // Compensation factor for the VBAT divider
#else
#define VBAT_DIVIDER (0.71275837F) // 2M + 0.806M voltage divider on VBAT = (2M / (0.806M + 2M))
#define VBAT_DIVIDER_COMP (1.403F) // Compensation factor for the VBAT divider
#endif

#define REAL_VBAT_MV_PER_LSB (VBAT_DIVIDER_COMP * VBAT_MV_PER_LSB)

float battery_readmv(void)
{
    float raw;

    // Set the analog reference to 3.0V (default = 3.6V)
    analogReference(AR_INTERNAL_3_0);

    // Set the resolution to 12-bit (0..4095)
    analogReadResolution(12); // Can be 8, 10, 12 or 14

    // Let the ADC settle
    delay(1);

    // Get the raw 12-bit, 0..3000mV ADC value
    raw = analogRead(vbat_pin);

    // Set the ADC back to the default settings
    analogReference(AR_DEFAULT);
    analogReadResolution(10);

    // Convert the raw value to compensated mv, taking the resistor-
    // divider into account (providing the actual LIPO voltage)
    // ADC range is 0..3000mV and resolution is 12-bit (0..4095)
    return raw * REAL_VBAT_MV_PER_LSB;
}

uint8_t battery_mvToPercent(float mvolts)
{
    if (mvolts < 3300)
        return 0;

    if (mvolts < 3600)
    {
        mvolts -= 3300;
        return mvolts / 30;
    }
    else if (mvolts < 4100)
    {
        mvolts -= 3600;
        return 10 + (mvolts * 0.16F); // thats mvolts /6.66666666
    }

    return 100;
}

void battery_setup()
{
    pinMode(PIN_CHARGE, INPUT_PULLUP);
    pinMode(PIN_5V, INPUT);
}

static bool battery_5vValid()
{
    if (digitalRead(PIN_5V))
    {
        return true;
    }

    else
    {
        return false;
    }
}

static bool battery_charging()
{
    if (digitalRead(PIN_CHARGE))
    {
        return false;
    }

    else
    {
        return true;
    }
}

Battery_State_t battery_getChargeState()
{
    if (battery_5vValid())
    {
        if (battery_charging())
        {
            return CHARGING;
        }
        else
        {
            return FULL;
        }
    }
    else
    {
        return UNCHARGED;
    }
}
