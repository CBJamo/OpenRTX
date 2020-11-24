/***************************************************************************
 *   Copyright (C) 2020 by Federico Amedeo Izzo IU2NUO,                    *
 *                         Niccolò Izzo IU2KIN,                            *
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

#ifndef HWCONFIG_H
#define HWCONFIG_H

#include <stm32f4xx.h>

/* Screen dimensions */
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128

/* Display */
#define LCD_D0  GPIOD,14
#define LCD_D1  GPIOD,15
#define LCD_D2  GPIOD,0
#define LCD_D3  GPIOD,1
#define LCD_D4  GPIOE,7
#define LCD_D5  GPIOE,8
#define LCD_D6  GPIOE,9
#define LCD_D7  GPIOE,10
#define LCD_WR  GPIOD,5
#define LCD_RD  GPIOD,4
#define LCD_CS  GPIOD,6
#define LCD_RS  GPIOD,12
#define LCD_RST GPIOD,13
#define LCD_BKLIGHT GPIOD,8

/* Signalling LEDs */
#define GREEN_LED  GPIOE,0
#define RED_LED    GPIOE,1

/* Analog inputs */
#define AIN_VBAT   GPIOA,1

/* Channel selection rotary encoder */
#define CH_SELECTOR_0 GPIOE,14
#define CH_SELECTOR_1 GPIOB,11

/* Push-to-talk switch */
#define PTT_SW GPIOE,11

/*
 * Keyboard. Here we define only rows, since coloumn lines are the same as
 * LCD_Dx. See also: https://www.qsl.net/dl4yhf/RT3/md380_hw.html#keyboard
 *
 * "Monitor" and "Function" buttons, on the other hand, are connected to
 * keyboard row 3 and on LCD_D6 and LCD_D7. They are SWAPPED with respect to
 * connections made on MD-380.
 */
#define KB_ROW1 GPIOA,6 /* K1 */
#define KB_ROW2 GPIOD,2 /* K2 */
#define KB_ROW3 GPIOD,3 /* K3 */
#define MONI_SW LCD_D6
#define FUNC_SW LCD_D7

/* External flash */
#define FLASH_CS  GPIOD,7
#define FLASH_CLK GPIOB,3
#define FLASH_SDO GPIOB,4
#define FLASH_SDI GPIOB,5

/*
 * To enable pwm for display backlight dimming uncomment this directive.
 *
 * WARNING: backlight pwm is disabled by default because it generates a
 * continuous tone in the speaker and headphones.
 *
 * This issue cannot be solved in any way because it derives from how the
 * MD-UV380 mcu pins are used: to have a noiseless backlight pwm, the control
 * pin has to be connected to a mcu pin having between its alternate functions
 * an output compare channel of one of the timers. With this configuration, the
 * pwm signal can completely generated in hardware and its frequency can be well
 * above 22kHz, which is the upper limit for human ears.
 *
 * In the MD-UV380 radio, display backlight is connected to PD8, which is not
 * connected to any of the available output compare channels. Thus, the pwm
 * signal generation is managed inside the TIM11 ISR by toggling the backlight
 * pin and its frequency has to be low (~250Hz) to not put too much overehad on
 * the processor due to timer ISR triggering at an high rate.
 *
 * #define ENABLE_BKLIGHT_DIMMING
 */

#endif
