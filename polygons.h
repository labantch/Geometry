// polygons.h - Polygon operations
// Includes: Area, perimeter, point-in-polygon, convex hull

#ifndef POLYGONS_H
#define POLYGONS_H

#include "lines_segments.h"

// ============================================================================
// POLYGON OPERATIONS
// ============================================================================

// Triangle area
ld triangleArea(pt a, pt b, pt c) {
    return fabsl(cross(b - a, c - a)) / 2.0L;
}

// Polygon area using shoelace formula
ld polygonArea(vector<pt> poly) {
    ld area = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++)
        area += cross(poly[i], poly[(i + 1) % n]);
    return fabsl(area) / 2.0L;
}

// Returns the perimeter of a polygon.
// The vertices must be given in order (CW or CCW).
ld polygonPerimeter(const vector<pt>& poly) {
    ld per = 0;
    int n = poly.size();

    for (int i = 0; i < n; i++)
        per += abs(poly[(i + 1) % n] - poly[i]);

    return per;
}

// Helper for ray casting: check if p is above a
bool isAbove(pt a, pt p) {
    return p.Y >= a.Y;
}

// Check if segment PQ crosses horizontal ray from A to right
bool crossesRay(pt a, pt p, pt q) {
    return (isAbove(a, q) - isAbove(a, p)) * orient(a, p, q) > 0;
}

// Point in polygon test using ray casting
// strict=true: boundary is outside, false: boundary is inside
bool pointInPolygon(vector<pt> poly, pt point, bool strict = true) {
    int crossings = 0;
    int n = poly.size();
    
    for (int i = 0; i < n; i++) {
        pt curr = poly[i];
        pt next = poly[(i + 1) % n];
        
        if (onSegment(curr, next, point))
            return !strict;
        
        if (crossesRay(point, curr, next))
            crossings++;
    }
    
    return crossings & 1;
}

// ============================================================================
// CONVEX HULL
// ============================================================================

// Helper for convex hull: check if points are in clockwise order
bool cw(pt a, pt b, pt c, bool include_collinear) {
    int o = sgn(orient(a, b, c));
    return o < 0 || (include_collinear && o == 0);
}

bool ccw(pt a, pt b, pt c, bool include_collinear) {
    int o = sgn(orient(a, b, c));
    return o > 0 || (include_collinear && o == 0);
}

// Check if three points are collinear
bool collinear(pt a, pt b, pt c) { 
    return sgn(orient(a, b, c)) == 0; 
}

// Convex hull using Andrew's monotone chain algorithm
// include_collinear: if true, collinear points on edges are included
void convex_hull(vector<pt>& a, bool include_collinear = false) {
    if (a.size() <= 1) return;

    // Lowest point (if tie, leftmost)
    pt p0 = *min_element(a.begin(), a.end(), [](pt a, pt b) {
        return make_pair(a.Y, a.X) < make_pair(b.Y, b.X);
    });

    sort(a.begin(), a.end(), [&p0](const pt& a, const pt& b) {
        int o = sgn(orient(p0, a, b));
        if (o == 0)
            return sq(a - p0) < sq(b - p0);

        return o < 0;
    });

    if (include_collinear) {
        int i = (int)a.size() - 1;
        while (i >= 0 && collinear(p0, a[i], a.back())) i--;
        reverse(a.begin() + i + 1, a.end());
    }

    vector<pt> st;
    for (int i = 0; i < (int)a.size(); i++) {
        while (st.size() > 1 && !cw(st[st.size() - 2], st.back(), a[i], include_collinear))
            st.pop_back();

        if (st.empty() || a[i] != st.back())
            st.push_back(a[i]);
    }

    if (!include_collinear && st.size() == 2 && st[0] == st[1])
        st.pop_back();

    a = st;
}

// Check if point p is inside or on the boundary of a convex polygon.
// The polygon may be in CW or CCW order.
bool pointInConvexPolygon(const vector<pt>& poly, pt p) {
    int n = poly.size();

    for (int i = 0; i < n; i++) {
        ld o = orient(poly[i], poly[(i + 1) % n], p);

        if (sgn(o) == 0) {
            if (onSegment(poly[i], poly[(i + 1) % n], p))
                continue;
        }

        if (i == 0)
            o = (o > 0 ? 1 : (o < 0 ? -1 : 0));
        else {
            int cur = (o > 0 ? 1 : (o < 0 ? -1 : 0));
            if (cur != 0 && cur != (orient(poly[0], poly[1], p) > 0 ? 1 : (orient(poly[0], poly[1], p) < 0 ? -1 : 0)))
                return false;
        }
    }

    return true;
}

// ============================================================================
// POLYGON DISTANCE OPERATIONS
// ============================================================================

// Minimum distance between a point and a polygon (convex or simple)
ld distancePointToPolygon(const vector<pt>& poly, pt p, pt* closest = nullptr) {
    int n = poly.size();
    if (n == 0) return numeric_limits<ld>::max();
    
    ld minDist = numeric_limits<ld>::max();
    pt closestPt;
    
    for (int i = 0; i < n; i++) {
        pt a = poly[i];
        pt b = poly[(i + 1) % n];
        
        ld dist = distancePointToSegment(a, b, p);
        
        if (dist < minDist) {
            minDist = dist;
            
            if (closest) {
                pt proj = Line(a, b).projection(p);
                if (onSegment(a, b, proj)) {
                    closestPt = proj;
                } else {
                    closestPt = (abs(p - a) < abs(p - b)) ? a : b;
                }
            }
        }
    }
    
    if (closest) *closest = closestPt;
    return minDist;
}

