/***************************************************************************
 *   Copyright (C) 2020 by Federico Izzo IU2NUO, Niccolò Izzo IU2KIN and   *
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

/**
 * This driver provides an lcd screen emulator to allow UI development and
 * testing on a x86/x64 computer.
 * Graphics control is provided through SDL2 library, you need to have the SDL2
 * development library installed on your machine to compile and run code using
 * this driver.
 */

#include "display.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#undef main            /* necessary to avoid conflicts with SDL_main */

/* 
 * Screen dimensions, adjust basing on the size of the screen you need to
 * emulate
 */
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 160
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 128
#endif

#if !defined(PIX_FMT_BW) && !defined(PIX_FMT_GRAYSC) && !defined(PIX_FMT_RGB565)
#warning No pixel format defined, defaulting to RGB565
#define PIX_FMT_RGB565
#endif
 
SDL_Window  *window;        /* SDL window             */
SDL_Surface *renderSurface; /* SDL rendering surface  */
void *frameBuffer;          /* Pointer to framebuffer */
bool inProgress;            /* Flag to signal when rendering is in progress */

/**
 * @internal
 * Internal helper function which fetches pixel at position (x, y) from framebuffer
 * and returns it in SDL-compatible format, which is ARGB8888.
 */
uint32_t fetchPixelFromFb(unsigned int x, unsigned int y)
{
    uint32_t pixel = 0;

    #ifdef PIX_FMT_BW
    /*
     * Black and white 1bpp format: framebuffer is an array of uint8_t, where
     * each cell contains the values of eight pixels, one per bit.
     */
    uint8_t *fb = (uint8_t *)(frameBuffer);
    unsigned int cell = (x + y*SCREEN_WIDTH) / 8;
    unsigned int elem = (x + y*SCREEN_WIDTH) % 8;
    if(fb[cell] & (1 << elem)) pixel = 0xFFFFFFFF;
    #endif

    #ifdef PIX_FMT_GRAYSC
    /*
     * Convert from 8bpp grayscale to ARGB8888, we have to do nothing more that 
     * replicating the pixel value for the three components
     */
    uint8_t *fb = (uint8_t *)(frameBuffer);
    uint8_t px = fb[x + y*SCREEN_WIDTH];

    pixel = 0xFF000000 | (px << 16) | (px << 8) | px;
    #endif

    #ifdef PIX_FMT_RGB565
    /*
     * SDL pixel format is ARGB8888, while ours is RGB565, thus we need
     * to do some conversions when writing framebuffer content to the
     * window. We also set alpha value to its maximum.
     */
    uint16_t *fb = (uint16_t *)(frameBuffer);

    uint32_t r = (fb[x + y*SCREEN_WIDTH] & 0xF800) >> 11;
    uint32_t g = (fb[x + y*SCREEN_WIDTH] & 0x07E0) >> 5;
    uint32_t b = (fb[x + y*SCREEN_WIDTH] & 0x001F) & 0x1F;

    /*
     * Here we do conversions by multiplying by some scaling factors,
     * we use ints just because the precision of floats is not really
     * needed.
     * Conversion factors:
     * - five bit to eight bit: 8.226
     * - six bit to eight bit: 4.0476
     */
    r = (r * 8) + (r * 226)/1000;
    g = (g * 4) + (g * 476)/10000;
    b = (b * 8) + (b * 226)/1000;

    pixel = 0xFF000000 | (r << 16) | (g << 8) | b;
    #endif

    return pixel;
}


void display_init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL video init error!!\n");

    }
    else
    {

        window = SDL_CreateWindow(" ",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH,SCREEN_HEIGHT,
                                  SDL_WINDOW_SHOWN);

        renderSurface = SDL_GetWindowSurface(window);
        SDL_FillRect(renderSurface,NULL,0xFFFFFF);

        /*
         * Black and white pixel format: framebuffer type is uint8_t where each
         * bit represents a pixel. We have to allocate
         * (SCREEN_HEIGHT * SCREEN_WIDTH)/8 elements
         */
        #ifdef PIX_FMT_BW
        unsigned int fbSize = (SCREEN_HEIGHT * SCREEN_WIDTH)/8;
        if((fbSize * 8) < (SCREEN_HEIGHT * SCREEN_WIDTH)) fbSize += 1; /* Compensate for eventual truncation error in division */
        fbSize *= sizeof(uint8_t);
        #endif

        /*
         * Grayscale pixel format: framebuffer type is uint8_t where each element
         * controls one pixel
         */
        #ifdef PIX_FMT_GRAYSC
        unsigned int fbSize = SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint8_t);
        #endif

        /*
         * RGB565 pixel format: framebuffer type is uint16_t where each element
         * controls one pixel
         */
        #ifdef PIX_FMT_RGB565
        unsigned int fbSize = SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint16_t);
        #endif

        frameBuffer = malloc(fbSize);
        memset(frameBuffer, 0xFFFF, fbSize);
        inProgress = false;
    }
}

void display_terminate()
{
    while(inProgress) { }         /* Wait until current render finishes */
    printf("Terminating SDL display emulator, goodbye!\n");
    free(frameBuffer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void display_renderRows(uint8_t startRow, uint8_t endRow)
{
    Uint32 *pixels = (Uint32*)renderSurface->pixels;
    inProgress = true;

    for(unsigned int x = 0; x < SCREEN_WIDTH; x++)
    {
        for(unsigned int y = startRow; y < endRow; y++)
        {
            pixels[x + y*SCREEN_WIDTH] = fetchPixelFromFb(x, y);
        }
    }

    inProgress = false;
    SDL_UpdateWindowSurface(window);
}

void display_render()
{
    display_renderRows(0, SCREEN_HEIGHT);
}

bool display_renderingInProgress()
{
    return inProgress;
}

void *display_getFrameBuffer()
{
    return (void *)(frameBuffer);
}
