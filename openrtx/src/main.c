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

#include <os.h>
#include <ui.h>
#include <stdlib.h>
#include <inttypes.h>
#include <threads.h>
#include <platform.h>
#include <battery.h>
#include <graphics.h>
#include <hwconfig.h>

/* If battery level is below 0% draw low battery screen, wait and shutdown */
void check_battery() {
    OS_ERR os_err;

    // Check battery percentage
    float vbat = platform_getVbat();
    float charge = battery_getCharge(vbat);

    // Draw low battery screen
    if (charge <= 0) {
        gfx_clearScreen();
        uint16_t bat_width = SCREEN_WIDTH / 2;
        uint16_t bat_height = SCREEN_HEIGHT / 3;
        point_t bat_pos = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6};
        gfx_drawBattery(bat_pos, bat_width, bat_height, 0.1f);
        point_t text_pos_1 = {0, SCREEN_HEIGHT * 3 / 4};
        point_t text_pos_2 = {0, SCREEN_HEIGHT * 3 / 4 + 16};
        const color_t color_white = {255, 255, 255};

        gfx_print(text_pos_1,
                  "For emergency operation",
                  FONT_SIZE_6PT,
                  TEXT_ALIGN_CENTER,
                  color_white);
        gfx_print(text_pos_2,
                  "press any button.",
                  FONT_SIZE_6PT,
                  TEXT_ALIGN_CENTER,
                  color_white);
        gfx_render();

        // Wait 5 seconds
        OSTimeDlyHMSM(0u, 0u, 5u, 0u, OS_OPT_TIME_HMSM_STRICT, &os_err);

        // TODO: Shut down radio unless a button is pressed
    }
}

int main(void)
{
    OS_ERR os_err;

    // Initialize platform drivers
    platform_init();

    // Initialize graphics driver
    gfx_init();

    // Initialize user interface
    ui_init();

    // Check if battery has enough charge to operate
    check_battery();

    // Display splash screen
    ui_drawSplashScreen();
    gfx_render();
    while(gfx_renderingInProgress());

    // Wait 30ms before turning on backlight to hide random pixels on screen
    OSTimeDlyHMSM(0u, 0u, 0u, 30u, OS_OPT_TIME_HMSM_STRICT, &os_err);
    platform_setBacklightLevel(255);

    // Keep the splash screen for 1 second
    OSTimeDlyHMSM(0u, 0u, 1u, 0u, OS_OPT_TIME_HMSM_STRICT, &os_err);

    // Create OpenRTX threads
    create_threads();

    // Auxiliary functions loop
    while(true) {
        check_battery();
        OSTimeDlyHMSM(0u, 1u, 0u, 0u, OS_OPT_TIME_HMSM_STRICT, &os_err);
    }
}
