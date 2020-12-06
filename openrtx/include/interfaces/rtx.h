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

#include <stdint.h>
#include <datatypes.h>

enum funcmode
{
    OFF,
    RX,
    TX
};

enum tone
{
    NONE,
    CTCSS_67_0,
    CTCSS_71_9,
    CTCSS_81_5
};

enum bw
{
    BW_12_5,
    BW_25
};

enum opmode
{
    FM,
    DMR
};

void rtx_init();

void rtx_terminate();

void rtx_setTxFreq(freq_t freq);

void rtx_setRxFreq(freq_t freq);

void rtx_setFuncmode(enum funcmode mode);

void rtx_setToneRx(enum tone t);

void rtx_setToneTx(enum tone t);

void rtx_setBandwidth(enum bw b);

float rtx_getRssi();

void rtx_setOpmode(enum opmode mode);

#endif /* RTX_H */
