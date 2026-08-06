// ============================================================================
// Geometry Notebook — CHAPTER 7 — POLYGONS: AREA, PERIMETER, POINT-IN-POLYGON
// ============================================================================

// CHAPTER 7 — POLYGONS: AREA, PERIMETER, POINT-IN-POLYGON
// شرح: العمليات الأساسية على المضلعات (Polygon) - مساحة ومحيط واختبار احتواء
// ============================================================================

// شرح: مساحة المثلث من إحداثيات الأضلاع
// Triangle area
// ---- Area ----
ld triangleArea(pt a, pt b, pt c) {
    return fabsl(cross(b - a, c - a)) / 2.0L;
}

// ---- Distance / Cross / Circumcircle (Point-style helpers) ----

// المسافة بين نقطتين
// Euclidean distance between two points
ld Distance(pt a, pt b) {
    return abs(a - b);
}

// الضرب الاتجاهي للمتجهين AB و AC (موجب = يسار، سالب = يمين)
// Cross product of AB and AC (same as orient)
ld Cross(pt a, pt b, pt c) {
    return cross(b - a, c - a);
}

// مساحة المثلث من ثلاث نقط (نفس triangleArea)
// Triangle area from three points
ld TriangleArea(pt a, pt b, pt c) {
    return fabsl(Cross(a, b, c)) / 2.0L;
}

// نصف قطر الدائرة المحيطة بالمثلث ABC
// Circumradius of triangle ABC
ld CircumRadius(pt a, pt b, pt c) {
    ld sideA = Distance(b, c);
    ld sideB = Distance(a, c);
    ld sideC = Distance(a, b);
    ld area = TriangleArea(a, b, c);
    if (area < EPS) return 0; // degenerate
    return (sideA * sideB * sideC) / (4.0L * area);
}

// محيط الدائرة المحيطة بالمثلث ABC
// Circumference of the circumcircle of triangle ABC
ld Circumference(pt a, pt b, pt c) {
    return 2.0L * PI * CircumRadius(a, b, c);
}

// شرح: مساحة المضلع باستخدام صيغة الحذاء (Shoelace Formula)
// Polygon area using shoelace formula
ld polygonArea(vector<pt> poly) {
    ld area = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++)
        area += cross(poly[i], poly[(i + 1) % n]);
    return fabsl(area) / 2.0L;
}

// شرح: محيط المضلع (مجموع أطوال أضلاعه)
// Returns the perimeter of a polygon.
// The vertices must be given in order (CW or CCW).
// ---- Perimeter ----
ld polygonPerimeter(const vector<pt>& poly) {
    ld per = 0;
    int n = poly.size();

    for (int i = 0; i < n; i++)
        per += abs(poly[(i + 1) % n] - poly[i]);

    return per;
}

// شرح: دالة مساعدة بتتأكد إذا كانت النقطة p فوق النقطة a (لخوارزمية Ray Casting)
// Helper for ray casting: check if p is above a
// ---- Point-in-polygon (ray casting) ----
bool isAbove(pt a, pt p) {
    return p.Y >= a.Y;
}

// شرح: بيتأكد إذا كانت القطعة PQ بتقطع الشعاع الأفقي الخارج من a لليمين
// Check if segment PQ crosses horizontal ray from A to right
bool crossesRay(pt a, pt p, pt q) {
    return (isAbove(a, q) - isAbove(a, p)) * orient(a, p, q) > 0;
}

// شرح: اختبار "نقطة جوه مضلع" باستخدام تقنية Ray Casting
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
