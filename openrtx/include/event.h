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

#ifndef EVENT_H
#define EVENT_H

/**
 * This enum describes the event message type:
 * - EVENT_KBD is used to send a keypress
 * - EVENT_STATUS is used to send a status change notification
 */
enum eventType_t
{
    EVENT_KBD = 0,
    EVENT_STATUS = 1
};

/**
 * The event message is constrained to 32 bits size
 * This is necessary to send event messages in uC OS/III Queues
 * That accept a void * type message, which is 32-bits wide on
 * ARM cortex-M MCUs
 * uC OS/III Queues are meant to send pointer to allocated data
 * But if we keep the size under 32 bits, we can sent the
 * entire message, casting it to a void * pointer.
 */
typedef struct
{
    uint32_t type    : 3;
    uint32_t payload : 29;
}event_t;

#endif /* EVENT_H */
