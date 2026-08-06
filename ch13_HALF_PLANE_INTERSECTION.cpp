// ============================================================================
// Geometry Notebook — CHAPTER 13 — HALF-PLANE INTERSECTION
// ============================================================================

// CHAPTER 13 — HALF-PLANE INTERSECTION
// شرح: تقاطع مجموعة أنصاف مستويات (Half-planes) بيرجع مضلع محدب
// ============================================================================

// شرح: يمثل نصف مستوي (Half-plane) بنقطة ومتجه اتجاه وزاويته
// ---- Half-plane struct ----
struct Halfplane {
    pt p, pq;      // Point and direction vector
    ld angle;
    
    Halfplane() {}
    Halfplane(const pt& a, const pt& b) : p(a), pq(b - a) {
        angle = atan2l(pq.Y, pq.X);
    }
    
    // شرح: بيتأكد إذا كانت النقطة r برة نصف المستوي (الجهة اليسرى هي "جوه")
    // Check if point r is outside this half-plane (left side is inside)
    bool out(const pt& r) {
        return cross(pq, r - p) < -EPS;
    }
    
    bool operator < (const Halfplane& e) const {
        return angle < e.angle;
    }
    
    // شرح: نقطة تقاطع خطي نصفي مستويين (على افتراض إنهم مش متوازيين)
    // Intersection point of two half-plane lines (assumed not parallel)
    friend pt inter(const Halfplane& s, const Halfplane& t) {
        ld alpha = cross((t.p - s.p), t.pq) / cross(s.pq, t.pq);
        return s.p + (s.pq * alpha);
    }
};

// شرح: تقاطع مجموعة أنصاف مستويات - النتيجة مضلع محدب (أو فاضي لو مفيش تقاطع)
// Intersection of half-planes (returns convex polygon)
// ---- Half-plane intersection ----
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
