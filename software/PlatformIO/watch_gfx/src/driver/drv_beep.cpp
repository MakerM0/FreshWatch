/***
 * @Author: MakerM0
 * @Date: 2024-09-07 13:11:14
 * @LastEditTime: 2024-10-03 19:50:51
 * @FilePath: \jdi_watch_gfx\src\driver\drv_beep.cpp
 * @Description:
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved.
 */
#include "drv_beep.h"
#include "nrf.h"
#include "nrf_pwm.h"
#include "nRF52_PWM.h"

const int maxValue = bit(15) - 1;

void beep_setup()
{
#ifdef MODE_BEEP
    HwPWM1.addPin(PIN_BEEP);

    // Enable PWM modules with 15-bit resolutions(max) but different clock div

    HwPWM1.setResolution(10);
    HwPWM1.setMaxValue(250);
    HwPWM1.setClockDiv(PWM_PRESCALER_PRESCALER_DIV_32); // freq = 500khz
    HwPWM1.begin();
#else
    pinMode(PIN_BEEP, OUTPUT);
    digitalWrite(PIN_BEEP, LOW);

#endif
}

void beep_start()
{
#ifdef MODE_BEEP
    HwPWM1.writePin(PIN_BEEP, 200, false);
#else
    digitalWrite(PIN_BEEP, HIGH);

#endif
}

void beep_stop()
{
#ifdef MODE_BEEP
        HwPWM1.stop();
#else
    digitalWrite(PIN_BEEP, LOW);

#endif
}
// nRF52_PWM *PWM_Instance;

// float frequency = 2000.0f;

// float dutyCycle = 0.0f;

// #define pinToUse PIN_BEEP

// void beep_setup()
// {

//     // assigns PWM frequency of 1.0 KHz and a duty cycle of 0%
//     PWM_Instance = new nRF52_PWM(pinToUse, frequency, dutyCycle);

//     if ((!PWM_Instance) || !PWM_Instance->isPWMEnabled())
//     {
//         // Serial.print(F("Stop here forever"));

//         while (true)
//             delay(10000);
//     }
// }

// void beep_start()
// {
//     // You can change frequency here, anytime
//     frequency = 2000.0f;
//     // frequency = 20.0f;
//     dutyCycle = 50.0f;
//     PWM_Instance->setPWM(pinToUse, frequency, dutyCycle); // 512对应于10位分辨率下的50%占空比
// }

// void beep_stop()
// {
//  frequency = 2000.0f;
//     // frequency = 20.0f;
//     dutyCycle = 0.0f;
//     PWM_Instance->setPWM(pinToUse, frequency, dutyCycle); // 512对应于10位分辨率下的50%占空比
// }
