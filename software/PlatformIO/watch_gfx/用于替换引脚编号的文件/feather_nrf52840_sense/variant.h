 /*
  Copyright (c) 2014-2015 Arduino LLC.  All right reserved.
  Copyright (c) 2016 Sandeep Mistry All right reserved.
  Copyright (c) 2018, Adafruit Industries (adafruit.com)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VARIANT_FRESHWATCH_
#define _VARIANT_FRESHWATCH_

/** Master clock frequency */
#define VARIANT_MCK       (64000000ul)

#define USE_LFRC    // Board uses RC for LF

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "WVariant.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// Number of pins defined in PinDescription array
#define PINS_COUNT           (48)
#define NUM_DIGITAL_PINS     (48)
#define NUM_ANALOG_INPUTS    (6) // A6 is used for battery, A7 is analog reference
#define NUM_ANALOG_OUTPUTS   (0)

// LEDs
#define PIN_NEOPIXEL  (2)
#define PIN_NEOPIXEL_PWR         (32+13)
#define NEOPIXEL_NUM         1

#define LED_BUILTIN          -1
#define LED_CONN             -1

#define LED_RED              -1
#define LED_BLUE             -1

#define LED_STATE_ON         1         // State when LED is litted


/*
 * Buttons
 */
#define PIN_BUTTON1          (24)

// Microphone
#define PIN_PDM_DIN           -1
#define PIN_PDM_CLK           -1
#define PIN_PDM_PWR           -1  // not used

/*
 * Analog pins
 */
 
#define PIN_A6               (30)
#define PIN_A7               (31) 

 
#define ADC_RESOLUTION    14

// Other pins
#define PIN_AREF           PIN_A7
#define PIN_VBAT           PIN_A6
// #define PIN_NFC1           (33)
// #define PIN_NFC2           (2)

static const uint8_t AREF = PIN_AREF;

/*
 * Serial interfaces
 */
#define PIN_SERIAL1_RX       (28)
#define PIN_SERIAL1_TX       (6)

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 1

#define PIN_SPI_MISO         (29)
#define PIN_SPI_MOSI         (8)
#define PIN_SPI_SCK          (7)

static const uint8_t SS   = (12);
static const uint8_t MOSI = PIN_SPI_MOSI ;
static const uint8_t MISO = PIN_SPI_MISO ;
static const uint8_t SCK  = PIN_SPI_SCK ;



// On-board TFT display
#define PIN_TFT_CS           12
#define PIN_TFT_DISP           4
#define PIN_TFT_EXTCOMIN        (32+9)

/*
 * Wire Interfaces
 */
#define WIRE_INTERFACES_COUNT 1

#define PIN_WIRE_SDA         (3)
#define PIN_WIRE_SCL         (32+10)

// QSPI Pins
#define PIN_QSPI_SCK         32
#define PIN_QSPI_CS          (32+2)
#define PIN_QSPI_IO0         22
#define PIN_QSPI_IO1         20
#define PIN_QSPI_IO2         (32+4)
#define PIN_QSPI_IO3         (32+6)

// On-board QSPI Flash
#define EXTERNAL_FLASH_DEVICES   GD25Q32J
#define EXTERNAL_FLASH_USE_QSPI

#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------------
 *        Arduino objects - C++ only
 *----------------------------------------------------------------------------*/

#endif
