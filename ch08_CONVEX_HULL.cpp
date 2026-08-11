// ============================================================================
// Geometry Notebook — CHAPTER 8 — CONVEX HULL
// ============================================================================

// CHAPTER 8 — CONVEX HULL
// شرح: بناء الغلاف المحدب (Convex Hull) لمجموعة نقط، وفحص التحدب والاحتواء
// ============================================================================

// شرح: دالة مساعدة بتتأكد إذا كانت النقط الثلاثة بترسم دوران مع عقارب الساعة
// Helper for convex hull: check if points are in clockwise order
// ---- Orientation helpers ----
bool cw(pt a, pt b, pt c, bool include_collinear) {
    int o = sgn(orient(a, b, c));
    return o < 0 || (include_collinear && o == 0);
}

// شرح: نفس اللي فوق بس لدوران عكس عقارب الساعة
bool ccw(pt a, pt b, pt c, bool include_collinear) {
    int o = sgn(orient(a, b, c));
    return o > 0 || (include_collinear && o == 0);
}

// شرح: بيتأكد إذا كانت ثلاث نقط واقعة على استقامة واحدة
// Check if three points are collinear
bool collinear(pt a, pt b, pt c) { 
    return sgn(orient(a, b, c)) == 0; 
}

// شرح: بناء الغلاف المحدب باستخدام خوارزمية Andrew's Monotone Chain
// Convex hull using Andrew's monotone chain algorithm
// include_collinear: if true, collinear points on edges are included
// ---- Build convex hull ----
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

        // CW : return o < 0;
        // CCW: return o > 0;
        return o < 0;
    });

    if (include_collinear) {
        int i = (int)a.size() - 1;
        while (i >= 0 && collinear(p0, a[i], a.back())) i--;
        reverse(a.begin() + i + 1, a.end());
    }

    vector<pt> st;
    for (int i = 0; i < (int)a.size(); i++) {
        // CW : !cw(...)
        // CCW: !ccw(...)
        while (st.size() > 1 && !cw(st[st.size() - 2], st.back(), a[i], include_collinear))
            st.pop_back();

        if (st.empty() || a[i] != st.back())
            st.push_back(a[i]);
    }

    if (!include_collinear && st.size() == 2 && st[0] == st[1])
        st.pop_back();

    a = st;
}

// شرح: بيتأكد إذا كانت النقطة p جوه أو على حدود مضلع محدب (CW أو CCW)
// Returns true if p is inside or on the boundary of a convex polygon.
// The polygon may be in CW or CCW order.
// ---- Point in convex polygon ----
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

// شرح: بيتأكد إذا كان المضلع محدبًا بالكامل (كل الزوايا في نفس الاتجاه)
// UTILITY: Check if polygon is convex
// ---- Convexity test ----
bool isConvex(vector<pt>& polygon) {
    int n = polygon.size();
    if (n < 3) return false;
    
    int sign = 0;
    for (int i = 0; i < n; i++) {
        ld o = orient(polygon[i], polygon[(i+1)%n], polygon[(i+2)%n]);
        if (fabsl(o) > EPS) {
            int curSign = (o > 0) ? 1 : -1;
            if (sign == 0) sign = curSign;
            else if (sign != curSign) return false;
        }
    }
    return true;
}

// نقطة مع فهرسها الأصلي (مفيد بعد الـ hull)
struct PP {
    pt po;
    int idx;
};

// Convex hull (Andrew) على PP — رجّع CCW
// لازم pts تكون متخصوصية حسب x ثم y قبل النداء
vector<PP> convexHullPP(vector<PP>& pts) {
    int n = (int)pts.size(), k = 0;
    if (n <= 1) return pts;
    sort(pts.begin(), pts.end(), [](const PP& a, const PP& b) {
        if (fabsl(a.po.X - b.po.X) > EPS) return a.po.X < b.po.X;
        return a.po.Y < b.po.Y;
    });
    vector<PP> H(2 * n);
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(H[k - 1].po - H[k - 2].po, pts[i].po - H[k - 2].po) < -EPS) k--;
        H[k++] = pts[i];
    }
    for (int i = n - 2, t = k + 1; i >= 0; --i) {
        while (k >= t && cross(H[k - 1].po - H[k - 2].po, pts[i].po - H[k - 2].po) < -EPS) k--;
        H[k++] = pts[i];
    }
    if (k > 0) H.resize(k - 1);
    else H.clear();
    return H;
}


// ============================================================================
