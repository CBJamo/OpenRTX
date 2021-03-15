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

#ifndef HRC6000_H
#define HRC6000_H

#include <stdint.h>
#include <stdbool.h>

#include <calibInfo_GDx.h>

/**
 * Initialise the HR_C6000 driver.
 */
void C6000_init();

/**
 * Terminate the HR_C6000 driver.
 */
void C6000_terminate();

/**
 * Set value for two-point modulation offset adjustment. This value usually is
 * stored in radio calibration data.
 * @param offset: value for modulation offset adjustment.
 */
void C6000_setModOffset(uint16_t offset);

/**
 * Set values for two-point modulation amplitude adjustment. These values
 * usually are stored in radio calibration data.
 * @param iMag: value for modulation offset adjustment.
 */
void C6000_setModAmplitude(uint8_t iAmp, uint8_t qAmp);

/**
 *
 */
void C6000_setMod2Bias(uint8_t bias);

/**
 * Set value for FM-mode modulation factor, a value dependent on bandwidth.
 * @param mf: value for FM modulation factor.
 */
void C6000_setModFactor(uint8_t mf);

/**
 * Configure the gain of lineout DAC stage. Allowed range is 1 - 31 and each
 * step corresponds to a variation of 1.5dB.
 * @param value: gain for the DAC stage.
 */
void C6000_setDacGain(uint8_t value);

/**
 * Configure chipset for DMR operation.
 */
void C6000_dmrMode();

/**
 * Configure chipset for analog FM operation.
 */
void C6000_fmMode();

/**
 * Start analog FM transmission.
 */
void C6000_startAnalogTx();

/**
 * Stop analog FM transmission.
 */
void C6000_stopAnalogTx();

/**
 * Check if SPI common to HR_C6000 and PLL is in use by this driver.
 * @return true if SPI lines are being used by this driver.
 */
bool C6000_spiInUse();

#endif /* HRC6000_H */