// Minimum distance between two convex polygons
// Uses rotating calipers approach for O(n+m) complexity
ld distanceConvexPolygons(vector<pt>& P, vector<pt>& Q) {
    int n = P.size(), m = Q.size();
    
    // Check for intersection first
    for (const auto& p : P) {
        if (pointInPolygon(Q, p, false)) return 0;
    }
    for (const auto& q : Q) {
        if (pointInPolygon(P, q, false)) return 0;
    }
    
    // Check if any edge intersects
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            pt temp;
            if (properIntersection(P[i], P[(i+1)%n], Q[j], Q[(j+1)%m], temp))
                return 0;
        }
    }
    
    // Find minimum distance using rotating calipers
    ld minDist = numeric_limits<ld>::max();
    int p = 0, q = 0;
    
    for (int i = 1; i < n; i++) {
        if (P[i].Y < P[p].Y || (P[i].Y == P[p].Y && P[i].X < P[p].X))
            p = i;
    }
    for (int j = 1; j < m; j++) {
        if (Q[j].Y > Q[q].Y || (Q[j].Y == Q[q].Y && Q[j].X > Q[q].X))
            q = j;
    }
    
    for (int step = 0; step < n + m; step++) {
        minDist = min(minDist, abs(P[p] - Q[q]));
        
        pt nextP = P[(p + 1) % n];
        pt nextQ = Q[(q + 1) % m];
        
        ld angle = cross(nextP - P[p], nextQ - Q[q]);
        if (angle >= 0) {
            p = (p + 1) % n;
        } else {
            q = (q + 1) % m;
        }
    }
    
    return minDist;
}

// Minimum distance between two simple polygons (not necessarily convex)
ld distanceSimplePolygons(const vector<pt>& P, const vector<pt>& Q) {
    int n = P.size(), m = Q.size();
    
    for (int i = 0; i < n; i++) {
        if (pointInPolygon(Q, P[i], false)) return 0;
    }
    for (int i = 0; i < m; i++) {
        if (pointInPolygon(P, Q[i], false)) return 0;
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            pt temp;
            if (properIntersection(P[i], P[(i+1)%n], Q[j], Q[(j+1)%m], temp))
                return 0;
        }
    }
    
    ld minDist = numeric_limits<ld>::max();
    
    for (int i = 0; i < n; i++) {
        minDist = min(minDist, distancePointToPolygon(Q, P[i]));
    }
    
    for (int i = 0; i < m; i++) {
        minDist = min(minDist, distancePointToPolygon(P, Q[i]));
    }
    
    return minDist;
}

// Wrapper function that automatically chooses the best algorithm
ld distanceBetweenPolygons(vector<pt>& P, vector<pt>& Q) {
    bool convexP = true, convexQ = true;
    
    int n = P.size();
    if (n >= 3) {
        int sign = 0;
        for (int i = 0; i < n; i++) {
            ld o = orient(P[i], P[(i+1)%n], P[(i+2)%n]);
            if (fabsl(o) > EPS) {
                int currentSign = (o > 0) ? 1 : -1;
                if (sign == 0) sign = currentSign;
                else if (sign != currentSign) {
                    convexP = false;
                    break;
                }
            }
        }
    }
    
    int m = Q.size();
    if (m >= 3) {
        int sign = 0;
        for (int i = 0; i < m; i++) {
            ld o = orient(Q[i], Q[(i+1)%m], Q[(i+2)%m]);
            if (fabsl(o) > EPS) {
                int currentSign = (o > 0) ? 1 : -1;
                if (sign == 0) sign = currentSign;
                else if (sign != currentSign) {
                    convexQ = false;
                    break;
                }
            }
        }
    }
    
    if (convexP && convexQ && n >= 3 && m >= 3) {
        return distanceConvexPolygons(P, Q);
    } else {
        return distanceSimplePolygons(P, Q);
    }
}

// ============================================================================
// MINKOWSKI SUM
// ============================================================================

// Reorder polygon starting from lowest point (for Minkowski sum)
void reorderPolygon(vector<pt>& P) {
    size_t pos = 0;
    for (size_t i = 1; i < P.size(); i++) {
        if (P[i].Y < P[pos].Y || (P[i].Y == P[pos].Y && P[i].X < P[pos].X))
            pos = i;
    }
    rotate(P.begin(), P.begin() + pos, P.end());
}

// Minkowski sum of two convex polygons (CCW order)
vector<pt> minkowskiSum(vector<pt> P, vector<pt> Q) {
    reorderPolygon(P);
    reorderPolygon(Q);
    
    P.push_back(P[0]);
    P.push_back(P[1]);
    Q.push_back(Q[0]);
    Q.push_back(Q[1]);
    
    vector<pt> result;
    size_t i = 0, j = 0;
    
    while (i < P.size() - 2 || j < Q.size() - 2) {
        result.push_back(P[i] + Q[j]);
        ld cr = cross(P[i + 1] - P[i], Q[j + 1] - Q[j]);
        if (cr >= 0 && i < P.size() - 2) ++i;
        if (cr <= 0 && j < Q.size() - 2) ++j;
    }
    return result;
}

#endif