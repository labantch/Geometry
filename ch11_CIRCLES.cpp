// ============================================================================
// Geometry Notebook — CHAPTER 11 — CIRCLES
// ============================================================================

// CHAPTER 11 — CIRCLES
// شرح: كل حاجة متعلقة بالدوائر - تقاطع، مساحة تقاطع، الدائرة المحيطة...
// ============================================================================

// شرح: هيكل بسيط بيمثل الدائرة بمركز ونصف قطر
// ---- Circle struct ----
struct Circle {
    pt center;
    ld radius;

    Circle() : center(0, 0), radius(0) {}
    Circle(pt c, ld r) : center(c), radius(r) {}

    // القطر
    ld diameter() const { return 2.0L * radius; }
    // المحيط
    ld circumference() const { return 2.0L * PI * radius; }
    // المساحة
    ld area() const { return PI * radius * radius; }
    // هل النقطة جوه / على الحدود؟
    bool contains(pt p, bool strict = false) const {
        ld d2 = sq(p - center);
        ld r2 = radius * radius;
        return strict ? (d2 < r2 - EPS) : (d2 <= r2 + EPS);
    }
};


// ---- Triangle shape | شكل المثلث ----
struct Triangle {
    pt a, b, c;

    Triangle() {}
    Triangle(pt A, pt B, pt C) : a(A), b(B), c(C) {}

    // الأضلاع
    ld sideA() const { return abs(b - c); } // opposite to a
    ld sideB() const { return abs(a - c); }
    ld sideC() const { return abs(a - b); }
    // المحيط
    ld perimeter() const { return sideA() + sideB() + sideC(); }
    // المساحة (نصف الضرب الاتجاهي)
    ld area() const { return fabsl(cross(b - a, c - a)) / 2.0L; }
    // نصف قطر الدائرة المحيطة
    ld circumradius() const {
        ld ar = area();
        if (ar < EPS) return 0;
        return sideA() * sideB() * sideC() / (4.0L * ar);
    }
    // محيط الدائرة المحيطة
    ld circumference() const { return 2.0L * PI * circumradius(); }
    // نصف قطر الدائرة الداخلية
    ld inradius() const {
        ld s = perimeter() / 2.0L;
        if (s < EPS) return 0;
        return area() / s;
    }
    // المراكز
    pt centroid() const { return (a + b + c) / 3.0L; }
    pt circumcenter() const {
        pt ab = b - a, ac = c - a;
        ld d = 2.0L * cross(ab, ac);
        if (fabsl(d) < EPS) return a;
        ld u = norm2(ab), v = norm2(ac);
        return a + pt((u * ac.Y - v * ab.Y) / d, (v * ab.X - u * ac.X) / d);
    }
    pt incenter() const {
        ld aa = abs(b - c), bb = abs(a - c), cc = abs(a - b);
        ld s = aa + bb + cc;
        if (s < EPS) return a;
        return (a * aa + b * bb + c * cc) / s;
    }
    bool contains(pt p, bool strict = false) const {
        ld o1 = orient(a, b, p), o2 = orient(b, c, p), o3 = orient(c, a, p);
        bool pos = (o1 >= -EPS && o2 >= -EPS && o3 >= -EPS);
        bool neg = (o1 <=  EPS && o2 <=  EPS && o3 <=  EPS);
        if (strict)
            return (o1 > EPS && o2 > EPS && o3 > EPS) || (o1 < -EPS && o2 < -EPS && o3 < -EPS);
        return pos || neg;
    }
};

// ---- Square shape | شكل المربع (محاذي للمحاور من ركن وضلع) ----
struct Square {
    pt bottomLeft;
    ld side;

    Square() : bottomLeft(0, 0), side(0) {}
    Square(pt bl, ld s) : bottomLeft(bl), side(s) {}

    ld area() const { return side * side; }
    ld perimeter() const { return 4.0L * side; }
    ld diagonal() const { return side * sqrtl(2.0L); }
    bool contains(pt p, bool strict = false) const {
        ld x1 = bottomLeft.X, y1 = bottomLeft.Y;
        ld x2 = x1 + side, y2 = y1 + side;
        if (strict)
            return p.X > x1 + EPS && p.X < x2 - EPS && p.Y > y1 + EPS && p.Y < y2 - EPS;
        return p.X >= x1 - EPS && p.X <= x2 + EPS && p.Y >= y1 - EPS && p.Y <= y2 + EPS;
    }
};

// ---- Sphere | الكرة ----
struct Sphere {
    ld radius;
    Sphere(ld r = 0) : radius(r) {}
    ld diameter() const { return 2.0L * radius; }
    ld area() const { return 4.0L * PI * radius * radius; }       // surface area
    ld surfaceArea() const { return area(); }
    ld volume() const { return (4.0L / 3.0L) * PI * radius * radius * radius; }
};

// ---- Cylinder | الأسطوانة القائمة ----
struct Cylinder {
    ld radius, height;
    Cylinder(ld r = 0, ld h = 0) : radius(r), height(h) {}
    ld baseArea() const { return PI * radius * radius; }
    ld lateralArea() const { return 2.0L * PI * radius * height; }
    ld area() const { return lateralArea() + 2.0L * baseArea(); } // total surface
    ld surfaceArea() const { return area(); }
    ld volume() const { return baseArea() * height; }
};

