// geometry_base.h - Basic geometric primitives and operations
// Includes: Points, Vectors, Transformations, Angles

#ifndef GEOMETRY_BASE_H
#define GEOMETRY_BASE_H

#include <bits/stdc++.h>
using namespace std;

typedef long double ld;
typedef complex<ld> pt;

const ld EPS = 1e-9L;
const ld PI = acosl(-1.0L);

#define X real()
#define Y imag()

// ============================================================================
// BASIC VECTOR OPERATIONS
// ============================================================================

// Squared length of a vector
ld sq(pt p) {
    return p.X * p.X + p.Y * p.Y;
}

// Dot product: a·b = |a||b|cos(θ)
// > 0: angle < 90°, = 0: perpendicular, < 0: angle > 90°
ld dotProduct(pt a, pt b) {
    return a.X * b.X + a.Y * b.Y;
}

// Cross product: a×b = |a||b|sin(θ)
// > 0: left turn (CCW), < 0: right turn (CW), = 0: collinear
ld cross(pt a, pt b) {
    return a.X * b.Y - a.Y * b.X;
}

// Returns the unit vector in the direction of v.
// Requires v != (0, 0).
pt normalize(pt v) {
    return v / abs(v);
}

// Sign function with EPS tolerance
int sgn(ld val) {
    if (val > EPS) return 1;
    if (val < -EPS) return -1;
    return 0;
}

// Check if two vectors are perpendicular
bool arePerpendicular(pt a, pt b) {
    return fabsl(dotProduct(a, b)) < EPS;
}

// Rotate vector 90° counter-clockwise: (x,y) → (-y,x)
pt rotate90CCW(pt v) {
    return {-v.Y, v.X};
}

// ============================================================================
// TRANSFORMATIONS
// ============================================================================

// Translate point p by vector v
pt translate(pt v, pt p) {
    return p + v;
}

// Scale point p relative to center c by factor
pt scale(pt c, ld factor, pt p) {
    return c + (p - c) * factor;
}

// Rotate point p around center c by angle a (radians, CCW)
pt rot(pt p, pt c, ld a) {
    pt v = p - c;
    pt rotate = {cosl(a), sinl(a)};
    return c + rotate * v;
}

// Linear transformation mapping segment (p→q) to (fp→fq)
// Applies same translation, rotation, and scaling to point r
pt linearTransfo(pt p, pt q, pt r, pt fp, pt fq) {
    return fp + (r - p) * (fq - fp) / (q - p);
}

// ============================================================================
// ANGLE OPERATIONS
// ============================================================================

// Orientation of three points: cross(b-a, c-a)
// > 0: left turn (CCW), < 0: right turn (CW), = 0: collinear
ld orient(pt a, pt b, pt c) {
    return cross(b - a, c - a);
}

// Smallest angle between two vectors [0, π]
ld angle(pt v, pt w) {
    ld cos_theta = dotProduct(v, w) / abs(v) / abs(w);
    return acosl(max((ld)-1.0L, min((ld)1.0L, cos_theta)));
}

// Polar angle of point p normalized to [0, 2π)
ld polarAngle(pt p) {
    ld ang = arg(p);
    if (ang < 0) ang += 2 * PI;
    return ang;
}

// Oriented angle from vector AB to AC, CCW in [0, 2π)
ld orientedAngle(pt a, pt b, pt c) {
    ld theta = angle(b - a, c - a);
    if (orient(a, b, c) > 0) return theta;
    return 2 * PI - theta;
}

// Signed angle from vector AB to AC in [-π, π]
ld angleTravelled(pt a, pt b, pt c) {
    ld theta = angle(b - a, c - a);
    if (orient(a, b, c) > 0) return theta;
    return -theta;
}

// Check if point p lies inside angle ∠BAC (including boundary)
bool inAngle(pt a, pt b, pt c, pt p) {
    ld abp = orient(a, b, p);
    ld acp = orient(a, c, p);
    ld abc = orient(a, b, c);
    
    if (abc < 0) swap(abp, acp);
    return (abp >= 0 && acp <= 0) ^ (abc < 0);
}

#endif