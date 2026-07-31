// lines_segments.h - Lines, Segments, and their operations
// Includes: Line representation, segment operations, intersections, distances

#ifndef LINES_SEGMENTS_H
#define LINES_SEGMENTS_H

#include "geometry_base.h"

// ============================================================================
// LINE OPERATIONS
// ============================================================================

struct Line;
bool intersection(Line l1, Line l2, pt &out);

struct Line {
    pt direction;   // Direction vector
    ld constant;    // cross(direction, point) = constant
    
    // Constructors
    Line(pt direction, ld constant) : direction(direction), constant(constant) {}
    Line(ld a, ld b, ld c) : direction({b, -a}), constant(c) {}
    Line(pt p, pt q) : direction(q - p), constant(cross(direction, p)) {}
    
    // Point side: >0 left, <0 right, =0 on line
    ld pointSide(pt p) {
        return cross(direction, p) - constant;
    }
    
    // Perpendicular distance from point to line
    ld distanceToPoint(pt p) {
        return fabsl(pointSide(p)) / abs(direction);
    }
    
    // Squared distance (faster, no sqrt)
    ld squaredDistanceToPoint(pt p) {
        ld s = pointSide(p);
        return s * s / (ld)sq(direction);
    }
    
    // Line perpendicular to this line through point p
    Line perpendicularThrough(pt p) {
        return {p, p + rotate90CCW(direction)};
    }
    
    // Compare projections of points onto line direction
    bool compareProjection(pt p, pt q) {
        return dotProduct(direction, p) < dotProduct(direction, q);
    }
    
    // Translate line by offset vector
    Line translated(pt offset) {
        return {direction, constant + cross(direction, offset)};
    }
    
    // Shift line left by given perpendicular distance
    Line shiftLeft(ld distance) {
        return {direction, constant + distance * abs(direction)};
    }
    
    // Orthogonal projection of point p onto line
    pt projection(pt p) {
        return p - rotate90CCW(direction) * pointSide(p) / (ld)sq(direction);
    }
    
    // Reflection of point p across line
    pt reflection(pt p) {
        return p - rotate90CCW(direction) * (ld)2 * pointSide(p) / (ld)sq(direction);
    }

    // Get line coefficients: A, B, C where Ax + By + C = 0
    tuple<ld, ld, ld> coefficients() const {
        return {-direction.Y, direction.X, -constant};
    }
};

// Get line equation coefficients: Ax + By + C = 0
tuple<ld, ld, ld> lineEquation(pt a, pt b) {
    return {-b.Y + a.Y, b.X - a.X, -(a.X * (b.Y - a.Y) - a.Y * (b.X - a.X))};
}

// Get line equation from point and normal vector
tuple<ld, ld, ld> lineEquationNormal(pt p, pt normal) {
    ld A = normal.X;
    ld B = normal.Y;
    ld C = -(A * p.X + B * p.Y);
    return {A, B, C};
}

// Intersection point of two lines
bool intersection(Line l1, Line l2, pt &out) {
    ld det = cross(l1.direction, l2.direction);
    if (fabsl(det) < EPS) return false;
    out = (l2.direction * l1.constant - l1.direction * l2.constant) / det;
    return true;
}

// Distance from point p to ray starting at a through b
ld distancePointToRay(pt a, pt b, pt p) {
    if (dotProduct(b - a, p - a) < 0) return abs(p - a);
    Line l(a, b);
    return l.distanceToPoint(p);
}

// Check if two rays intersect
bool raysIntersect(pt a, pt b, pt c, pt d) {
    Line l1(a, b), l2(c, d);
    pt p;

    if (!intersection(l1, l2, p)) {
        // Parallel
        if (fabsl(orient(a, b, c)) > EPS)
            return false;

        // Collinear - check if rays overlap
        return dotProduct(b - a, c - a) >= -EPS ||
               dotProduct(d - c, a - c) >= -EPS;
    }

    return dotProduct(b - a, p - a) >= -EPS &&
           dotProduct(d - c, p - c) >= -EPS;
}

// Distance between two rays
ld distanceRayToRay(pt a, pt b, pt c, pt d) {
    if (raysIntersect(a, b, c, d))
        return 0.0L;

    return min({
        distancePointToRay(a, b, c),
        distancePointToRay(a, b, d),
        distancePointToRay(c, d, a),
        distancePointToRay(c, d, b)
    });
}

