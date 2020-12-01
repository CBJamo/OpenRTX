/***************************************************************************
 *   Copyright (C) 2020 by Silvano Seva IU2KWO                             *
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

#include "MK22F51212.h"
#include "gpio.h"

/*
 * MK22 GPIO management is a bit convoluted: instead of having all the registers
 * needed for GPIO configuration in one single peripheral (like ST does), we have
 * GPIO peripheral for input/output direction selection, output control and
 * input reading, while alternate function, pull up/down and other stuff is
 * managed by the PORT peripheral. WHY?!
 * To overcome this while keeping the GPIO driver interface standard, we have to
 * create a function that, given the GPIO register, gives back the corresponding
 * PORT one. Ah, and, of course, this cannot be made using simple pointer
 * addition/subtraction/multiplication...
 */
PORT_Type *getPortFromGPio(const GPIO_Type *gpio)
{
    PORT_Type *port;

    switch(((unsigned int) gpio))
    {
        case ((unsigned int) GPIOA):
            port = PORTA;
            break;

        case ((unsigned int) GPIOB):
            port = PORTB;
            break;

        case ((unsigned int) GPIOC):
            port = PORTC;
            break;

        case ((unsigned int) GPIOD):
            port = PORTD;
            break;

        case ((unsigned int) GPIOE):
            port = PORTE;
            break;

        default:
            port = 0;
            break;
    }

    return port;
}

void gpio_setMode(void *port, uint8_t pin, enum Mode mode)
{
    GPIO_Type *g = (GPIO_Type *)(port);
    PORT_Type *p = getPortFromGPio(g);

    /* Clear previous settings */
    g->PDDR &= ~(1 << pin);
    p->PCR[pin] = 0;

    switch(mode)
    {
        case INPUT:
            p->PCR[pin] |= PORT_PCR_MUX(1);   /* Enable pin in GPIO mode */
            g->PDDR     &= ~(1 << pin);       /* Input mode              */
            break;

        case INPUT_PULL_UP:
            p->PCR[pin] |= PORT_PCR_MUX(1);   /* Enable pin in GPIO mode */
            p->PCR[pin] |= PORT_PCR_PS(1);    /* Pull up mode            */
            p->PCR[pin] |= PORT_PCR_PE(1);    /* Pull up/down enable     */
            g->PDDR     &= ~(1 << pin);       /* Input mode              */
            break;

        case INPUT_PULL_DOWN:
            p->PCR[pin] |= PORT_PCR_MUX(1);   /* Enable pin in GPIO mode */
            p->PCR[pin] &= ~PORT_PCR_PS(1);   /* Pull down mode          */
            p->PCR[pin] |= PORT_PCR_PE(1);    /* Pull up/down enable     */
            g->PDDR     &= ~(1 << pin);       /* Input mode              */

            break;

        /*
         * case INPUT_ANALOG:
         * NOTE: analog input mode unimplemented here for hardware structure
         * reasons.
         */

        case OUTPUT:
            p->PCR[pin] |= PORT_PCR_MUX(1);   /* Enable pin in GPIO mode  */
            g->PDDR     |= (1 << pin);        /* Output mode              */
            break;

        case OPEN_DRAIN:
            p->PCR[pin] |= PORT_PCR_MUX(1);   /* Enable pin in GPIO mode  */
            p->PCR[pin] |= PORT_PCR_ODE(1);   /* Enable open drain mode   */
            g->PDDR     |= (1 << pin);        /* Output mode              */
            break;

        /*
         * case ALTERNATE:
         * NOTE: alternate mode unimplemented here for hardware structure
         * reasons.
         */

        /*
         * ALTERNATE_OD:
         * NOTE: alternate open drain mode unimplemented here for hardware
         * structure reasons.
         */

        default:
            p->PCR[pin] |= PORT_PCR_MUX(1);   /* Enable pin in GPIO mode */
            g->PDDR     &= ~(1 << pin);       /* Input mode              */
            break;
    }
}

void gpio_setAlternateFunction(void *port, uint8_t pin, uint8_t afNum)
{
    GPIO_Type *g = (GPIO_Type *)(port);
    PORT_Type *p = getPortFromGPio(g);

    p->PCR[pin] &= ~PORT_PCR_MUX_MASK;      /* Clear old configuration */
    p->PCR[pin] |= PORT_PCR_MUX(2 + afNum); /* Set new AF, range 0 - 7 */
}

void gpio_setOutputSpeed(void *port, uint8_t pin, enum Speed spd)
{
    GPIO_Type *g = (GPIO_Type *)(port);
    PORT_Type *p = getPortFromGPio(g);

    if(spd >= FAST)
    {
        p->PCR[pin] |= PORT_PCR_SRE(1);
    }
    else
    {
        p->PCR[pin] &= ~PORT_PCR_SRE(1);
    }
}

void gpio_setPin(void *port, uint8_t pin)
{
    ((GPIO_Type *)(port))->PSOR = (1 << pin);
}

void gpio_clearPin(void *port, uint8_t pin)
{
    ((GPIO_Type *)(port))->PCOR = (1 << pin);
}

void gpio_togglePin(void *port, uint8_t pin)
{
    ((GPIO_Type *)(port))->PTOR ^= (1 << pin);
}

uint8_t gpio_readPin(const void *port, uint8_t pin)
{
    GPIO_Type *g = (GPIO_Type *)(port);
    return ((g->PDIR & (1 << pin)) != 0) ? 1 : 0;
}
