// circles.h - Circle operations
// Includes: Circle-line, circle-circle intersection, circumcircle

#ifndef CIRCLES_H
#define CIRCLES_H

#include "polygons.h"

// ============================================================================
// CIRCLE OPERATIONS
// ============================================================================

struct Circle {
    pt center;
    ld radius;
};

// Circle-line intersection
// Returns: 0=no intersection, 1=tangent, 2=two points
int circleLineIntersection(pt center, ld r, Line l, pair<pt, pt>& out) {
    ld h2 = r * r - l.squaredDistanceToPoint(center);
    if (h2 < -EPS) return 0;
    h2 = max((ld)0, h2);
    
    pt p = l.projection(center);
    pt h = normalize(l.direction) * sqrtl(h2);
    out = {p - h, p + h};
    
    return (h2 <= EPS) ? 1 : 2;
}

// Circle-circle intersection
// Returns: 0=no intersection, 1=tangent, 2=two points
int circleCircleIntersection(pt o1, ld r1, pt o2, ld r2, pair<pt, pt>& out) {
    pt d = o2 - o1;
    ld D = abs(d);
    
    if (D > r1 + r2 + EPS) return 0;
    if (D < fabsl(r1 - r2) - EPS) return 0;
    if (D < EPS) return 0;
    
    ld xcoord = (r1 * r1 - r2 * r2 + D * D) / (2 * D);
    ld h2 = r1 * r1 - xcoord * xcoord;
    if (h2 < -EPS) return 0;
    h2 = max((ld)0, h2);
    
    pt p = o1 + normalize(d) * xcoord;
    pt v = rotate90CCW(normalize(d)) * sqrtl(h2);
    out = {p - v, p + v};
    
    return (h2 <= EPS) ? 1 : 2;
}

// Area of intersection of two circles
ld circleIntersectionArea(pt o1, ld r1, pt o2, ld r2) {
    ld d = abs(o2 - o1);
    
    if (d >= r1 + r2 - EPS) return 0;
    if (d <= fabsl(r1 - r2) + EPS) {
        ld r = min(r1, r2);
        return PI * r * r;
    }
    
    ld a1 = 2 * acosl(max((ld)-1.0L, min((ld)1.0L, (d*d + r1*r1 - r2*r2) / (2*d*r1))));
    ld a2 = 2 * acosl(max((ld)-1.0L, min((ld)1.0L, (d*d + r2*r2 - r1*r1) / (2*d*r2))));
    
    return r1*r1*(a1 - sinl(a1))/2.0L + r2*r2*(a2 - sinl(a2))/2.0L;
}

// Returns the circumscribed circle of a cyclic polygon.
// Returns false if the polygon is not cyclic or is degenerate.
bool circumcirclePolygon(const vector<pt>& poly, Circle &cir) {
    int n = poly.size();
    if (n < 3) return false;

    // Find any 3 non-collinear vertices.
    int a = 0, b = 1, c = -1;
    for (int i = 2; i < n; i++) {
        if (fabsl(cross(poly[b] - poly[a], poly[i] - poly[a])) > EPS) {
            c = i;
            break;
        }
    }

    if (c == -1) return false; // All points are collinear.

    pt A = poly[a], B = poly[b], C = poly[c];

    // Circumcenter formula.
    ld d = 2.0L * cross(B - A, C - A);

    pt u = B - A;
    pt v = C - A;

    ld uu = sq(u);
    ld vv = sq(v);

    cir.center = A + pt(
        (uu * v.Y - vv * u.Y) / d,
        (vv * u.X - uu * v.X) / d
    );

    cir.radius = abs(cir.center - A);

    // Check that every vertex lies on the circle.
    for (const pt &p : poly) {
        if (fabsl(abs(p - cir.center) - cir.radius) > EPS)
            return false;
    }

    return true;
}

// Circumradius of a regular n-gon.
ld circumradiusRegularPolygon(int n, ld side) {
    return side / (2.0L * sinl(PI / n));
}

// Area of the circumscribed circle.
ld circumcircleAreaRegularPolygon(int n, ld side) {
    ld R = circumradiusRegularPolygon(n, side);
    return PI * R * R;
}

// Count integer lattice points inside circle of radius r
long long countPoints(int r) {
    long long cnt = 0;
    long long rr = 1LL * r * r;

    for (int x = -r; x <= r; x++) {
        long long y = sqrtl(rr - 1LL * x * x);
        while (1LL * x * x + (y + 1) * (y + 1) <= rr) y++;
        while (1LL * x * x + y * y > rr) y--;
        cnt += 2 * y + 1;
    }

    return cnt;
}

#endif