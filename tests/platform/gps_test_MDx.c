/***************************************************************************
 *   Copyright (C) 2021 by Federico Amedeo Izzo IU2NUO,                    *
 *                         Niccolò Izzo IU2KIN,                            *
 *                         Frederik Saraci IU2NRO,                         *
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
#include <interfaces/delays.h>
#include <interfaces/gpio.h>
#include <stdint.h>
#include <stdio.h>
#include <hwconfig.h>
#include <minmea.h>

#define GPS_EN   GPIOD,8
#define GPS_DATA GPIOD,9

char line[MINMEA_MAX_LENGTH*10];

int main()
{
    platform_init();

    gpio_setMode(GPS_EN,   OUTPUT);
    gpio_setMode(GPS_DATA, INPUT_PULL_DOWN);

    gpio_setPin(GPS_EN);

    uint8_t elapsedTime = 0;

    printf("Waiting for GPS_DATA to rise");
    while((gpio_readPin(GPS_DATA) == 0) && (elapsedTime < 20))
    {
        printf(".");
        elapsedTime += 1;
        delayMs(500);
    }

    printf(" %s.\r\n", elapsedTime < 20 ? "OK" : "TIMEOUT");

    gpio_setMode(GPS_DATA, ALTERNATE);
    gpio_setAlternateFunction(GPS_DATA, 7);

    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    __DSB();

    const unsigned int quot = 2*42000000/9600;
    USART3->BRR = quot/2 + (quot & 1);
    USART3->CR3 |= USART_CR3_ONEBIT;
    USART3->CR1 = USART_CR1_UE
                | USART_CR1_RE;

    int i = 0;
    while(1)
    {
        while((USART3->SR & USART_SR_RXNE) == 0) ;
        line[i++] = USART3->DR;
        // If a NMEA sentence is complete
        if (line[i - 1] == '\n') {
            line[i] = '\0';
            printf("%s\n\r", line);
            switch (minmea_sentence_id(line, false)) {
                case MINMEA_SENTENCE_RMC:
                {
                    struct minmea_sentence_rmc frame;
                    if (minmea_parse_rmc(&frame, line)) {
                        printf("$RMC: raw coordinates and speed: (%d/%d,%d/%d) %d/%d\n\r",
                                frame.latitude.value, frame.latitude.scale,
                                frame.longitude.value, frame.longitude.scale,
                                frame.speed.value, frame.speed.scale);
                        printf("$RMC fixed-point coordinates and speed scaled to three decimal places: (%d,%d) %d\n\r",
                                minmea_rescale(&frame.latitude, 1000),
                                minmea_rescale(&frame.longitude, 1000),
                                minmea_rescale(&frame.speed, 1000));
                        printf("$RMC floating point degree coordinates and speed: (%f,%f) %f\n\r",
                                minmea_tocoord(&frame.latitude),
                                minmea_tocoord(&frame.longitude),
                                minmea_tofloat(&frame.speed));
                    }
                } break;

                case MINMEA_SENTENCE_GGA:
                {
                    struct minmea_sentence_gga frame;
                    if (minmea_parse_gga(&frame, line)) {
                        printf("$GGA: fix quality: %d\n\r", frame.fix_quality);
                    }
                } break;

                case MINMEA_SENTENCE_GSV:
                {
                    struct minmea_sentence_gsv frame;
                    if (minmea_parse_gsv(&frame, line)) {
                        printf("$GSV: message %d of %d\n\r", frame.msg_nr, frame.total_msgs);
                        printf("$GSV: satellites in view: %d\n\r", frame.total_sats);
                        for (int i = 0; i < 4; i++)
                            printf("$GSV: sat nr %d, elevation: %d, azimuth: %d, snr: %d dbm\n\r",
                                frame.sats[i].nr,
                                frame.sats[i].elevation,
                                frame.sats[i].azimuth,
                                frame.sats[i].snr);
                    }
                } break;

                case MINMEA_SENTENCE_VTG:
                {

                } break;

                // Ignore this message as we take data from RMC
                case MINMEA_SENTENCE_GLL:
                    ;
                break;

                // These messages are never sent by the Jumpstar JS-M710 Module
                case MINMEA_SENTENCE_GSA: break;
                case MINMEA_SENTENCE_GST: break;
                case MINMEA_SENTENCE_ZDA: break;

                // Error handling
                case MINMEA_INVALID:
                {
                    printf("Error: Invalid NMEA sentence!\n\r");
                } break;

                case MINMEA_UNKNOWN:
                {
                    printf("Error: Unsupported NMEA sentence!\n\r");
                } break;
            }
            i = 0;
        }
    }

    return 0;
}
