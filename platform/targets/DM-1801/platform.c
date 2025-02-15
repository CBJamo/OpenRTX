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

#include <interfaces/platform.h>
#include <interfaces/nvmem.h>
#include <interfaces/gpio.h>
#include <calibInfo_GDx.h>
#include <ADC0_GDx.h>
#include <string.h>
#include <I2C0.h>
#include <pthread.h>
#include "hwconfig.h"

/* Mutex for concurrent access to ADC0 */
pthread_mutex_t adc_mutex;

gdxCalibration_t calibration;
hwInfo_t hwInfo;

void platform_init()
{
    /* Configure GPIOs */
    gpio_setMode(GREEN_LED, OUTPUT);
    gpio_setMode(RED_LED,   OUTPUT);

    gpio_setMode(LCD_BKLIGHT, OUTPUT);
    gpio_clearPin(LCD_BKLIGHT);

    gpio_setMode(PTT_SW, INPUT);

    gpio_setMode(PWR_SW, OUTPUT);

    /*
     * Configure backlight PWM: 58.5kHz, 8 bit resolution
     */
    SIM->SCGC6 |= SIM_SCGC6_FTM0(1); /* Enable clock */

    FTM0->CONTROLS[3].CnSC = FTM_CnSC_MSB(1)
                           | FTM_CnSC_ELSB(1); /* Edge-aligned PWM, clear on match */
    FTM0->CONTROLS[3].CnV  = 0;

    FTM0->MOD  = 0xFF;                         /* Reload value          */
    FTM0->SC   = FTM_SC_PS(3)                  /* Prescaler divide by 8 */
               | FTM_SC_CLKS(1);               /* Enable timer          */

    gpio_setMode(LCD_BKLIGHT, OUTPUT);
    gpio_setAlternateFunction(LCD_BKLIGHT, 2);

    /*
     * Initialise ADC
     */
    adc0_init();
    pthread_mutex_init(&adc_mutex, NULL);

    /*
     * Initialise I2C driver, once for all the modules
     */
    gpio_setMode(I2C_SDA, OPEN_DRAIN);
    gpio_setMode(I2C_SCL, OPEN_DRAIN);
    gpio_setAlternateFunction(I2C_SDA, 3);
    gpio_setAlternateFunction(I2C_SCL, 3);
    i2c0_init();

    /*
     * Initialise non volatile memory manager and zero calibration data.
     * Actual loading of calibration data is deferred to the first call of
     * platform_getCalibrationData().
     */
    nvm_init();
    memset(&calibration, 0x00, sizeof(gdxCalibration_t));

    /* Initialise hardware information structure */
    hwInfo.uhf_maxFreq = FREQ_LIMIT_UHF_HI/1000000;
    hwInfo.uhf_minFreq = FREQ_LIMIT_UHF_LO/1000000;
    hwInfo.vhf_maxFreq = FREQ_LIMIT_VHF_HI/1000000;
    hwInfo.vhf_minFreq = FREQ_LIMIT_VHF_LO/1000000;
    hwInfo.uhf_band    = 1;
    hwInfo.vhf_band    = 1;
    hwInfo.lcd_type    = 0;
    memcpy(hwInfo.name, "DM-1801", 7);
    hwInfo.name[7] = '\0';
}

void platform_terminate()
{
    gpio_clearPin(LCD_BKLIGHT);
    gpio_clearPin(RED_LED);
    gpio_clearPin(GREEN_LED);

    adc0_terminate();
    pthread_mutex_destroy(&adc_mutex);

    i2c0_terminate();

    /* Finally, remove power supply */
    gpio_clearPin(PWR_SW);
}

float platform_getVbat()
{
    float value = 0.0f;
    pthread_mutex_lock(&adc_mutex);
    value = adc0_getMeasurement(1);
    pthread_mutex_unlock(&adc_mutex);

    return (value * 3.0f)/1000.0f;
}

float platform_getMicLevel()
{
    float value = 0.0f;
    pthread_mutex_lock(&adc_mutex);
    value = adc0_getMeasurement(3);
    pthread_mutex_unlock(&adc_mutex);

    return value;
}

float platform_getVolumeLevel()
{
    /* TODO */
    return 0.0f;
}

uint8_t platform_getChSelector()
{
    /* GD77 does not have a channel selector */
    return 0;
}

bool platform_getPttStatus()
{
    /* PTT line has a pullup resistor with PTT switch closing to ground */
    return (gpio_readPin(PTT_SW) == 0) ? true : false;
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
    FTM0->CONTROLS[3].CnV = level;
}

const void *platform_getCalibrationData()
{
    /* The first time this function is called, load calibration data from flash */
    if(calibration.vhfCalPoints[0] == 0)
    {
        nvm_readCalibData(&calibration);
    }

    return ((const void *) &calibration);
}

const hwInfo_t *platform_getHwInfo()
{
    return &hwInfo;
}
