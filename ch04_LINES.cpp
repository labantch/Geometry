// ============================================================================
// Geometry Notebook — CHAPTER 4 — LINES
// ============================================================================

// CHAPTER 4 — LINES
// شرح: تمثيل الخط المستقيم (Line) وكل العمليات عليه (مسافة، تقاطع، انعكاس...)
// ============================================================================

// Forward declaration of Line struct
struct Line;

// Intersection point of two lines - forward declaration
// ---- Line intersection ----
bool intersection(Line l1, Line l2, pt &out);

// شرح: الكلاس الأساسي اللي بيمثل خط مستقيم بمتجه اتجاه وثابت (constant)
// ---- Line struct ----
struct Line {
    pt direction;   // Direction vector
    ld constant;    // cross(direction, point) = constant
    
    // Constructors
    Line(pt direction, ld constant) : direction(direction), constant(constant) {}
    Line(ld a, ld b, ld c) : direction({b, -a}), constant(c) {}
    Line(pt p, pt q) : direction(q - p), constant(cross(direction, p)) {}
    
    // شرح: هل النقطة على يمين الخط ولا شمال ولا عليه بالظبط
    // Point side: >0 left, <0 right, =0 on line
    ld pointSide(pt p) {
        return cross(direction, p) - constant;
    }
    
    // شرح: المسافة العمودية من نقطة للخط
    // Perpendicular distance from point to line
    ld distanceToPoint(pt p) {
        return fabsl(pointSide(p)) / abs(direction);
    }

    // Signed perpendicular distance (positive on left of direction, negative on right)
    ld signedDistance(pt p) {
        return pointSide(p) / abs(direction);
    }
    
    // شرح: مربع المسافة (أسرع، من غير sqrt)
    // Squared distance (faster, no sqrt)
    ld squaredDistanceToPoint(pt p) {
        ld s = pointSide(p);
        return s * s / (ld)sq(direction);
    }
    
    // شرح: بيرجع خط عمودي على هذا الخط ومار بنقطة p
    // Line perpendicular to this line through point p
    Line perpendicularThrough(pt p) {
        return {p, p + rotate90CCW(direction)};
    }
    
    // شرح: بيقارن إسقاط نقطتين على اتجاه الخط (أيهما أبعد في اتجاه الخط)
    // Compare projections of points onto line direction
    bool compareProjection(pt p, pt q) {
        return dotProduct(direction, p) < dotProduct(direction, q);
    }
    
    // شرح: بيزيح الخط بالكامل بمتجه إزاحة معين
    // Translate line by offset vector
    Line translated(pt offset) {
        return {direction, constant + cross(direction, offset)};
    }
    
    // شرح: بيزيح الخط لجهة اليسار بمسافة عمودية معينة
    // Shift line left by given perpendicular distance
    Line shiftLeft(ld distance) {
        return {direction, constant + distance * abs(direction)};
    }
    
    // شرح: إسقاط عمودي لنقطة p على الخط
    // Orthogonal projection of point p onto line
    pt projection(pt p) {
        return p - rotate90CCW(direction) * pointSide(p) / (ld)sq(direction);
    }
    
    // شرح: انعكاس النقطة p حوالين الخط (المرآة)
    // Reflection of point p across line
    pt reflection(pt p) {
        return p - rotate90CCW(direction) * (ld)2 * pointSide(p) / (ld)sq(direction);
    }

    // شرح: بيرجع معاملات معادلة الخط A, B, C بحيث Ax + By + C = 0
    // Get line coefficients: A, B, C where Ax + By + C = 0
    tuple<ld, ld, ld> coefficients() const {
        return {-direction.Y, direction.X, -constant};
    }
};

// شرح: معادلة الخط المار بنقطتين a و b على هيئة Ax + By + C = 0
// Get line equation coefficients: Ax + By + C = 0
// ---- Line equation ----
tuple<ld, ld, ld> lineEquation(pt a, pt b) {
    return {-b.Y + a.Y, b.X - a.X, -(a.X * (b.Y - a.Y) - a.Y * (b.X - a.X))};
}

// شرح: معادلة الخط لما تعرف نقطة عليه ومتجه عمودي (Normal) عليه
// Get line equation from point and normal vector
tuple<ld, ld, ld> lineEquationNormal(pt p, pt normal) {
    ld A = normal.X;
    ld B = normal.Y;
    ld C = -(A * p.X + B * p.Y);
    return {A, B, C};
}

