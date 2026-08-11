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



// Distance Between Two Rays

// شرح:
// أقل مسافة بين شعاعين
// لو الشعاعين بيتقاطعوا → المسافة = 0
//
// Distance between two rays
ld distanceRayToRay(pt a, pt b, pt c, pt d) {

    // If the rays intersect, distance = 0
    if (raysIntersect(a, b, c, d))
        return 0.0L;

    // Otherwise, check all possible closest distances
    return min({
        distancePointToRay(a, b, c),
        distancePointToRay(a, b, d),
        distancePointToRay(c, d, a),
        distancePointToRay(c, d, b)
    });
}


// شعاع: نقطة بداية + اتجاه وحدة
// Ray: origin + unit direction
struct Ray {
    pt origin;
    pt direction; // unit length
};

// بناء شعاع من نقطتين a → b
// Build unit ray from a toward b
Ray makeRay(pt a, pt b) {
    pt d = b - a;
    return {a, d / abs(d)};
}

// تقاطع شعاعين (المعامل t >= 0 للاتنين)
// Intersection of two rays (both parameters >= 0)
bool intersectRay(Ray r1, Ray r2, pt& out) {
    pt dp = r2.origin - r1.origin;
    ld denom = cross(r1.direction, r2.direction);
    if (fabsl(denom) < EPS) return false;
    ld t1 = cross(dp, r2.direction) / denom;
    ld t2 = cross(dp, r1.direction) / denom;
    if (t1 < -EPS || t2 < -EPS) return false;
    out = r1.origin + r1.direction * t1;
    return true;
}

// استرجاع الشعاع التاني من المنصف: U معروف، B منصف → الشعاع الآخر
// Given known ray U and bisector B, recover the other ray direction
pt recoverOtherRay(pt U, pt B) {
    ld nu = abs(U), nb = abs(B);
    pt u = U / nu, b = B / nb;
    ld d = dotProduct(b, u);
    return (2.0L * d * b - u) * nu; // reflect & rescale
}

// تقاطع شعاع (origin → dir) مع قطعة seg
// Ray–segment intersection
struct Segment { pt a, b; };

bool raySegmentIntersection(pt origin, pt dir, Segment seg, pt& out) {
    pt r = dir;
    pt q = seg.a;
    pt s = seg.b - seg.a;
    ld rxs = cross(r, s);
    if (fabsl(rxs) < EPS) return false; // parallel
    ld t = cross(q - origin, s) / rxs;
    ld u = cross(q - origin, r) / rxs;
    if (t < -EPS) return false;          // behind ray origin
    if (u < -EPS || u > 1 + EPS) return false;
    out = origin + r * t;
    return true;
}

// ============================================================================
