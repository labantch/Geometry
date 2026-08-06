// ============================================================================
// Geometry Notebook — CHAPTER 20 — EXTRA CONTEST UTILITIES (ECPC / ACPC)
// ============================================================================

// CHAPTER 20 — EXTRA CONTEST UTILITIES (ECPC / ACPC)
// ============================================================================

// ---------------------------------------------------------------------------
// 20.1  Radial / polar sort
// ترتيب نقط polar حول مركز
// ---------------------------------------------------------------------------

// ترتيب النقط حسب الزاوية القطبية حول المركز (الأقرب أولاً عند التساوي)
// Sort points by polar angle around center (closer first on ties)
void radialSort(vector<pt>& pts, pt center = {0, 0}) {
    auto ang = [&](pt p) { return atan2l((p - center).Y, (p - center).X); };
    sort(pts.begin(), pts.end(), [&](pt a, pt b) {
        ld aa = ang(a), bb = ang(b);
        if (fabsl(aa - bb) > EPS) return aa < bb;
        return sq(a - center) < sq(b - center);
    });
}

// مقارنة نقطتين حسب الزاوية القطبية حول مركز (للاستخدام في sort / set)
// Compare two points by polar angle around center
bool polarLess(pt center, pt a, pt b) {
    pt va = a - center, vb = b - center;
    ld c = cross(va, vb);
    if (fabsl(c) > EPS) return c > 0;
    return sq(va) < sq(vb);
}

// الزاوية القطبية للنقطة p بالنسبة للمركز، في المدى [0, 2π)
// Polar angle of p relative to center, normalized to [0, 2π)
ld polarAngleFrom(pt center, pt p) {
    ld ang = atan2l((p - center).Y, (p - center).X);
    if (ang < 0) ang += 2 * PI;
    return ang;
}

// ضرب اتجاهي صحيح (آمن من الـ overflow لإحداثيات المسابقات)
// Integer cross product (overflow-safe for typical contest coords)
long long crossLL(long long ax, long long ay, long long bx, long long by) {
    return ax * by - ay * bx;
}
long long crossLL(pair<long long,long long> a, pair<long long,long long> b) {
    return a.first * b.second - a.second * b.first;
}

// ---------------------------------------------------------------------------
// 20.2  Triangle centers
// مراكز المثلث
// ---------------------------------------------------------------------------

// مركز الدائرة المحيطة بالمثلث ABC
// Circumcenter of triangle ABC
pt circumcenter(pt a, pt b, pt c) {
    pt ab = b - a, ac = c - a;
    ld d = 2.0L * cross(ab, ac);
    if (fabsl(d) < EPS) return a;
    ld u = norm2(ab), v = norm2(ac);
    return a + pt((u * ac.Y - v * ab.Y) / d, (v * ab.X - u * ac.X) / d);
}

// مركز الدائرة الداخلية للمثلث ABC (ملتقى المنصفات)
// Incenter of triangle ABC
pt incenter(pt a, pt b, pt c) {
    ld aa = abs(b - c), bb = abs(a - c), cc = abs(a - b);
    ld s = aa + bb + cc;
    if (s < EPS) return a;
    return (a * aa + b * bb + c * cc) / s;
}

// مركز ثقل المثلث ABC (متوسط الرؤوس)
// Centroid of triangle ABC
pt centroid(pt a, pt b, pt c) { return (a + b + c) / 3.0L; }

// مركز ثقل المضلع (موزون بالمساحة)
// Area-weighted centroid of a polygon
pt polygonCentroid(const vector<pt>& poly) {
    ld A = 0, cx = 0, cy = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        pt u = poly[i], v = poly[(i + 1) % n];
        ld cuv = cross(u, v);
        A += cuv; cx += (u.X + v.X) * cuv; cy += (u.Y + v.Y) * cuv;
    }
    A *= 0.5L;
    if (fabsl(A) < EPS) return poly.empty() ? pt{0,0} : poly[0];
    return {cx / (6.0L * A), cy / (6.0L * A)};
}