// شرح: نقطة تقاطع خطين، بترجع false لو الخطين متوازيين
// Intersection point of two lines
bool intersection(Line l1, Line l2, pt &out) {
    ld det = cross(l1.direction, l2.direction);
    if (fabsl(det) < EPS) return false;
    out = (l2.direction * l1.constant - l1.direction * l2.constant) / det;
    return true;
}

// شرح: منصف الزاوية بين خطين متقاطعين (داخلي أو خارجي حسب interior)
// Angle bisector of two intersecting lines
// interior=true: internal bisector, false: external bisector
// ---- Angle bisectors ----
Line angleBisector(Line l1, Line l2, bool interior) {
    assert(fabsl(cross(l1.direction, l2.direction)) > EPS);
    ld sign = interior ? 1 : -1;
    return {
        l2.direction / abs(l2.direction) + l1.direction / abs(l1.direction) * sign,
        l2.constant / abs(l2.direction) + l1.constant / abs(l1.direction) * sign
    };
}

// شرح: منصف زاوية ∠BAC (داخلي افتراضيًا، أو خارجي لو internal=false)
// Angle bisector of ∠BAC
// internal = true  -> internal angle bisector
// internal = false -> external angle bisector
// Returns the line passing through A that bisects the angle
// formed by the rays (or segments) AB and AC.
Line angleBisector(pt A, pt B, pt C, bool internal = true) {
    pt u = normalize(B - A);
    pt v = normalize(C - A);

    pt dir = internal ? (u + v) : (u - v);

    // Degenerate case:
    // Internal: AB and AC are opposite (180°).
    // External: AB and AC have the same direction (0°).
    // In both cases u ± v = (0,0), so choose any perpendicular direction.
    if (abs(dir) < EPS)
        dir = rotate90CCW(u);

    return Line(A, A + dir);
}

// شرح: خطين متوازيين على بعد مسافة r من كل جهة للخط الأصلي l
// ---- Parallel lines ----
pair<Line, Line> parallelLinesAtDistance(Line l, ld r) {
    return {l.shiftLeft(r), l.shiftLeft(-r)};
}

// Check if two lines are parallel
bool areParallel(Line l1, Line l2) {
    return fabsl(cross(l1.direction, l2.direction)) < EPS;
}

// Free-function signed distance from point to line
ld signedDistance(Line l, pt p) {
    return l.signedDistance(p);
}

// شرح: نفس فكرة الخطين المتوازيين لكن كمعاملات A,B,C بدل كائن Line
pair<tuple<ld, ld, ld>, tuple<ld, ld, ld>>
parallelLineCoefficients(ld A, ld B, ld C, ld R) {
    ld delta = R * sqrtl(A * A + B * B);

    return {
        {A, B, C + delta},
        {A, B, C - delta}
    };
}

// ميل الخط بين نقطتين (DBL_MAX لو رأسي)
// Slope of line through p1,p2 (huge value if vertical)
ld getLineSlope(pt p1, pt p2) {
    pt d = p2 - p1;
    if (fabsl(d.X) < EPS) return 1e100L; // vertical
    return d.Y / d.X;
}

// نقطتين على الخط ax + by + c = 0
// Two points on the line ax + by + c = 0
void getTwoPointsFromEquation(ld a, ld b, ld c, pt& p1, pt& p2) {
    if (fabsl(b) > EPS) {
        p1 = {0, -c / b};
        p2 = {1, -(a + c) / b};
    } else {
        ld x = -c / a;
        p1 = {x, 0};
        p2 = {x, 1};
    }
}

// نقطتين على خط ممثّل بـ direction + constant
// Two points on a Line (direction, constant)
void getTwoPointsFromDirectionVector(Line l, pt& p1, pt& p2) {
    pt origin = {0, 0};
    ld mag2 = sq(l.direction);
    pt onLine = origin + rotate90CCW(l.direction) * (l.constant / mag2);
    // NOTE: depends on your Line convention; if projection exists use it:
    // pt onLine = l.projection({0, 0});
    pt dir = normalize(l.direction);
    p1 = onLine + dir;
    p2 = onLine - dir;
}


// ============================================================================
