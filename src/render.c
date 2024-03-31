#include "render.h"

#include <stdint.h>

#include <SDL.h>

#include "gilberte.h"

int values[(WINDOW_WIDTH > WINDOW_HEIGHT) ? WINDOW_WIDTH : WINDOW_HEIGHT];

void Interpolate(const int i0, const int d0, const int i1, const int d1);

void DrawPoint(void* buffer,
               const Vec2Int* p,
               const int r, const int g, const int b)
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
    uint32_t* pixel = (uint32_t*)buffer + p->y * WINDOW_WIDTH + p->x;

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

void DrawLine(void* buffer,
              const Vec2Int* p0, const Vec2Int* p1,
              const int r, const int g, const int b)
{
    const Vec2Int* p0_ = p0;
    const Vec2Int* p1_ = p1;

    if (abs(p1_->x - p0_->x) > abs(p1_->y - p0_->y))
    {
        // Swap
        if (p0_->x > p1_->x)
        {
            p0_ = p1;
            p1_ = p0;
        }

        Interpolate(p0_->x, p0_->y, p1_->x, p1_->y);

        for (int x = p0_->x; x <= p1_->x; ++x)
        {
            const Vec2Int p = { .x = x, .y = values[x - p0_->x] };
            DrawPoint(buffer, &p, r, g, b);
        }
    }
    else
    {
        // Swap
        if (p0_->y > p1_->y)
        {
            p0_ = p1;
            p1_ = p0;
        }

        Interpolate(p0_->y, p0_->x, p1_->y, p1_->x);

        for (int y = p0_->y; y <= p1_->y; ++y)
        {
            const Vec2Int p = { .x = values[y - p0_->y], .y = y };
            DrawPoint(buffer, &p, r, g, b);
        }
    }
}

void Interpolate(const int i0, const int d0, const int i1, const int d1)
{
    if (i0 == i1)
    {
        values[0] = d0;
    }

    const double a = (double)(d1 - d0) / (double)(i1 - i0);
    double d = d0;
    
    for (int i = i0; i <= i1; ++i)
    {
        values[i - i0] = (int)round(d);
        d += a;
    }
}