// ---------------------------------------------------------------------------
// 20.3  Point in convex polygon — O(log n)
// نقطة داخل مضلع محدب في O(log n)
// المضلع لازم يكون CCW ومحدب
// ---------------------------------------------------------------------------

// هل النقطة p داخل مضلع محدب؟ (بحث ثنائي على المروحة من الرأس 0)
// Point in convex polygon via binary search on the fan from vertex 0
bool pointInConvexPolygonLog(const vector<pt>& poly, pt p) {
    int n = (int)poly.size();
    if (n < 3) return false;
    if (orient(poly[0], poly[1], p) < -EPS) return false;
    if (orient(poly[0], poly[n - 1], p) > EPS) return false;
    int l = 1, r = n - 1;
    while (r - l > 1) {
        int mid = (l + r) / 2;
        if (orient(poly[0], poly[mid], p) >= 0) l = mid;
        else r = mid;
    }
    return orient(poly[l], poly[r], p) >= -EPS;
}

// ---------------------------------------------------------------------------
// 20.3b  Shape inside polygon
// شكل جوه مضلع: قطعة / دائرة / مضلع
// ---------------------------------------------------------------------------

// هل القطعة AB كلها جوه المضلع؟
// (الطرفين جوه + مفيش تقاطع مع أي ضلع إلا على الحدود)
// Segment AB fully inside polygon (endpoints inside, no improper edge crossings)
bool segmentInPolygon(const vector<pt>& poly, pt a, pt b) {
    if (!pointInPolygon(poly, a, false)) return false;
    if (!pointInPolygon(poly, b, false)) return false;
    int n = (int)poly.size();
    for (int i = 0; i < n; i++) {
        pt c = poly[i], d = poly[(i + 1) % n];
        pt out;
        // proper crossing => segment goes outside
        if (properIntersection(a, b, c, d, out)) return false;
    }
    // midpoint check (handles concave cases where endpoints in but middle out)
    pt mid = (a + b) / 2.0L;
    if (!pointInPolygon(poly, mid, false)) return false;
    return true;
}

// هل الدائرة كلها جوه المضلع؟
// المركز جوه + أقل مسافة من المركز لحدود المضلع >= نصف القطر
// Circle fully inside polygon: center inside and dist(center, boundary) >= r
bool circleInPolygon(const vector<pt>& poly, pt center, ld r) {
    if (!pointInPolygon(poly, center, false)) return false;
    // distance to boundary must be at least r
    // if center is strictly inside, distancePointToPolygon may be 0 when
    // implemented as 0-for-inside — so compute min distance to edges
    ld minDist = 1e100L;
    int n = (int)poly.size();
    for (int i = 0; i < n; i++) {
        minDist = min(minDist, distancePointToSegment(poly[i], poly[(i + 1) % n], center));
    }
    return minDist >= r - EPS;
}

