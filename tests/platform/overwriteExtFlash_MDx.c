/***************************************************************************
 *   Copyright (C) 2020 by Federico Amedeo Izzo IU2NUO,                    *
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

#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include "W25Qx.h"

uint8_t block[256] = {0};

void printChunk(void *chunk)
{
    uint8_t *ptr = ((uint8_t *) chunk);
    for(size_t i = 0; i < 16; i++) printf("%02x ", ptr[i]);
    for(size_t i = 0; i < 16; i++)
    {
        if((ptr[i] > 0x22) && (ptr[i] < 0x7f)) printf("%c", ptr[i]);
        else printf(".");
    }
}

int main()
{
    W25Qx_init();
    W25Qx_wakeup();

    while(1)
    {
        getchar();
        
        // On UV380 flash at 0x5F60 there are 36032 bytes of 0xFF
        uint32_t addr = 0x5F60;
        printf("Read memory @ 0x5F60\r\n");
        W25Qx_readData(addr, block, 256);
        for (int offset = 0; offset < 256; offset += 16)
        {
            printf("\r\n%lx: ", addr + offset);
            printChunk(block + offset);
        }
        block[3] = 0xAA;
        printf("\r\nWrite memory @ 0x5F60... ");
        bool success = W25Qx_writeData(addr, block, 16);
        printf("%s\r\n", success ? "success" : "failed");

        printf("Read memory @ 0x5F60\r\n");
        W25Qx_readData(addr, block, 256);
        for (int offset = 0; offset < 256; offset += 16)
        {
            printf("\r\n%lx: ", addr + offset);
            printChunk(block + offset);
        }
    }

    return 0;
}
