#pragma once

#include "vec.h"

void SetBuffer(void* buffer);

void DrawPoint(const Vec2Int* p,
    const int r, const int g, const int b);

void DrawLine(
    const Vec2Int* p0, const Vec2Int* p1,
    const int r, const int g, const int b);

void DrawTriangleWireframe(
    const Vec2Int* p0, const Vec2Int* p1, const Vec2Int* p2,
    const int r, const int g, const int b);

void DrawTriangleSolid(
    const Vec2Int* p0, const Vec2Int* p1, const Vec2Int* p2,
    const int r, const int g, const int b);

void DrawTriangleShaded(
    const Vec2Int* p0, const Vec2Int* p1, const Vec2Int* p2,
    double h0, double h1, double h2,
    const int r, const int g, const int b);

Vec2Int ViewportToCanvas(const Vec2Double* p);

Vec2Int ProjectVertex(const Vec3* p);
