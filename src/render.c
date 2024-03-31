// TODO: Why pass buffer if there is only one buffer?
// TODO: Every call to `Interpolate()` calls `malloc()` which seems expensive.

#include "render.h"

#include <stdbool.h>
#include <stdint.h>

#include <SDL.h>

#include "gilberte.h"

void Interpolate(const int i0, const int d0, const int i1, const int d1,
                 int* values);

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
        // Horizontal-ish line

        if (p0_->x > p1_->x)
        {
            p0_ = p1;
            p1_ = p0;
        }

        int values[WINDOW_WIDTH];

        Interpolate(p0_->x, p0_->y, p1_->x, p1_->y, values);
        for (int x = p0_->x; x <= p1_->x; ++x)
        {
            const Vec2Int p = { .x = x, .y = values[x - p0_->x] };
            DrawPoint(buffer, &p, r, g, b);
        }
    }
    else
    {
        // Vertical-ish line

        if (p0_->y > p1_->y)
        {
            p0_ = p1;
            p1_ = p0;
        }

        int values[WINDOW_HEIGHT];

        Interpolate(p0_->y, p0_->x, p1_->y, p1_->x, values);
        for (int y = p0_->y; y <= p1_->y; ++y)
        {
            const Vec2Int p = { .x = values[y - p0_->y], .y = y };
            DrawPoint(buffer, &p, r, g, b);
        }
    }
}

void DrawTriangleWireframe(void* buffer,
                           const Vec2Int* p0, const Vec2Int* p1, const Vec2Int* p2,
                           const int r, const int g, const int b)
{
    DrawLine(buffer, p0, p1, r, g, b);
    DrawLine(buffer, p1, p2, r, g, b);
    DrawLine(buffer, p2, p0, r, g, b);
}

void DrawTriangleFilled(void* buffer,
                        const Vec2Int* p0, const Vec2Int* p1, const Vec2Int* p2,
                        const int r, const int g, const int b)
{
    Vec2Int* p0_ = p0;
    Vec2Int* p1_ = p1;
    Vec2Int* p2_ = p2;

    {
        if (p1_->y < p0_->y)
        {
            // swap p1 and p0
            p0_ = p1;
            p1_ = p0;
        }
        if (p2_->y < p0_->y)
        {
            // swap p2 and p0
            const Vec2Int* temp = p0_;
            p0_ = p2_;
            p2_ = temp;
        }
        if (p2_->y < p1_->y)
        {
            // swap p2 and p1
            const Vec2Int* temp = p1_;
            p1_ = p2_;
            p2_ = temp;
        }
    }
    
    {
        int x012[WINDOW_WIDTH];
        int x02[WINDOW_WIDTH]; // it is the "tall" side

        // p1_->y - 1 (i.e. the last point) becase this last point will be x12's first point
        Interpolate(p0_->y, p0_->x, p1_->y, p1_->x, x012);
        Interpolate(p1_->y, p1_->x, p2_->y, p2_->x, x012 + (p1_->y - p0_->y));
        Interpolate(p0_->y, p0_->x, p2_->y, p2_->x, x02);

        int* x_left;
        int* x_right;
        
        const int m = floor((double)(p2_->y - p0_->y) / 2.0);
        if (x02[m] < x012[m])
        {
            x_left = x02;
            x_right = x012;
        }
        else
        {
            x_left = x012;
            x_right = x02;
        }

        for (int y = p0_->y; y <= p2_->y; ++y)
        {
            for (int x = x_left[y - p0_->y]; x <= x_right[y - p0_->y]; ++x)
            {
                const Vec2Int p = { .x = x, .y = y };
                DrawPoint(buffer, &p, r, g, b);
            }
        }
    }
}

// `i` stands for independent
// `d` stands for   dependent
// we compute d = f(i)
void Interpolate(const int i0, const int d0, const int i1, const int d1,
                 int* values)
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
