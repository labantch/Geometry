// ============================================================================
// Geometry Notebook — CHAPTER 6 — SEGMENTS
// ============================================================================

// CHAPTER 6 — SEGMENTS
// شرح: عمليات على القطع المستقيمة (Segment) - بداية ونهاية محددتين
// ============================================================================

// شرح: بيتأكد إذا كانت النقطة p جوه الدائرة اللي قطرها AB
// Check if point p is inside the circle with diameter AB
// ---- Helpers (disk / inside) ----
bool inDisk(pt a, pt b, pt p) {
    return dotProduct(a - p, b - p) <= EPS;
}

// شرح: بيتأكد إذا كانت النقطة p جوه أو على حدود دائرة مركزها center ونصف قطرها r
bool inside(pt p, pt center, ld r) {
    return sq(p - center) <= r * r + EPS;
}

// شرح: بيتأكد إذا كانت النقطة c واقعة بالظبط على القطعة AB
// Check if point c lies on segment AB
// ---- Point on segment ----
bool onSegment(pt a, pt b, pt c) {
    return fabsl(orient(a, b, c)) < EPS && inDisk(a, b, c);
}

// ---- Point on line / ray ----
// Check if point p lies on the infinite line through a and b
bool onLine(pt a, pt b, pt p) {
    return fabsl(orient(a, b, p)) < EPS;
}

// Check if point p lies on the ray starting at a going through b
// (includes the endpoint a)
bool onRay(pt a, pt b, pt p) {
    return fabsl(orient(a, b, p)) < EPS && dotProduct(b - a, p - a) >= -EPS;
}

// شرح: تقاطع "حقيقي" بين قطعتين (بيتقاطعوا جوه بعض مش على الأطراف)
// Proper intersection of two segments (crossing at interior point)
// ---- Segment intersection ----
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

// شرح: كل نقط التقاطع الممكنة بين قطعتين (شامل حالات التطابق على الأطراف)
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

// شرح: أقل مسافة من نقطة p للقطعة المستقيمة AB
// Minimum distance from point p to segment AB
// ---- Distances ----
ld distancePointToSegment(pt a, pt b, pt p) {
    if (a != b) {
        Line l(a, b);
        if (l.compareProjection(a, p) && l.compareProjection(p, b))
            return l.distanceToPoint(p);
    }
    return min(abs(p - a), abs(p - b));
}

// شرح: أقل مسافة بين قطعتين مستقيمتين (صفر لو متقاطعين)
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


// ============================================================================
