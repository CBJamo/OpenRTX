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

#ifndef RTX_H
#define RTX_H

#include <datatypes.h>
#include <stdint.h>
#include <cps.h>
#include <pthread.h>

typedef struct
{
    uint8_t opMode    : 2,  /**< Operating mode (FM, DMR, ...) */
            bandwidth : 2,  /**< Channel bandwidth             */
            txDisable : 1,  /**< Disable TX operation          */
            scan      : 1,  /**< Scan enabled                  */
            opStatus  : 2;  /**< Operating status (OFF, ...)   */

    freq_t rxFrequency;     /**< RX frequency, in Hz           */
    freq_t txFrequency;     /**< TX frequency, in Hz           */

    float txPower;          /**< TX power, in W                */
    uint8_t sqlLevel;       /**< Squelch opening level         */

    uint16_t rxToneEn : 1,  /**< RX CTC/DCS tone enable        */
             rxTone   : 15; /**< RX CTC/DCS tone               */

    uint16_t txToneEn : 1,  /**< TX CTC/DCS tone enable        */
             txTone   : 15; /**< TX CTC/DCS tone               */
}
__attribute__((packed)) rtxStatus_t;

/**
 * \enum bandwidth Enumeration type defining the current rtx bandwidth.
 */
enum bandwidth
{
    BW_12_5 = 0,    /**< 12.5kHz bandwidth */
    BW_20   = 1,    /**< 20kHz bandwidth   */
    BW_25   = 2     /**< 25kHz bandwidth   */
};

/**
 * \enum opmode Enumeration type defining the current rtx operating mode.
 */
enum opmode
{
    FM  = 0,        /**< Analog FM */
    DMR = 1         /**< DMR       */
};

/**
 * \enum opstatus Enumeration type defining the current rtx operating status.
 */
enum opstatus
{
    OFF = 0,        /**< OFF          */
    RX  = 1,        /**< Receiving    */
    TX  = 2         /**< Transmitting */
};


/**
 * Initialise rtx stage.
 * @param m: pointer to the mutex protecting the shared configuration data
 * structure.
 */
void rtx_init(pthread_mutex_t *m);

/**
 * Shut down rtx stage
 */
void rtx_terminate();

/**
 * Post a new RTX configuration on the internal message queue. Data structure
 * \b must be protected by the same mutex whose pointer has been passed as a
 * parameter to rtx_init(). This driver only copies its content into the internal
 * data structure, eventual garbage collection has to be performed by caller.
 * @param cfg: pointer to a structure containing the new RTX configuration.
 */
void rtx_configure(const rtxStatus_t *cfg);

/**
 * Obtain a copy of the RTX driver's internal status data structure.
 * @return copy of the RTX driver's internal status data structure.
 */
rtxStatus_t rtx_getCurrentStatus();

/**
 * High-level code is in charge of calling this function periodically, since it
 * contains all the RTX management functionalities.
 */
void rtx_taskFunc();

/**
 * Get current RSSI in dBm.
 * @return RSSI value in dBm.
 */
float rtx_getRssi();

#endif /* RTX_H */