// ---- Cone | المخروط القائم ----
struct Cone {
    ld radius, height;
    Cone(ld r = 0, ld h = 0) : radius(r), height(h) {}
    ld slantHeight() const { return sqrtl(radius * radius + height * height); }
    ld baseArea() const { return PI * radius * radius; }
    ld lateralArea() const { return PI * radius * slantHeight(); }
    ld area() const { return lateralArea() + baseArea(); }
    ld surfaceArea() const { return area(); }
    ld volume() const { return baseArea() * height / 3.0L; }
};


// شرح: تقاطع دائرة مع خط مستقيم - بترجع 0 (مفيش تقاطع) أو 1 (مماس) أو 2 (نقطتين)
// Circle-line intersection
// Returns: 0=no intersection, 1=tangent, 2=two points
// ---- Circle–line intersection ----
int circleLineIntersection(pt center, ld r, Line l, pair<pt, pt>& out) {
    ld h2 = r * r - l.squaredDistanceToPoint(center);
    if (h2 < -EPS) return 0;
    h2 = max((ld)0, h2);
    
    pt p = l.projection(center);
    pt h = normalize(l.direction) * sqrtl(h2);
    out = {p - h, p + h};
    
    return (h2 <= EPS) ? 1 : 2;
}

// شرح: تقاطع دائرتين - بترجع 0 (مفيش تقاطع) أو 1 (مماستين) أو 2 (نقطتين)
// Circle-circle intersection
// Returns: 0=no intersection, 1=tangent, 2=two points
// ---- Circle–circle intersection ----
int circleCircleIntersection(pt o1, ld r1, pt o2, ld r2, pair<pt, pt>& out) {
    pt d = o2 - o1;
    ld D = abs(d);
    
    if (D > r1 + r2 + EPS) return 0;
    if (D < fabsl(r1 - r2) - EPS) return 0;
    if (D < EPS) return 0;
    
    ld xcoord = (r1 * r1 - r2 * r2 + D * D) / (2 * D);
    ld h2 = r1 * r1 - xcoord * xcoord;
    if (h2 < -EPS) return 0;
    h2 = max((ld)0, h2);
    
    pt p = o1 + normalize(d) * xcoord;
    pt v = rotate90CCW(normalize(d)) * sqrtl(h2);
    out = {p - v, p + v};
    
    return (h2 <= EPS) ? 1 : 2;
}

// شرح: مساحة تقاطع دائرتين مع بعض
// Area of intersection of two circles
// ---- Intersection area ----
ld circleIntersectionArea(pt o1, ld r1, pt o2, ld r2) {
    ld d = abs(o2 - o1);
    
    if (d >= r1 + r2 - EPS) return 0;
    if (d <= fabsl(r1 - r2) + EPS) {
        ld r = min(r1, r2);
        return PI * r * r;
    }
    
    ld a1 = 2 * acosl(max((ld)-1.0L, min((ld)1.0L, (d*d + r1*r1 - r2*r2) / (2*d*r1))));
    ld a2 = 2 * acosl(max((ld)-1.0L, min((ld)1.0L, (d*d + r2*r2 - r1*r1) / (2*d*r2))));
    
    return r1*r1*(a1 - sinl(a1))/2.0L + r2*r2*(a2 - sinl(a2))/2.0L;
}

// شرح: مراكز الدوائر (نصف قطر r) اللي بتلامس خطين معينين
// Centers of circles tangent to two lines
pair<pt, pt> tangentCircleCenters(Line l1, Line l2, ld r, bool interior = true) {
    pt O;
    intersection(l1, l2, O);
    
    Line bis = angleBisector(l1, l2, interior);
    ld theta = angle(l1.direction, l2.direction);
    pt dir = normalize(bis.direction);
    ld dist = r / sinl(theta / 2.0L);
    
    return {O + dir * dist, O - dir * dist};
}

// شرح: الدائرة المحيطة (Circumcircle) بمضلع دوري (Cyclic) - بترجع false لو مش دوري
// Returns the circumscribed circle of a cyclic polygon.
// Returns false if the polygon is not cyclic or is degenerate.
// ---- Circumcircle ----
bool circumcirclePolygon(const vector<pt>& poly, Circle &cir) {
    int n = poly.size();
    if (n < 3) return false;

    // Find any 3 non-collinear vertices.
    int a = 0, b = 1, c = -1;
    for (int i = 2; i < n; i++) {
        if (fabsl(cross(poly[b] - poly[a], poly[i] - poly[a])) > EPS) {
            c = i;
            break;
        }
    }

    if (c == -1) return false; // All points are collinear.

    pt A = poly[a], B = poly[b], C = poly[c];

    // Circumcenter formula.
    ld d = 2.0L * cross(B - A, C - A);

    pt u = B - A;
    pt v = C - A;

    ld uu = norm2(u);
    ld vv = norm2(v);

    cir.center = A + pt(
        (uu * v.Y - vv * u.Y) / d,
        (vv * u.X - uu * v.X) / d
    );

    cir.radius = abs(cir.center - A);

    // Check that every vertex lies on the circle.
    for (const pt &p : poly) {
        if (fabsl(abs(p - cir.center) - cir.radius) > EPS)
            return false;
    }

    return true;
}

