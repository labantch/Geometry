// ============================================================================
// Geometry Notebook — CHAPTER 5 — RAYS
// ============================================================================

// CHAPTER 5 — RAYS
// شرح: عمليات على الأشعة (Ray) - نقطة بداية + اتجاه لما نهاية
// ============================================================================

// شرح: المسافة من نقطة p لأقرب نقطة على الشعاع اللي بادئ من a وماشي في اتجاه b
// Distance from point p to ray starting at a through b
// ---- Point to ray ----
ld distancePointToRay(pt a, pt b, pt p) {
    if (dotProduct(b - a, p - a) < 0) return abs(p - a);
    Line l(a, b);
    return l.distanceToPoint(p);
}

// شرح: بيتأكد إذا كان شعاعين بيتقاطعوا (بما فيهم حالة التطابق على نفس الخط)
// Check if two rays intersect
// ---- Ray intersection & distance ----
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

// شرح: أقل مسافة بين شعاعين (صفر لو متقاطعين)
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

// ============================================================================
