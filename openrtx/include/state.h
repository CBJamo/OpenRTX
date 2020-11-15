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

#ifndef STATE_H
#define STATE_H

#include <datatypes.h>

/**
 * Part of this structure has been commented because the corresponding
 * functionality is not yet implemented.
 * Uncomment once the related feature is ready
 */

typedef struct state_t {
    //enum ui_screen;
    //enum tuner_mode;
    //enum radio_mode;
    
    //time_t rx_status_tv;
    //bool rx_status;
    
    //time_t tx_status_tv;
    //bool tx_status;
    
    freq_t rx_freq;
    freq_t tx_freq;
    
    //float tx_power;
    
    //uint8_t squelch;
    
    //tone_t rx_tone;
    //tone_t tx_tone;
    
    //ch_t *channel;
    
//#ifdef DMR_ENABLED
    //uint8_t dmr_color;
    //uint8_t dmr_timeslot;
    //dmr_contact_t *dmr_contact;
//#endif
} state_t;

/**
 * This function initialises the Radio state, acquiring the information
 * needed to populate it from device drivers. 
 */
void state_init();

/**
 * This function updates the state information by sourcing the
 * updated values of the various fields of the state_t struct
 * from corresponding device drivers.
 */
void state_update();

/**
 * Fetch current state.
 * @return current state.
 */
state_t state_getCurrentState();

/**
 * This function terminates the Radio state.
 */
void state_terminate();

#endif /* STATE_H */
