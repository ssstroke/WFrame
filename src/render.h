#pragma once

#include "vec.h"

/*
Write RGB color to (x, y) coordinate of imaginary plane of
32-bit pixels that the `buffer` points to.
It is assumed that `r`, `g` and `b` are in [0, 255] range.
*/
void DrawPoint(void* buffer,
    const Vec2Int* p,
    const int r, const int g, const int b);

/*
Draw points of specified color between `p0` and `p1`.
*/
void DrawLine(void* buffer,
    const Vec2Int* p0, const Vec2Int* p1,
    const int r, const int g, const int b);

/*
Draw the outline of the triangle specified by 3 points.
Internally this functions just calls `DrawLine()`
3 times for each side of the triangle.
*/
void DrawTriangleWireframe(void* buffer,
    const Vec2Int* p0, const Vec2Int* p1, const Vec2Int* p2,
    const int r, const int g, const int b);

void DrawTriangleSolid(void* buffer,
    const Vec2Int* p0, const Vec2Int* p1, const Vec2Int* p2,
    const int r, const int g, const int b);

void DrawTriangleShaded(void* buffer,
    const Vec2Int* p0, const Vec2Int* p1, const Vec2Int* p2,
    double h0, double h1, double h2,
    const int r, const int g, const int b);