// هل المضلع A كله جوه المضلع B؟
// كل رؤوس A جوه B + كل أضلاع A جوه B (مفيش تقاطع حقيقي)
// Polygon A fully inside polygon B
bool polygonInPolygon(const vector<pt>& A, const vector<pt>& B) {
    if (A.empty()) return true;
    for (pt p : A) {
        if (!pointInPolygon(B, p, false)) return false;
    }
    int n = (int)A.size(), m = (int)B.size();
    for (int i = 0; i < n; i++) {
        pt a = A[i], b = A[(i + 1) % n];
        // each edge of A must lie inside B
        if (!segmentInPolygon(B, a, b)) return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// 20.3c  Circle ↔ polygon: outside?  intersection area
// دائرة ومضلع: بره؟ مساحة التقاطع؟
// ---------------------------------------------------------------------------

// هل الدائرة كلها بره المضلع؟ (مفيش تقاطع)
// Circle completely outside polygon (no intersection with interior/boundary)
bool circleOutsidePolygon(const vector<pt>& poly, pt center, ld r) {
    // if center inside => not outside
    if (pointInPolygon(poly, center, false)) return false;
    // if any vertex inside circle => intersects
    for (pt p : poly) {
        if (abs(p - center) <= r + EPS) return false;
    }
    // if any edge closer than r => intersects
    int n = (int)poly.size();
    for (int i = 0; i < n; i++) {
        if (distancePointToSegment(poly[i], poly[(i + 1) % n], center) <= r + EPS)
            return false;
    }
    return true;
}

// ---- helpers for circle ∩ polygon area (center translated to origin) ----

// مساحة القطاع الدائري الموقع من زاوية من a إلى b (مركز 0)
// Signed sector area from ray 0→a to ray 0→b on circle radius r
static ld _sectorAreaSigned(pt a, pt b, ld r) {
    ld ang = atan2l(a.Y, a.X) - atan2l(b.Y, b.X);
    // normalize to (-π, π]
    while (ang > PI) ang -= 2 * PI;
    while (ang < -PI) ang += 2 * PI;
    return 0.5L * r * r * ang;
}

// مساحة تقاطع المثلث (0, a, b) مع القرص نصف قطره r (مركز 0)
// Intersection area of triangle (0,a,b) with disk radius r centered at 0
static ld _diskTriangleArea(pt a, pt b, ld r) {
    ld da = abs(a), db = abs(b);
    // both inside disk → full triangle
    if (da <= r + EPS && db <= r + EPS)
        return cross(a, b) / 2.0L;

    // find segment–circle intersections (segment a-b)
    pt d = b - a;
    ld A = norm2(d);
    ld B = 2.0L * dotProduct(a, d);
    ld C = norm2(a) - r * r;
    ld disc = B * B - 4.0L * A * C;

    vector<pt> hits;
    if (disc >= -EPS && A > EPS) {
        disc = max(disc, (ld)0);
        ld s = sqrtl(disc);
        for (ld sgn : {(ld)-1, (ld)1}) {
            ld t = (-B + sgn * s) / (2.0L * A);
            if (t >= -EPS && t <= 1.0L + EPS) {
                t = min((ld)1, max((ld)0, t));
                hits.push_back(a + d * t);
            }
        }
    }
    // unique hits
    vector<pt> uniq;
    for (pt p : hits) {
        bool ok = true;
        for (pt q : uniq) if (abs(p - q) < EPS) { ok = false; break; }
        if (ok) uniq.push_back(p);
    }
    hits = uniq;

    // case: both outside, 0 or 2 intersections
    if (da > r + EPS && db > r + EPS) {
        if (hits.size() < 2) {
            // edge misses disk: only sector if origin side sees the arc
            // if line is far, contribution is sector only when origin's projection...
            // safer: if distance from 0 to line ab >= r → pure sector of angle a0b
            Line L(a, b);
            if (L.distanceToPoint({0, 0}) >= r - EPS)
                return _sectorAreaSigned(a, b, r);
            // line cuts disk but segment may not: use sector
            return _sectorAreaSigned(a, b, r);
        }
        // two hits: sector + middle part through chord
        pt p = hits[0], q = hits[1];
        // order along edge
        if (dotProduct(p - a, d) > dotProduct(q - a, d)) swap(p, q);
        return _sectorAreaSigned(a, p, r) + cross(p, q) / 2.0L + _sectorAreaSigned(q, b, r);
    }

    // one inside, one outside
    if (hits.empty()) {
        // shouldn't happen often; fallback sector
        return _sectorAreaSigned(a, b, r);
    }
    pt p = hits[0];
    if (da <= r + EPS) {
        // a inside, b outside
        return cross(a, p) / 2.0L + _sectorAreaSigned(p, b, r);
    } else {
        // b inside, a outside
        return _sectorAreaSigned(a, p, r) + cross(p, b) / 2.0L;
    }
}

// مساحة تقاطع دائرة مع مضلع (يشتغل كويس مع المحدب؛ للبسيط كمان غالباً)
// Area of (circle ∩ polygon). Best for convex; OK for simple polygons.
ld circlePolygonIntersectionArea(const vector<pt>& poly, pt center, ld r) {
    if (r < EPS || poly.size() < 3) return 0;
    // full circle inside polygon
    if (circleInPolygon(poly, center, r))
        return PI * r * r;
    // circle fully outside
    if (circleOutsidePolygon(poly, center, r))
        return 0;

    ld area = 0;
    int n = (int)poly.size();
    for (int i = 0; i < n; i++) {
        pt a = poly[i] - center;
        pt b = poly[(i + 1) % n] - center;
        area += _diskTriangleArea(a, b, r);
    }
    return fabsl(area);
}

// مساحة الجزء من الدائرة اللي بره المضلع
// Area of circle that lies outside the polygon = πr² - intersection
ld circleAreaOutsidePolygon(const vector<pt>& poly, pt center, ld r) {
    return max((ld)0, PI * r * r - circlePolygonIntersectionArea(poly, center, r));
}

// مساحة الجزء من المضلع اللي جوه الدائرة (نفس تقاطع الدائرة∩المضلع)
// Area of polygon that lies inside the circle (same as intersection)
ld polygonAreaInsideCircle(const vector<pt>& poly, pt center, ld r) {
    return circlePolygonIntersectionArea(poly, center, r);
}

// ---------------------------------------------------------------------------
// 20.3d  Shape in / out of polygon (convex or general)
// شكل جوه أو بره مضلع (محدب أو عادي)
// ---------------------------------------------------------------------------

// ---- helpers: point-in with optional convex fast path ----
static bool _pip(const vector<pt>& poly, pt p, bool strict, bool convex) {
    if (convex) {
        // boundary: treat like non-strict via O(n) convex test when needed
        if (strict) {
            // strict inside: all orients strictly same side
            int n = (int)poly.size();
            for (int i = 0; i < n; i++) {
                ld o = orient(poly[i], poly[(i + 1) % n], p);
                if (o <= EPS) return false;
            }
            return true;
        }
        return pointInConvexPolygon(poly, p);
    }
    return pointInPolygon(poly, p, strict);
}

// قطعة بره المضلع بالكامل؟
// Segment completely outside polygon
bool segmentOutsidePolygon(const vector<pt>& poly, pt a, pt b, bool convex = false) {
    if (_pip(poly, a, false, convex) || _pip(poly, b, false, convex)) return false;
    int n = (int)poly.size();
    for (int i = 0; i < n; i++) {
        pt out;
        if (properIntersection(a, b, poly[i], poly[(i + 1) % n], out)) return false;
        // touch counts as not fully outside
        auto hits = segmentIntersections(a, b, poly[i], poly[(i + 1) % n]);
        if (!hits.empty()) return false;
    }
    return true;
}

// مثلث جوه المضلع؟
bool triangleInPolygon(const vector<pt>& poly, const Triangle& t, bool convex = false) {
    if (!_pip(poly, t.a, false, convex)) return false;
    if (!_pip(poly, t.b, false, convex)) return false;
    if (!_pip(poly, t.c, false, convex)) return false;
    if (!segmentInPolygon(poly, t.a, t.b)) return false;
    if (!segmentInPolygon(poly, t.b, t.c)) return false;
    if (!segmentInPolygon(poly, t.c, t.a)) return false;
    return true;
}

// مثلث بره المضلع بالكامل؟
bool triangleOutsidePolygon(const vector<pt>& poly, const Triangle& t, bool convex = false) {
    if (_pip(poly, t.a, false, convex) || _pip(poly, t.b, false, convex) ||
        _pip(poly, t.c, false, convex)) return false;
    // any edge intersects polygon edge?
    pt edges[3][2] = {{t.a, t.b}, {t.b, t.c}, {t.c, t.a}};
    int n = (int)poly.size();
    for (auto& e : edges) {
        if (!segmentOutsidePolygon(poly, e[0], e[1], convex)) return false;
    }
    // polygon vertex inside triangle?
    for (pt p : poly) {
        if (t.contains(p, false)) return false;
    }
    return true;
}

// مستطيل جوه المضلع؟
bool rectangleInPolygon(const vector<pt>& poly, const Rectangle& R, bool convex = false) {
    pt p1(min(R.x1, R.x2), min(R.y1, R.y2));
    pt p2(max(R.x1, R.x2), min(R.y1, R.y2));
    pt p3(max(R.x1, R.x2), max(R.y1, R.y2));
    pt p4(min(R.x1, R.x2), max(R.y1, R.y2));
    vector<pt> corners = {p1, p2, p3, p4};
    for (pt p : corners)
        if (!_pip(poly, p, false, convex)) return false;
    for (int i = 0; i < 4; i++)
        if (!segmentInPolygon(poly, corners[i], corners[(i + 1) % 4])) return false;
    return true;
}

// مستطيل بره المضلع؟
bool rectangleOutsidePolygon(const vector<pt>& poly, const Rectangle& R, bool convex = false) {
    pt p1(min(R.x1, R.x2), min(R.y1, R.y2));
    pt p2(max(R.x1, R.x2), min(R.y1, R.y2));
    pt p3(max(R.x1, R.x2), max(R.y1, R.y2));
    pt p4(min(R.x1, R.x2), max(R.y1, R.y2));
    vector<pt> corners = {p1, p2, p3, p4};
    for (pt p : corners)
        if (_pip(poly, p, false, convex)) return false;
    for (int i = 0; i < 4; i++)
        if (!segmentOutsidePolygon(poly, corners[i], corners[(i + 1) % 4], convex)) return false;
    // any polygon vertex inside rectangle?
    for (pt p : poly)
        if (R.contains(p, false)) return false;
    return true;
}

// مربع جوه / بره المضلع
bool squareInPolygon(const vector<pt>& poly, const Square& S, bool convex = false) {
    Rectangle R(S.bottomLeft.X, S.bottomLeft.Y,
                S.bottomLeft.X + S.side, S.bottomLeft.Y + S.side);
    return rectangleInPolygon(poly, R, convex);
}
bool squareOutsidePolygon(const vector<pt>& poly, const Square& S, bool convex = false) {
    Rectangle R(S.bottomLeft.X, S.bottomLeft.Y,
                S.bottomLeft.X + S.side, S.bottomLeft.Y + S.side);
    return rectangleOutsidePolygon(poly, R, convex);
}

// دائرة جوه مضلع محدب (أسرع)
bool circleInConvexPolygon(const vector<pt>& poly, pt center, ld r) {
    return circleInPolygon(poly, center, r); // same test; poly must be convex for meaning
}
// دائرة بره مضلع محدب
bool circleOutsideConvexPolygon(const vector<pt>& poly, pt center, ld r) {
    return circleOutsidePolygon(poly, center, r);
}

// مضلع A بره المضلع B بالكامل؟
bool polygonOutsidePolygon(const vector<pt>& A, const vector<pt>& B) {
    for (pt p : A)
        if (pointInPolygon(B, p, false)) return false;
    for (pt p : B)
        if (pointInPolygon(A, p, false)) return false;
    int n = (int)A.size(), m = (int)B.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            auto hits = segmentIntersections(A[i], A[(i + 1) % n], B[j], B[(j + 1) % m]);
            if (!hits.empty()) return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// 20.4  Tangents
// المماسات (من نقطة لدائرة، ومشتركة بين دائرتين)
// ---------------------------------------------------------------------------

// مماسات من نقطة p لدائرة مركزها o ونصف قطرها r
// يرجع 0 / 1 / 2 نقط تماس على الدائرة
// Tangents from point p to circle (o, r); returns 0/1/2 tangent points
int tangentsPointCircle(pt p, pt o, ld r, pair<pt, pt>& out) {
    pt v = p - o;
    ld d2 = sq(v);
    if (d2 < r * r - EPS) return 0;
    if (fabsl(d2 - r * r) <= EPS) { out = {p, p}; return 1; }
    ld d = sqrtl(d2);
    ld h = r * sqrtl(d2 - r * r) / d2;
    ld s = r * r / d2;
    pt mid = o + v * s;
    pt perp = {-v.Y * h, v.X * h};
    out = {mid + perp, mid - perp};
    return 2;
}

// المماسات الخارجية المشتركة بين دائرتين (مباشرة)
// External (direct) common tangents of two circles
vector<pair<pt, pt>> externalTangents(pt o1, ld r1, pt o2, ld r2) {
    vector<pair<pt, pt>> res;
    pt d = o2 - o1;
    if (abs(d) < EPS) return res;
    if (fabsl(r1 - r2) < EPS) {
        pt perp = normalize(rotate90CCW(d)) * r1;
        res.push_back({o1 + perp, o2 + perp});
        res.push_back({o1 - perp, o2 - perp});
        return res;
    }
    ld k = r1 / r2;
    pt c = (o1 - o2 * k) / (1 - k);
    pair<pt, pt> t1, t2;
    int n1 = tangentsPointCircle(c, o1, r1, t1), n2 = tangentsPointCircle(c, o2, r2, t2);
    if (n1 >= 1 && n2 >= 1) res.push_back({t1.first, t2.first});
    if (n1 >= 2 && n2 >= 2) res.push_back({t1.second, t2.second});
    return res;
}

// المماسات الداخلية المشتركة بين دائرتين (متقاطعة)
// Internal (cross) common tangents of two circles
vector<pair<pt, pt>> internalTangents(pt o1, ld r1, pt o2, ld r2) {
    vector<pair<pt, pt>> res;
    pt d = o2 - o1;
    ld dist = abs(d);
    if (dist < EPS || dist < fabsl(r1 + r2) - EPS) return res;
    ld k = r1 / r2;
    pt c = (o1 + o2 * k) / (1 + k);
    pair<pt, pt> t1, t2;
    int n1 = tangentsPointCircle(c, o1, r1, t1), n2 = tangentsPointCircle(c, o2, r2, t2);
    if (n1 >= 1 && n2 >= 1) res.push_back({t1.first, t2.first});
    if (n1 >= 2 && n2 >= 2) res.push_back({t1.second, t2.second});
    return res;
}

// ---------------------------------------------------------------------------
// 20.5  Segment–circle intersection
// تقاطع قطعة مستقيمة مع دائرة
// ---------------------------------------------------------------------------

// تقاطع القطعة AB مع الدائرة (o, r) — يرجع 0 / 1 / 2 نقط على القطعة
// Intersection of segment AB with circle (o, r); points must lie ON the segment
int segmentCircleIntersection(pt a, pt b, pt o, ld r, pair<pt, pt>& out) {
    Line l(a, b);
    pair<pt, pt> cand;
    int n = circleLineIntersection(o, r, l, cand);
    if (n == 0) return 0;
    vector<pt> good;
    if (n >= 1 && onSegment(a, b, cand.first)) good.push_back(cand.first);
    if (n >= 2 && onSegment(a, b, cand.second)) good.push_back(cand.second);
    if (good.empty()) return 0;
    if (good.size() == 1) { out = {good[0], good[0]}; return 1; }
    out = {good[0], good[1]};
    return 2;
}

// ---------------------------------------------------------------------------
// 20.6  Power of a point
// قوة النقطة بالنسبة للدائرة: PO² - r²
// ---------------------------------------------------------------------------

// قوة النقطة (نسخة أعداد صحيحة)
// Power of point (integer version): dx² + dy² - r²
long long PowerOfPoint(long long cx, long long cy, long long r, long long px, long long py) {
    long long dx = px - cx, dy = py - cy;
    return dx * dx + dy * dy - r * r;
}

// قوة النقطة (نسخة floating)
// Power of point (floating version)
ld powerOfPoint(pt o, ld r, pt p) { return sq(p - o) - r * r; }

// ---------------------------------------------------------------------------
// 20.7  Minimum enclosing circle (Welzl)
// أصغر دائرة تغطي كل النقط — O(n) متوقع
// ---------------------------------------------------------------------------

// أصغر دائرة تمر بنقطتين (القطر = المسافة بينهما)
Circle minCircle2(pt a, pt b) { return {(a + b) / 2.0L, abs(a - b) / 2.0L}; }

// أصغر دائرة تمر بثلاث نقط (أو قطر أطول ضلع لو المثلث منفرج)
Circle minCircle3(pt a, pt b, pt c) {
    if (dotProduct(b - a, c - a) <= 0) return minCircle2(b, c);
    if (dotProduct(a - b, c - b) <= 0) return minCircle2(a, c);
    if (dotProduct(a - c, b - c) <= 0) return minCircle2(a, b);
    pt cen = circumcenter(a, b, c);
    return {cen, abs(cen - a)};
}

// خوارزمية Welzl العشوائية لأصغر دائرة محيطة
Circle welzl(vector<pt>& pts, vector<pt> r, int n) {
    if (n == 0 || r.size() == 3) {
        if (r.size() == 0) return {{0, 0}, 0};
        if (r.size() == 1) return {r[0], 0};
        if (r.size() == 2) return minCircle2(r[0], r[1]);
        return minCircle3(r[0], r[1], r[2]);
    }
    int i = rand() % n;
    swap(pts[i], pts[n - 1]);
    Circle d = welzl(pts, r, n - 1);
    if (abs(pts[n - 1] - d.center) <= d.radius + EPS) return d;
    r.push_back(pts[n - 1]);
    return welzl(pts, r, n - 1);
}

// أصغر دائرة تغطي مجموعة النقط (الواجهة العامة)
// Minimum enclosing circle of a point set
Circle minimumEnclosingCircle(vector<pt> pts) {
    if (pts.empty()) return {{0, 0}, 0};
    if (pts.size() == 1) return {pts[0], 0};
    srand(time(nullptr));
    random_shuffle(pts.begin(), pts.end());
    return welzl(pts, {}, (int)pts.size());
}

// ---------------------------------------------------------------------------
// 20.8  Distance from point to triangle
// أقل مسافة من نقطة لمثلث (0 لو جوه)
// ---------------------------------------------------------------------------

// المسافة من النقطة p للمثلث ABC (صفر لو النقطة داخله)
// Distance from p to triangle ABC (0 if inside)
ld distancePointToTriangle(pt p, pt a, pt b, pt c) {
    ld o1 = orient(a, b, p), o2 = orient(b, c, p), o3 = orient(c, a, p);
    if ((o1 >= -EPS && o2 >= -EPS && o3 >= -EPS) ||
        (o1 <=  EPS && o2 <=  EPS && o3 <=  EPS)) return 0;
    return min({distancePointToSegment(a, b, p),
                distancePointToSegment(b, c, p),
                distancePointToSegment(c, a, p)});
}

// ---------------------------------------------------------------------------
// 20.9  Poles
// أعمدة: ترتيب بالمسافة أو بالاتجاه ثم المسافة
// ---------------------------------------------------------------------------

// عمود: (x, y) المكان و z الارتفاع
struct Pole { int x, y, z; };

// ترتيب الأعمدة من الأقرب للأبعد عن الأصل (0,0)
// Sort poles by distance from origin (nearest first)
bool cmpDistance(const Pole& a, const Pole& b) {
    long long da = 1LL * a.x * a.x + 1LL * a.y * a.y;
    long long db = 1LL * b.x * b.x + 1LL * b.y * b.y;
    return da < db;
}

// ترتيب حسب الاتجاه من الأصل، ثم المسافة (الأقرب أولاً في نفس الاتجاه)
// Sort by direction from origin, then by distance
bool cmpDirection(const Pole& a, const Pole& b) {
    int g1 = gcd(abs(a.x), abs(a.y));
    int g2 = gcd(abs(b.x), abs(b.y));
    if (g1 == 0) g1 = 1;
    if (g2 == 0) g2 = 1;
    pair<int,int> d1 = {a.x / g1, a.y / g1};
    pair<int,int> d2 = {b.x / g2, b.y / g2};
    if (d1 != d2) return d1 < d2;
    long long da = 1LL * a.x * a.x + 1LL * a.y * a.y;
    long long db = 1LL * b.x * b.x + 1LL * b.y * b.y;
    return da < db;
}

// ---------------------------------------------------------------------------
// 20.10  Line sweep
// خط المسح — أحداث + مجموعة نشطة
// ---------------------------------------------------------------------------

// حدث على خط المسح الرأسي (يتحرك من الشمال لليمين)
// Event for a vertical sweep line moving left → right
struct SweepEvent {
    long long x;
    int type; // 0 = طرف أيسر، 1 = طرف أيمن، 2 = استعلام
    long long y1, y2;
    int id;
    bool operator<(const SweepEvent& o) const {
        if (x != o.x) return x < o.x;
        return type < o.type;
    }
};

// هل في تقاطع بين قطع أفقية ورأسية؟ O(n log n)
// Detect any intersection among axis-aligned H/V segments
bool anyHVIntersection(vector<array<long long,4>> segs) {
    vector<SweepEvent> ev;
    for (int i = 0; i < (int)segs.size(); i++) {
        auto [x1, y1, x2, y2] = segs[i];
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (y1 == y2) {
            ev.push_back({x1, 0, y1, y1, i});
            ev.push_back({x2, 1, y1, y1, i});
        } else if (x1 == x2) {
            ev.push_back({x1, 2, y1, y2, i});
        }
    }
    sort(ev.begin(), ev.end());
    multiset<long long> activeY;
    for (auto& e : ev) {
        if (e.type == 0) activeY.insert(e.y1);
        else if (e.type == 1) activeY.erase(activeY.find(e.y1));
        else {
            auto it = activeY.lower_bound(e.y1);
            if (it != activeY.end() && *it <= e.y2) return true;
        }
    }
    return false;
}

// أقرب زوج نقط بخط المسح O(n log n)
// Closest pair of points via line sweep
ld closestPairSweep(vector<pt> pts) {
    int n = (int)pts.size();
    if (n < 2) return numeric_limits<ld>::max();
    sort(pts.begin(), pts.end(), [](pt a, pt b) {
        if (a.X != b.X) return a.X < b.X;
        return a.Y < b.Y;
    });
    set<pair<ld, ld>> window;
    ld best = numeric_limits<ld>::max();
    int j = 0;
    for (int i = 0; i < n; i++) {
        while (j < i && pts[i].X - pts[j].X > best) {
            window.erase({pts[j].Y, pts[j].X});
            j++;
        }
        auto lo = window.lower_bound({pts[i].Y - best, -1e100L});
        auto hi = window.upper_bound({pts[i].Y + best,  1e100L});
        for (auto it = lo; it != hi; ++it) {
            pt q(it->second, it->first);
            best = min(best, abs(pts[i] - q));
        }
        window.insert({pts[i].Y, pts[i].X});
    }
    return best;
}

// مساحة اتحاد مستطيلات بمحاذاة المحاور — خط مسح O(n²)
// Union area of axis-aligned rectangles via line sweep
long long unionRectAreaSweep(vector<array<long long,4>> rects) {
    struct E {
        long long x; int typ, y1, y2;
        bool operator<(const E& o) const {
            if (x != o.x) return x < o.x;
            return typ > o.typ;
        }
    };
    vector<E> ev;
    vector<long long> ys;
    for (auto& r : rects) {
        long long x1 = r[0], y1 = r[1], x2 = r[2], y2 = r[3];
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (x1 == x2 || y1 == y2) continue;
        ev.push_back({x1, 1, (int)y1, (int)y2});
        ev.push_back({x2, -1, (int)y1, (int)y2});
        ys.push_back(y1); ys.push_back(y2);
    }
    if (ev.empty()) return 0;
    sort(ev.begin(), ev.end());
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    int m = (int)ys.size();
    vector<int> cov(m, 0);
    auto covered_len = [&]() -> long long {
        long long len = 0;
        for (int i = 0; i + 1 < m; i++)
            if (cov[i] > 0) len += ys[i + 1] - ys[i];
        return len;
    };
    auto add = [&](int y1, int y2, int delta) {
        int l = lower_bound(ys.begin(), ys.end(), y1) - ys.begin();
        int r = lower_bound(ys.begin(), ys.end(), y2) - ys.begin();
        for (int i = l; i < r; i++) cov[i] += delta;
    };
    long long area = 0;
    for (int i = 0; i < (int)ev.size(); ) {
        int j = i;
        long long x = ev[i].x;
        while (j < (int)ev.size() && ev[j].x == x) {
            add(ev[j].y1, ev[j].y2, ev[j].typ);
            j++;
        }
        if (j < (int)ev.size()) area += covered_len() * (ev[j].x - x);
        i = j;
    }
    return area;
}
