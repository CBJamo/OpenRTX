/***************************************************************************
 *   Copyright (C) 2020 by Federico Izzo IU2NUO, Niccolò Izzo IU2KIN and   *
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <os.h>
#include "gpio.h"
#include "graphics.h"
#include "hwconfig.h"
#include "platform.h"

void platform_test()
{
    OS_ERR os_err;
    point_t pos_line1 = {0, 0};
    point_t pos_line2 = {0, 9};
    point_t pos_line3 = {0, 17};
    color_t color_white = {255, 255, 255};
    char *buf = "Platform Test";
    gfx_print(pos_line1, buf, FONT_SIZE_1, TEXT_ALIGN_LEFT, color_white);
    float vBat = platform_getVbat();
    float micLevel = platform_getMicLevel();
    float volumeLevel = platform_getVolumeLevel();
    uint8_t currentCh = platform_getChSelector();
    bool ptt = platform_getPttStatus();
    char buf2[20] = "";
    snprintf(buf2, sizeof(buf2), "bat:%.2f mic:%.2f", vBat, micLevel);
    char buf3[20] = "";
    snprintf(buf3, sizeof(buf3), "vol:%.2f ch:%d ptt:%d", volumeLevel, currentCh, ptt);
    gfx_print(pos_line2, buf2, FONT_SIZE_1, TEXT_ALIGN_LEFT, color_white);
    gfx_print(pos_line3, buf3, FONT_SIZE_1, TEXT_ALIGN_LEFT, color_white);
    gfx_render();
    while(gfx_renderingInProgress());
    OSTimeDlyHMSM(0u, 0u, 1u, 0u, OS_OPT_TIME_HMSM_STRICT, &os_err);
}

int main(void)
{
    // Init the graphic stack
    gfx_init();
    platform_setBacklightLevel(255);

    point_t origin = {0, SCREEN_HEIGHT / 2};
    color_t color_yellow = {250, 180, 19};
    char *buffer = "OpenRTX";

    OS_ERR os_err;

    // Task infinite loop
    while(1)
    {
        gfx_clearScreen();
        gfx_print(origin, buffer, FONT_SIZE_4, TEXT_ALIGN_CENTER, color_yellow);
        //gfx_render();
        //while(gfx_renderingInProgress());
        platform_test();
        OSTimeDlyHMSM(0u, 0u, 0u, 100u, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}
