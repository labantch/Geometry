// advanced.h - Advanced geometry algorithms
// Includes: Half-plane intersection, closest pair, lattice points, convex utils

#ifndef ADVANCED_H
#define ADVANCED_H

#include "circles.h"

// ============================================================================
// LATTICE POINTS (Pick's Theorem)
// ============================================================================

// Number of lattice points on segment AB (includes A, excludes B)
long long latticeOnSegment(pt a, pt b) {
    return gcd((long long)llroundl(abs(a.X - b.X)),
               (long long)llroundl(abs(a.Y - b.Y)));
}

// Boundary lattice points of a lattice polygon
long long boundaryLatticePoints(const vector<pt>& poly) {
    long long cnt = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++)
        cnt += latticeOnSegment(poly[i], poly[(i + 1) % n]);
    return cnt;
}

// Interior lattice points using Pick's Theorem: A = I + B/2 - 1
long long interiorLatticePoints(const vector<pt>& poly) {
    long long B = boundaryLatticePoints(poly);
    ld A = polygonArea(poly);
    return llroundl(A - (ld)B / 2.0L + 1.0L);
}

// ============================================================================
// HALF-PLANE INTERSECTION
// ============================================================================

struct Halfplane {
    pt p, pq;      // Point and direction vector
    ld angle;
    
    Halfplane() {}
    Halfplane(const pt& a, const pt& b) : p(a), pq(b - a) {
        angle = atan2l(pq.Y, pq.X);
    }
    
    // Check if point r is outside this half-plane (left side is inside)
    bool out(const pt& r) {
        return cross(pq, r - p) < -EPS;
    }
    
    bool operator < (const Halfplane& e) const {
        return angle < e.angle;
    }
    
    // Intersection point of two half-plane lines (assumed not parallel)
    friend pt inter(const Halfplane& s, const Halfplane& t) {
        ld alpha = cross((t.p - s.p), t.pq) / cross(s.pq, t.pq);
        return s.p + (s.pq * alpha);
    }
};

// Intersection of half-planes (returns convex polygon)
vector<pt> halfPlaneIntersection(vector<Halfplane>& H) {
    const ld INF = 1e9L;
    pt box[4] = {
        pt(INF, INF),
        pt(-INF, INF),
        pt(-INF, -INF),
        pt(INF, -INF)
    };
    
    // Add bounding box half-planes (CCW order)
    for (int i = 0; i < 4; i++)
        H.push_back(Halfplane(box[i], box[(i + 1) % 4]));
    
    sort(H.begin(), H.end());
    deque<Halfplane> dq;
    int len = 0;
    
    for (int i = 0; i < (int)H.size(); i++) {
        // Remove redundant from back
        while (len > 1 && H[i].out(inter(dq[len-1], dq[len-2]))) {
            dq.pop_back();
            --len;
        }
        
        // Remove redundant from front
        while (len > 1 && H[i].out(inter(dq[0], dq[1]))) {
            dq.pop_front();
            --len;
        }
        
        // Handle parallel half-planes
        if (len > 0 && fabsl(cross(H[i].pq, dq[len-1].pq)) < EPS) {
            if (dotProduct(H[i].pq, dq[len-1].pq) < 0)  // Opposite directions
                return vector<pt>();
            
            if (H[i].out(dq[len-1].p)) {  // Keep leftmost
                dq.pop_back();
                --len;
            } else continue;
        }
        
        dq.push_back(H[i]);
        ++len;
    }
    
    // Final cleanup
    while (len > 2 && dq[0].out(inter(dq[len-1], dq[len-2]))) {
        dq.pop_back();
        --len;
    }
    while (len > 2 && dq[len-1].out(inter(dq[0], dq[1]))) {
        dq.pop_front();
        --len;
    }
    
    if (len < 3) return vector<pt>();
    
    // Reconstruct polygon
    vector<pt> ret(len);
    for (int i = 0; i + 1 < len; i++)
        ret[i] = inter(dq[i], dq[i+1]);
    ret.back() = inter(dq[len-1], dq[0]);
    return ret;
}

