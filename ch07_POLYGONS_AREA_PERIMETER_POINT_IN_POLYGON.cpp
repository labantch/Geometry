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


// نقطة داخل مضلع بطريقة ray-crossing (winding/toggle)
// Point-in-polygon via ray casting (even-odd)
bool pointInPolygonWinding(const vector<pt>& p, pt q) {
    bool in = false;
    int n = (int)p.size();
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((p[i].Y > q.Y) != (p[j].Y > q.Y)) &&
            (q.X < (p[j].X - p[i].X) * (q.Y - p[i].Y) / (p[j].Y - p[i].Y) + p[i].X))
            in = !in;
    }
    return in;
}

// موقع النقطة: INSIDE / OUTSIDE / BOUNDARY
// Classify point vs polygon
string pointPolygonLocation(const vector<pt>& poly, pt p) {
    int n = (int)poly.size();
    for (int i = 0; i < n; i++) {
        pt A = poly[i], B = poly[(i + 1) % n];
        if (fabsl(orient(A, B, p)) < EPS &&
            dotProduct(p - A, p - B) <= EPS)
            return "BOUNDARY";
    }
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        pt A = poly[i], B = poly[(i + 1) % n];
        if ((A.Y > p.Y) != (B.Y > p.Y)) {
            ld ori = (B.X - A.X) * (p.Y - A.Y) - (B.Y - A.Y) * (p.X - A.X);
            if ((B.Y > A.Y) == (ori > 0)) cnt++;
        }
    }
    return (cnt % 2 == 1) ? "INSIDE" : "OUTSIDE";
}

// قص مضلع بنصف مستوى يسار الخط A→B
// Clip polygon to the left half-plane of directed line A→B
vector<pt> clipAgainstLine(const vector<pt>& poly, pt A, pt B) {
    vector<pt> out;
    int n = (int)poly.size();
    for (int i = 0; i < n; i++) {
        pt U = poly[i], V = poly[(i + 1) % n];
        ld cU = cross(B - A, U - A);
        ld cV = cross(B - A, V - A);
        bool inU = cU >= -EPS, inV = cV >= -EPS;
        if (inU) out.push_back(U);
        if (inU ^ inV) {
            pt dir = V - U;
            ld num = -cross(B - A, U - A);
            ld den = cross(B - A, dir);
            if (fabsl(den) > EPS)
                out.push_back(U + dir * (num / den));
        }
    }
    return out;
}

// نواة مضلع بسيط CCW (تقاطع أنصاف المستويات الداخلية)
// Kernel of a simple CCW polygon (empty if none)
vector<pt> polygonKernel(const vector<pt>& poly) {
    vector<pt> kernel = poly;
    int n = (int)poly.size();
    for (int i = 0; i < n && !kernel.empty(); i++)
        kernel = clipAgainstLine(kernel, poly[i], poly[(i + 1) % n]);
    return kernel;
}

// نواة مضلع محدب (نفس الفكرة)
// Kernel of a convex polygon
vector<pt> kernelOfConvex(const vector<pt>& poly) {
    return polygonKernel(poly);
}

// ============================================================================