// Angle bisector of two intersecting lines
// interior=true: internal bisector, false: external bisector
Line angleBisector(Line l1, Line l2, bool interior) {
    assert(fabsl(cross(l1.direction, l2.direction)) > EPS);
    ld sign = interior ? 1 : -1;
    return {
        l2.direction / abs(l2.direction) + l1.direction / abs(l1.direction) * sign,
        l2.constant / abs(l2.direction) + l1.constant / abs(l1.direction) * sign
    };
}

// Angle bisector of ∠BAC
// internal = true  -> internal angle bisector
// internal = false -> external angle bisector
Line angleBisector(pt A, pt B, pt C, bool internal = true) {
    pt u = normalize(B - A);
    pt v = normalize(C - A);

    pt dir = internal ? (u + v) : (u - v);

    if (abs(dir) < EPS)
        dir = rotate90CCW(u);

    return Line(A, A + dir);
}

pair<Line, Line> parallelLinesAtDistance(Line l, ld r) {
    return {l.shiftLeft(r), l.shiftLeft(-r)};
}

pair<tuple<ld, ld, ld>, tuple<ld, ld, ld>>
parallelLineCoefficients(ld A, ld B, ld C, ld R) {
    ld delta = R * sqrtl(A * A + B * B);

    return {
        {A, B, C + delta},
        {A, B, C - delta}
    };
}

// Collinear means lying on the same straight line.
bool collinear(pt a, pt b, pt c) {
    return sgn(orient(a, b, c)) == 0;
}

/*bool collinear(pt a, pt b, pt c) {
    return fabsl((b.X - a.X) * (c.Y - a.Y) -
                 (b.Y - a.Y) * (c.X - a.X)) < EPS;
}*/

// ============================================================================
// SEGMENT OPERATIONS
// ============================================================================

// Check if point p is inside the circle with diameter AB
bool inDisk(pt a, pt b, pt p) {
    return dotProduct(a - p, b - p) <= EPS;
}

// Check if point c lies on segment AB
bool onSegment(pt a, pt b, pt c) {
    return fabsl(orient(a, b, c)) < EPS && inDisk(a, b, c);
}

// Proper intersection of two segments (crossing at interior point)
bool properIntersection(pt a, pt b, pt c, pt d, pt &out) {
    ld oa = orient(c, d, a);
    ld ob = orient(c, d, b);
    ld oc = orient(a, b, c);
    ld od = orient(a, b, d);
    
    if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0) {
        out = (a * ob - b * oa) / (ob - oa);
        return true;
    }
    return false;
}

// All intersection points between two segments
set<pair<ld, ld>> segmentIntersections(pt a, pt b, pt c, pt d) {
    set<pair<ld, ld>> intersections;
    pt p;
    
    // Check shared endpoints
    if (a == c || a == d) intersections.insert({a.X, a.Y});
    if (b == c || b == d) intersections.insert({b.X, b.Y});
    if (!intersections.empty()) return intersections;
    
    // Proper intersection
    if (properIntersection(a, b, c, d, p))
        return {{p.X, p.Y}};
    
    // Endpoint-on-segment cases
    if (onSegment(c, d, a)) intersections.insert({a.X, a.Y});
    if (onSegment(c, d, b)) intersections.insert({b.X, b.Y});
    if (onSegment(a, b, c)) intersections.insert({c.X, c.Y});
    if (onSegment(a, b, d)) intersections.insert({d.X, d.Y});
    
    return intersections;
}

// Minimum distance from point p to segment AB
ld distancePointToSegment(pt a, pt b, pt p) {
    if (a != b) {
        Line l(a, b);
        if (l.compareProjection(a, p) && l.compareProjection(p, b))
            return l.distanceToPoint(p);
    }
    return min(abs(p - a), abs(p - b));
}

// Minimum distance between two segments
ld distanceSegmentToSegment(pt a, pt b, pt c, pt d) {
    pt p;
    if (properIntersection(a, b, c, d, p)) return 0;
    return min({
        distancePointToSegment(a, b, c),
        distancePointToSegment(a, b, d),
        distancePointToSegment(c, d, a),
        distancePointToSegment(c, d, b)
    });
}

#endif