// ============================================================================
// CLOSEST PAIR OF POINTS
// ============================================================================

// Closest pair of points - O(n log n)
ld closestPairSquared(vector<pt> p) {
    sort(p.begin(), p.end(), [](pt a, pt b) {
        if (a.X != b.X) return a.X < b.X;
        return a.Y < b.Y;
    });
    
    set<pair<ld, ld>> window;  // {y, x}
    ld ans = numeric_limits<ld>::max();
    ld best = numeric_limits<ld>::max();
    int j = 0;
    
    for (int i = 0; i < (int)p.size(); i++) {
        while (j < i && p[i].X - p[j].X > best) {
            window.erase({p[j].Y, p[j].X});
            j++;
        }
        
        auto it1 = window.lower_bound({p[i].Y - best, -numeric_limits<ld>::max()});
        auto it2 = window.upper_bound({p[i].Y + best, numeric_limits<ld>::max()});
        
        for (auto it = it1; it != it2; ++it) {
            pt cur(it->second, it->first);
            ans = min(ans, sq(p[i] - cur));
        }
        
        if (ans != numeric_limits<ld>::max())
            best = sqrtl((ld)ans);
        
        window.insert({p[i].Y, p[i].X});
    }
    
    return ans;
}

ld closestPairDistance(vector<pt> p) {
    return sqrtl((ld)closestPairSquared(p));
}

// ============================================================================
// LINE INTERSECTIONS
// ============================================================================

// Check if any pair of lines intersect (unique intersection)
bool anyLineIntersection(const vector<Line>& lines, pt& out) {
    int n = lines.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (intersection(lines[i], lines[j], out))
                return true;
        }
    }
    return false;
}

// All intersection points between every pair of lines
vector<pt> allLineIntersections(const vector<Line>& lines) {
    vector<pt> ans;
    int n = lines.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            pt p;
            if (intersection(lines[i], lines[j], p))
                ans.push_back(p);
        }
    }
    return ans;
}

// ============================================================================
// CONVEX POLYGON UTILITIES
// ============================================================================

// Anti-podal pairs of a convex polygon
vector<pair<int, int>> allAntiPodalPairs(int n, vector<pt>& p) {
    vector<pair<int, int>> result;
    vector<bool> vis(n, false);
    
    auto nx = [&](int i) { return (i + 1) % n; };
    auto pv = [&](int i) { return (i - 1 + n) % n; };
    
    for (int p1 = 0, p2 = 0; p1 < n; ++p1) {
        pt base = p[nx(p1)] - p[p1];
        
        while (p2 == p1 || p2 == nx(p1) ||
               sgn(cross(base, p[nx(p2)] - p[p2])) == sgn(cross(base, p[p2] - p[pv(p2)]))) {
            p2 = nx(p2);
        }
        
        if (vis[p1]) continue;
        vis[p1] = true;
        
        result.push_back({p1, p2});
        result.push_back({nx(p1), p2});
        
        if (sgn(cross(base, p[nx(p2)] - p[p2])) == 0) {
            result.push_back({p1, nx(p2)});
            result.push_back({nx(p1), nx(p2)});
            vis[p2] = true;
        }
    }
    
    return result;
}

// Maximum distance between two convex polygons (rotating calipers)
ld maximumDistancePolygons(vector<pt>& u, vector<pt>& v) {
    int n = (int)u.size(), m = (int)v.size();
    ld ans = 0;
    
    if (n < 3 || m < 3) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                ans = max(ans, (ld)sq(u[i] - v[j]));
        return sqrtl((ld)ans);
    }
    
    if (u[0].X > v[0].X) {
        swap(n, m);
        swap(u, v);
    }
    
    int i = 0, j = 0, step = n + m + 10;
    while (j + 1 < m && v[j].X < v[j + 1].X) j++;
    
    while (step--) {
        if (cross(u[(i + 1) % n] - u[i], v[(j + 1) % m] - v[j]) >= 0)
            j = (j + 1) % m;
        else
            i = (i + 1) % n;
        ans = max(ans, (ld)sq(u[i] - v[j]));
    }
    
    return sqrtl((ld)ans);
}

#endif