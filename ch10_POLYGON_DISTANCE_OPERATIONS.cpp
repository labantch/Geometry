// ============================================================================
// Geometry Notebook — CHAPTER 10 — POLYGON DISTANCE OPERATIONS
// ============================================================================

// CHAPTER 10 — POLYGON DISTANCE OPERATIONS
// شرح: حساب المسافة بين نقطة ومضلع، أو بين مضلعين
// ============================================================================

// شرح: أقل مسافة من نقطة p لمضلع (محدب أو غير محدب)، ممكن ترجع أقرب نقطة كمان
// Minimum distance between a point and a polygon (convex or simple)
// Returns distance, and optionally the closest point on the polygon
// ---- Point to polygon ----
ld distancePointToPolygon(const vector<pt>& poly, pt p, pt* closest = nullptr) {
    int n = poly.size();
    if (n == 0) return numeric_limits<ld>::max();
    
    ld minDist = numeric_limits<ld>::max();
    pt closestPt;
    
    for (int i = 0; i < n; i++) {
        pt a = poly[i];
        pt b = poly[(i + 1) % n];
        
        // Distance from point to segment
        ld dist = distancePointToSegment(a, b, p);
        
        if (dist < minDist) {
            minDist = dist;
            
            // Find closest point on segment if requested
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

// شرح: أقل مسافة بين مضلعين محدبين باستخدام Rotating Calipers في O(n+m)
// Minimum distance between two convex polygons
// Uses rotating calipers approach for O(n+m) complexity
// ---- Convex polygon distance ----
ld distanceConvexPolygons(vector<pt>& P, vector<pt>& Q) {
    int n = P.size(), m = Q.size();
    
    // Check for intersection first
    // If polygons intersect, distance is 0
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
    
    // Find starting points
    int p = 0, q = 0;
    for (int i = 1; i < n; i++) {
        if (P[i].Y < P[p].Y || (P[i].Y == P[p].Y && P[i].X < P[p].X))
            p = i;
    }
    for (int j = 1; j < m; j++) {
        if (Q[j].Y > Q[q].Y || (Q[j].Y == Q[q].Y && Q[j].X > Q[q].X))
            q = j;
    }
    
    // Rotating calipers
    for (int step = 0; step < n + m; step++) {
        // Check distance between vertices
        minDist = min(minDist, abs(P[p] - Q[q]));
        
        // Check distance from vertex to edge
        pt nextP = P[(p + 1) % n];
        pt nextQ = Q[(q + 1) % m];
        
        // Move the polygon with the smaller angle
        ld angle = cross(nextP - P[p], nextQ - Q[q]);
        if (angle >= 0) {
            p = (p + 1) % n;
        } else {
            q = (q + 1) % m;
        }
    }
    
    return minDist;
}

// شرح: أقل مسافة بين مضلعين غير محدبين (بأي شكل)، أبطأ (O(n*m)) لكنها عامة
// Minimum distance between two simple polygons (not necessarily convex)
// O(n*m) complexity - works for any simple polygons
// ---- Simple polygon distance ----
ld distanceSimplePolygons(const vector<pt>& P, const vector<pt>& Q) {
    int n = P.size(), m = Q.size();
    
    // Check for intersection first
    for (int i = 0; i < n; i++) {
        if (pointInPolygon(Q, P[i], false)) return 0;
    }
    for (int i = 0; i < m; i++) {
        if (pointInPolygon(P, Q[i], false)) return 0;
    }
    
    // Check edge intersections
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            pt temp;
            if (properIntersection(P[i], P[(i+1)%n], Q[j], Q[(j+1)%m], temp))
                return 0;
        }
    }
    
    // Check all vertex-edge distances
    ld minDist = numeric_limits<ld>::max();
    
    // Vertices of P to edges of Q
    for (int i = 0; i < n; i++) {
        minDist = min(minDist, distancePointToPolygon(Q, P[i]));
    }
    
    // Vertices of Q to edges of P
    for (int i = 0; i < m; i++) {
        minDist = min(minDist, distancePointToPolygon(P, Q[i]));
    }
    
    return minDist;
}

// شرح: دالة ذكية بتختار تلقائيًا أسرع خوارزمية حسب كون المضلعين محدبين ولا لأ
// Wrapper function that automatically chooses the best algorithm
// If both polygons are convex, uses O(n+m) algorithm
// Otherwise uses O(n*m) algorithm
// ---- Auto dispatcher ----
ld distanceBetweenPolygons(vector<pt>& P, vector<pt>& Q) {
    // Check if both polygons are convex
    bool convexP = true, convexQ = true;
    
    // Check convexity of P
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
    
    // Check convexity of Q
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
