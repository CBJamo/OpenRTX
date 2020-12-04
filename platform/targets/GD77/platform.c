/***************************************************************************
 *   Copyright (C) 2020 by Federico Amedeo Izzo IU2NUO,                    *
 *                         Niccolò Izzo IU2KIN                             *
 *                         Frederik Saraci IU2NRO                          *
 *                         Silvano Seva IU2KWO                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   As a special exception, if other files instantiate templates or use   *
 *   macros or inline functions from this file, or you compile this file   *
 *   and link it with other works to produce a work based on this file,    *
 *   this file does not by itself cause the resulting work to be covered   *
 *   by the GNU General Public License. However the source code for this   *
 *   file must still be made available in accordance with the GNU General  *
 *   Public License. This exception does not invalidate any other reasons  *
 *   why a work based on this file might be covered by the GNU General     *
 *   Public License.                                                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <platform.h>
#include <gpio.h>
#include "hwconfig.h"
#include "rtc.h"

curTime_t rtc_getTime()
{
    /* TODO */
    curTime_t t;
    t.hour   = 12;
    t.minute = 12;
    t.second = 12;
    t.year  = 2020;
    t.day   = 4;
    t.month = 12;
    t.date  = 12;
    return t;
}

void platform_init()
{
    /* Configure GPIOs */
    gpio_setMode(GREEN_LED, OUTPUT);
    gpio_setMode(RED_LED, OUTPUT);

    gpio_setMode(LCD_BKLIGHT, OUTPUT);
    gpio_clearPin(LCD_BKLIGHT);
}

void platform_terminate()
{
    /* TODO */
}

float platform_getVbat()
{
    /* TODO */
    return 0.0f;
}

float platform_getMicLevel()
{
    /* TODO */
    return 0.0f;
}

float platform_getVolumeLevel()
{
    /* TODO */
    return 0.0f;
}

uint8_t platform_getChSelector()
{
    /* TODO */
    return 0;
}

bool platform_getPttStatus()
{
    /* TODO */
    return false;
}

void platform_ledOn(led_t led)
{
    switch(led)
    {
        case GREEN:
            gpio_setPin(GREEN_LED);
            break;

        case RED:
            gpio_setPin(RED_LED);
            break;

        default:
            break;
    }
}

void platform_ledOff(led_t led)
{
    switch(led)
    {
        case GREEN:
            gpio_clearPin(GREEN_LED);
            break;

        case RED:
            gpio_clearPin(RED_LED);
            break;

        default:
            break;
    }
}

void platform_beepStart(uint16_t freq)
{
    /* TODO */
    (void) freq;
}

void platform_beepStop()
{
    /* TODO */
}

void platform_setBacklightLevel(uint8_t level)
{
    /* TODO */
    if(level > 1)
    {
        gpio_setPin(LCD_BKLIGHT);
    }
    else
    {
        gpio_clearPin(LCD_BKLIGHT);
    }
}
