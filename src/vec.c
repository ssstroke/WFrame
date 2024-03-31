#include "vec.h"

#include <math.h>

/* (a, b, c) + (d, e, f) = (a + d, b + e, c + f) */
Vec3 Vec3Add(const Vec3* a, const Vec3* b)
{
    const Vec3 kResult = { .x = a->x + b->x,
                           .y = a->y + b->y,
                           .z = a->z + b->z };
    return kResult;
}

/* (a, b, c) - (d, e, f) = (a - d, b - e, c - f) */
Vec3 Vec3Sub(const Vec3* a, const Vec3* b)
{
    const Vec3 kResult = { .x = a->x - b->x,
                           .y = a->y - b->y,
                           .z = a->z - b->z };
    return kResult;
}

/* (a, b, c) + k = (a + k, b + k, c + k) */
Vec3 Vec3AddScalar(const Vec3* a, double k)
{
    const Vec3 kResult = { .x = a->x + k,
                           .y = a->y + k,
                           .z = a->z + k };
    return kResult;
}

/* (a, b, c) - k = (a - k, b - k, c - k) */
Vec3 Vec3SubScalar(const Vec3* a, double k)
{
    const Vec3 kResult = { .x = a->x - k,
                           .y = a->y - k,
                           .z = a->z - k };
    return kResult;
}

/* (a, b, c) * k = (ka, kb, kc) */
Vec3 Vec3MulScalar(const Vec3* a, double k)
{
    const Vec3 kResult = { .x = a->x * k,
                           .y = a->y * k,
                           .z = a->z * k };
    return kResult;
}

/* (a, b, c) / k = (a / k, b / k, c / k) */
Vec3 Vec3DivScalar(const Vec3* a, double k)
{
    const Vec3 kResult = { .x = a->x / k,
                           .y = a->y / k,
                           .z = a->z / k };
    return kResult;
}

/* sqrt(x * x + y * y + z * z) */
double Vec3Length(const Vec3* a)
{
    return sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
}

/* x * x + y * y + z * z */
double Vec3LengthSquared(const Vec3* a)
{
    return a->x * a->x + a->y * a->y + a->z * a->z;
}

/* a.x * b.x + a.y * b.y + a.z * b.z */
double Vec3DotProduct(const Vec3* a, const Vec3* b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

/*
a.y * b.z - a.z * b.y,
a.z * b.x - a.x * b.z,
a.x * b.y - a.y * b.x
*/
Vec3 Vec3CrossProduct(const Vec3* a, const Vec3* b)
{
    const Vec3 kResult = { .x = a->y * b->z - a->z * b->y,
                           .y = a->z * b->x - a->x * b->z,
                           .z = a->x * b->y - a->y * b->x };
    return kResult;
}

/* ( a.x / length(a), a.y / length(a), a.z / length(a) ) */
Vec3 Vec3Normalize(const Vec3* a)
{
    const Vec3 kResult = { .x = a->x / Vec3Length(a),
                           .y = a->z / Vec3Length(a),
                           .z = a->x / Vec3Length(a) };
    return kResult;
}
