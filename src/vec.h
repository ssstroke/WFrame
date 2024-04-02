#pragma once

typedef struct {
    int x;
    int y;
} Vec2_Int;

typedef struct {
    double x;
    double y;
} Vec2_Double;

typedef struct {
    int x;
    int y;
    int z;
} Vec3_Int;

typedef struct {
    double x;
    double y;
    double z;
} Vec3_Double;

/* (a, b, c) + (d, e, f) = (a + d, b + e, c + f) */
Vec3_Double Vec3Add(const Vec3_Double*, const Vec3_Double*);

/* (a, b, c) - (d, e, f) = (a - d, b - e, c - f) */
Vec3_Double Vec3Sub(const Vec3_Double*, const Vec3_Double*);

/* (a, b, c) + k = (a + k, b + k, c + k) */
Vec3_Double Vec3AddScalar(const Vec3_Double*, double);

/* (a, b, c) - k = (a - k, b - k, c - k) */
Vec3_Double Vec3SubScalar(const Vec3_Double*, double);

/* (a, b, c) * k = (ka, kb, kc) */
Vec3_Double Vec3MulScalar(const Vec3_Double*, double);

/* (a, b, c) / k = (a / k, b / k, c / k) */
Vec3_Double Vec3DivScalar(const Vec3_Double*, double);

/* sqrt(x * x + y * y + z * z) */
double Vec3Length(const Vec3_Double*);

/* (x * x) + (y * y) + (z * z) */
double Vec3LengthSquared(const Vec3_Double*);

/* (a.x * b.x) + (a.y * b.y) + (a.z * b.z) */
double Vec3DotProduct(const Vec3_Double*, const Vec3_Double*);

/*
(a.y * b.z) - (a.z * b.y),
(a.z * b.x) - (a.x * b.z),
(a.x * b.y) - (a.y * b.x)
*/
Vec3_Double Vec3CrossProduct(const Vec3_Double*, const Vec3_Double*);

/* ( x / length(vec), y / length(vec), z / length(vec) ) */
Vec3_Double Vec3Normalize(const Vec3_Double*);
