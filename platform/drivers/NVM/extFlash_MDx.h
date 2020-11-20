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

#ifndef EXTFLASH_MDx_H
#define EXTFLASH_MDx_H

#include <stdint.h>
#include <sys/types.h>

/**
 * Driver for external non volatile memory on MDx family devices, containing
 * both calibration and contact data.
 * For this family non volatile storage hardware is a Winbond W25Q128FV SPI flash
 * connected to SPI1 peripheral.
 */

/**
 * Initialise driver for external flash.
 */
void extFlash_init();

/**
 * Terminate driver for external flash.
 */
void extFlash_shutdown();

/**
 * Release flash chip from power down mode, this function should be called at
 * least once after the initialisation of the driver and every time after the
 * chip has been put in low power mode.
 * Application code must wait at least 3us before issuing any other command
 * after this one.
 */
void extFlash_wakeup();

/**
 * Put flash chip in low power mode.
 */
void extFlash_sleep();

/**
 * Read data from one of the flash security registers, located at addresses
 * 0x1000, 0x2000 and 0x3000 and 256-byte wide.
 * NOTE: If a read operation goes beyond the 256 byte boundary, length will be
 * truncated to the one reaching the end of the register.
 *
 * @param addr: start address for read operation, must be the full register address.
 * @param buf: pointer to a buffer where data is written to.
 * @param len: number of bytes to read.
 * @return: -1 if address is not whithin security registers address range, the
 * number of bytes effectively read otherwise.
 */
ssize_t extFlash_readSecurityRegister(uint32_t addr, uint8_t *buf, size_t len);

/**
 * Read data from flash memory.
 *
 * @param addr: start address for read operation.
 * @param buf: pointer to a buffer where data is written to.
 * @param len: number of bytes to read.
 */
void extFlash_readData(uint32_t addr, uint8_t *buf, size_t len);

#endif /* EXTFLASH_MDx_H */
