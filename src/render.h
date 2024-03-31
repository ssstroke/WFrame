#pragma once

#include "vec.h"

// TODO: Why pass buffer if there is only one buffer?

/*
Write RGB color to (x, y) coordinate of imaginary plane of
32-bit pixels that the `buffer` points to.
It is assumed that `r`, `g` and `b` are in [0, 255] range.
*/
void DrawPoint(void* buffer,
               const Vec2Int* p,
               const int r, const int g, const int b);

/*
TODO
*/
void DrawLine(void* buffer,
              const Vec2Int* p0, const Vec2Int* p1,
              const int r, const int g, const int b);
