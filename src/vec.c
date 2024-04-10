#include "vec.h"

#include <math.h>

/* (a, b, c) + (d, e, f) = (a + d, b + e, c + f) */
Vec3_Double Vec3Add(const Vec3_Double* a, const Vec3_Double* b)
{
    const Vec3_Double kResult = { .x = a->x + b->x,
                           .y = a->y + b->y,
                           .z = a->z + b->z };
    return kResult;
}

/* (a, b, c) - (d, e, f) = (a - d, b - e, c - f) */
Vec3_Double Vec3Sub(const Vec3_Double* a, const Vec3_Double* b)
{
    const Vec3_Double kResult = { .x = a->x - b->x,
                           .y = a->y - b->y,
                           .z = a->z - b->z };
    return kResult;
}

/* (a, b, c) + k = (a + k, b + k, c + k) */
Vec3_Double Vec3AddScalar(const Vec3_Double* a, double k)
{
    const Vec3_Double kResult = { .x = a->x + k,
                           .y = a->y + k,
                           .z = a->z + k };
    return kResult;
}

/* (a, b, c) - k = (a - k, b - k, c - k) */
Vec3_Double Vec3SubScalar(const Vec3_Double* a, double k)
{
    const Vec3_Double kResult = { .x = a->x - k,
                           .y = a->y - k,
                           .z = a->z - k };
    return kResult;
}

/* (a, b, c) * k = (ka, kb, kc) */
Vec3_Double Vec3MulScalar(const Vec3_Double* a, double k)
{
    const Vec3_Double kResult = { .x = a->x * k,
                           .y = a->y * k,
                           .z = a->z * k };
    return kResult;
}

/* (a, b, c) / k = (a / k, b / k, c / k) */
Vec3_Double Vec3DivScalar(const Vec3_Double* a, double k)
{
    const Vec3_Double kResult = { .x = a->x / k,
                           .y = a->y / k,
                           .z = a->z / k };
    return kResult;
}

/* sqrt(x * x + y * y + z * z) */
double Vec3Length(const Vec3_Double* a)
{
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

/* x * x + y * y + z * z */
double Vec3LengthSquared(const Vec3_Double* a)
{
    return a->x * a->x + a->y * a->y + a->z * a->z;
}

/* a.x * b.x + a.y * b.y + a.z * b.z */
double Vec3DotProduct(const Vec3_Double* a, const Vec3_Double* b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/*
a.y * b.z - a.z * b.y,
a.z * b.x - a.x * b.z,
a.x * b.y - a.y * b.x
*/
Vec3_Double Vec3CrossProduct(const Vec3_Double* a, const Vec3_Double* b)
{
    const Vec3_Double kResult = { .x = a->y * b->z - a->z * b->y,
                           .y = a->z * b->x - a->x * b->z,
                           .z = a->x * b->y - a->y * b->x };
    return kResult;
}

/* ( a.x / length(a), a.y / length(a), a.z / length(a) ) */
Vec3_Double Vec3Normalize(const Vec3_Double* a)
{
    const double len_inverse = 1.0 / Vec3Length(a);
    const Vec3_Double kResult = { .x = a->x * len_inverse,
                           .y = a->y * len_inverse,
                           .z = a->z * len_inverse };
    return kResult;
}
