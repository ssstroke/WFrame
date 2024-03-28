#include "render.h"

#include <stdint.h>

#include <SDL.h>

#include "gilberte.h"

void WriteColor(void* buffer, int x, int y, int r, int g, int b)
{
    /*
    * For every `y` we skip (y * WINDOW_WIDTH) pixels.
    * In other words, moving one column means moving `WINDOW_WIDTH` pixels.
    * See drawing below:
    * 
    * x = 5, y = 3
    *
    * ----------------
    * ----------------
    * ----------------
    * -----*----------
    * ----------------
    * ----------------
    * ----------------
    * 
    * `WINDOW_WIDTH` is defined in `gilberte.h` in order to avoid passing
    * one extra parameter to a function that will never change.
    */
    uint32_t* pixel = (uint32_t*)buffer + y * WINDOW_WIDTH + x;

    /* There is `SDL_MapRGBA` for this but I would like to reinvent the wheel ;) */
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    { // 4321
        /*
        * 0000 0000     0000 0000     0000 0000     0000 0000
        * { unused }    { b_channel } { g_channel } { r_channel }
        */
        *pixel = (r << 24) | (g << 16) | (b << 8) | 0xFF;
    }
    else
    { // 1234
        /*
        * 0000 0000     0000 0000     0000 0000     0000 0000
        * { r_channel } { g_channel } { b_channel } { unused }
        */
        *pixel = (r) | (g << 8) | (b << 16) | (0xFF << 24);
    }
}
