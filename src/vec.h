#pragma once

typedef struct {
    double x;
    double y;
    double z;
} Vec3;

/* (a, b, c) + (d, e, f) = (a + d, b + e, c + f) */
Vec3 Vec3Add(const Vec3*, const Vec3*);

/* (a, b, c) - (d, e, f) = (a - d, b - e, c - f) */
Vec3 Vec3Sub(const Vec3*, const Vec3*);

/* (a, b, c) + k = (a + k, b + k, c + k) */
Vec3 Vec3AddScalar(const Vec3*, double);

/* (a, b, c) - k = (a - k, b - k, c - k) */
Vec3 Vec3SubScalar(const Vec3*, double);

/* (a, b, c) * k = (ka, kb, kc) */
Vec3 Vec3MulScalar(const Vec3*, double);

/* (a, b, c) / k = (a / k, b / k, c / k) */
Vec3 Vec3DivScalar(const Vec3*, double);

/* sqrt(x * x + y * y + z * z) */
double Vec3Length(const Vec3*);

/* x * x + y * y + z * z */
double Vec3LengthSquared(const Vec3*);

/* a.x * b.x + a.y * b.y + a.z * b.z */
double Vec3DotProduct(const Vec3*, const Vec3*);

/*
a.y * b.z - a.z * b.y,
a.z * b.x - a.x * b.z,
a.x * b.y - a.y * b.x
*/
Vec3 Vec3CrossProduct(const Vec3*, const Vec3*);

/* ( x / length(vec), y / length(vec), z / length(vec) ) */
Vec3 Vec3Normalize(const Vec3*);

typedef struct {
    int x;
    int y;
} Vec2Int;
