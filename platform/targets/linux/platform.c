/***************************************************************************
 *   Copyright (C) 2020 by Frederik Saraci IU2NRO                          *
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

#include "platform.h"
#include "gpio.h"
#include <stdio.h>

void platform_init()
{
	printf("Platform init\n");
}

void platform_terminate()
{
	printf("Platform terminate\n");
}

void platform_setBacklightLevel(uint8_t level)
{
	printf("platform_setBacklightLevel(%u)\n", level);
}


float platform_getVbat(){
	printf("platform_getVbat()\n");
	return 0.69;
}


float platform_getMicLevel(){
	printf("platform_getMicLevel()\n");
	return 0.69;
}


float platform_getVolumeLevel(){
	printf("platform_getVolumeLevel()\n");
	return 0.69;
}


uint8_t platform_getChSelector(){
	printf("platform_getChSelector()\n");
	return 42;
}


bool platform_getPttStatus(){
	printf("platform_getVbat()\n");
	return true;
}


void platform_ledOn(led_t led){
	char* str;

	switch(led){
		case 0:
			str = "GREEN";
			break;
		case 1:
			str = "RED";
			break;
		case 2:
			str = "YELLOW";
			break;
		case 3:
			str = "WHITE";
			break;
	}

	printf("platform_ledOn(%s)\n", str);
}


void platform_ledOff(led_t led){
	printf("platform_ledOff()\n");
}


void platform_beepStart(uint16_t freq){
	printf("platform_beepStart(%u)\n", freq);
}


void platform_beepStop(){
	printf("platform_beepStop()\n");
}