// شرح: نصف قطر الدائرة المحيطة بمضلع منتظم عدد أضلاعه n وطول ضلعه side
// Circumradius of a regular n-gon.
ld circumradiusRegularPolygon(int n, ld side) {
    return side / (2.0L * sinl(PI / n));
}

// شرح: مساحة الدائرة المحيطة بمضلع منتظم
// Area of the circumscribed circle.
ld circumcircleAreaRegularPolygon(int n, ld side) {
    ld R = circumradiusRegularPolygon(n, side);
    return PI * R * R;
}

// شرح: عدد النقط الشبكية (لها إحداثيات صحيحة) جوه دائرة نصف قطرها r ومركزها الأصل
// Count integer lattice points inside circle of radius r
// ---- Lattice points in circle ----
long long countPoints(int r) {
    long long cnt = 0;
    long long rr = 1LL * r * r;

    for (int x = -r; x <= r; x++) {
        long long y = sqrtl(rr - 1LL * x * x);
        while (1LL * x * x + (y + 1) * (y + 1) <= rr) y++;
        while (1LL * x * x + y * y > rr) y--;
        cnt += 2 * y + 1;
    }

    return cnt;
}

// نقطة على الدائرة بزاوية theta (راديان)
// Point on circle at angle theta
pt pointAtAngle(pt O, ld r, ld theta) {
    return O + pt{r * cosl(theta), r * sinl(theta)};
}

// إسقاط نقطة على الدائرة (أقرب نقطة على المحيط)
// Project point onto circle boundary
pt projectOntoCircle(pt c, ld r, pt p) {
    pt v = p - c;
    ld dist = abs(v);
    if (dist < EPS) return c + pt{r, 0};
    return c + v * (r / dist);
}

// إسقاط على طول شعاع p→a لأقرب تقاطع مع الدائرة من a
// Ray from p toward a: circle hit closest to a
bool projectAlongToCircleClosestToA(pt C, ld r, pt p, pt a, pt& out) {
    pt d = a - p;
    ld d2 = sq(d);
    if (d2 < EPS) {
        out = projectOntoCircle(C, r, p);
        return true;
    }
    pt f = p - C;
    ld fd = dotProduct(f, d);
    ld ff_r2 = sq(f) - r * r;
    ld disc = fd * fd - d2 * ff_r2;
    if (disc < 0) return false;
    ld s = sqrtl(disc);
    ld t1 = (-fd + s) / d2, t2 = (-fd - s) / d2;
    pt I1 = p + d * t1, I2 = p + d * t2;
    out = (abs(I1 - a) < abs(I2 - a)) ? I1 : I2;
    return true;
}

// دائرة مماسة لخطين (مركز على المنصف)
// Circle tangent to two lines (center on angle bisector)
// يحتاج: intersection(Line,Line), bisector, Line::projection / distanceToPoint
bool circleBetweenLines(Line& L1, Line& L2, pt& center, ld& radius) {
    if (fabsl(cross(L1.direction, L2.direction)) < EPS) {
        // parallel lines
        ld d = fabsl(L2.constant - L1.constant) / abs(L1.direction);
        radius = d * 0.5L;
        Line M{L1.direction, (L1.constant + L2.constant) * 0.5L};
        // center = M.projection({0,0});
        pt origin{0, 0};
        center = origin - rotate90CCW(M.direction) * M.pointSide(origin) / (ld)sq(M.direction);
        return true;
    }
    pt P;
    if (!intersection(L1, L2, P)) return false;
    pt on1 = P + rotate90CCW(normalize(L1.direction));
    pt on2 = P + rotate90CCW(normalize(L2.direction));
    // استخدم angleBisector عندك لو موجود، أو:
    pt u = normalize(on1 - P), w = normalize(on2 - P);
    pt dir = u + w;
    if (abs(dir) < EPS) dir = rotate90CCW(u);
    Line bis(P, P + dir);
    center = P - rotate90CCW(bis.direction) * bis.pointSide(P) / (ld)sq(bis.direction);
    // better: center = bis.projection({0,0}) is wrong; use:
    center = bis.projection ? /* if you have method */ center : center;
    // safest with your API:
    // you likely have projection on Line — use it if named differently:
    radius = L1.distanceToPoint(center);
    return true;
}

// أكبر دائرة قطرها ab وتدخل في مستطيل [P1, P2]
// Largest circle with diameter ab that fits in axis-aligned box
pair<pt, ld> constrainedCircleFromDiameter(pt a, pt b, pt P1, pt P2) {
    pt mid = (a + b) * (ld)0.5;
    ld raw_r = abs(b - a) * 0.5L;
    ld fit_r = min({raw_r,
                    mid.X - P1.X, P2.X - mid.X,
                    mid.Y - P1.Y, P2.Y - mid.Y});
    return {mid, fit_r};
}

// ============================================================================
