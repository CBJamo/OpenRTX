/***************************************************************************
 *   Copyright (C) 2020 by Federico Amedeo Izzo IU2NUO,                    *
 *                         Niccolò Izzo IU2KIN                             *
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

/* Defines for GPIO control, really ugly but useful. */
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

#define LCD_BKLIGHT GPIOC,6

#define GREEN_LED  GPIOE,0
#define RED_LED    GPIOE,1

#define AIN_VOLUME GPIOA,0
#define AIN_VBAT   GPIOA,1
#define AIN_MIC    GPIOA,3
#define AIN_RSSI   GPIOB,0

#endif
