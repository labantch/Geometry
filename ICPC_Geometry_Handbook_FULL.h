// ============================================================================
//   ICPC GEOMETRY HANDBOOK
//   ------------------------------------------------------------------------
//   A single-header computational geometry library, organized as a book.
//   Rebuilt from Geeo.cpp + the split headers (geometry_base/lines_segments/
//   polygons/circles/advanced/polygon_inscribe/Simple), with duplicate
//   functions removed and a one-line Arabic note above every function
//   explaining what it does. All original English comments are kept.
//
//   شرح عام: مكتبة هندسة حسابية (Computational Geometry) كاملة في هيدر واحد،
//   متقسمة لفصول زي الكتاب، من غير أي تكرار، وكل فنكشن ليها سطر شرح بالعربي.
//
//   TABLE OF CONTENTS
//   ------------------------------------------------------------------------
//   CH 0  Setup & Constants
//   CH 1  Basic Vector Operations
//   CH 2  Transformations
//   CH 3  Angle Operations
//   CH 4  Lines
//   CH 5  Rays
//   CH 6  Segments
//   CH 7  Polygons: Area, Perimeter, Point-in-Polygon
//   CH 8  Convex Hull
//   CH 9  Minkowski Sum
//   CH 10 Polygon Distance Operations
//   CH 11 Circles
//   CH 12 Lattice Points (Pick's Theorem)
//   CH 13 Half-Plane Intersection
//   CH 14 Closest Pair of Points
//   CH 15 Rectangles
//   CH 16 Batch Line Intersections
//   CH 17 Convex Polygon Utilities (rotating calipers)
//   CH 18 Inscribed Shapes
//   CH 19 Quick Formula Reference (no vectors needed)
//   ------------------------------------------------------------------------
//   USAGE: #include "ICPC_Geometry_Handbook.h" then use `pt` for 2D points
//   (it's std::complex<long double> under the hood -> +,-,*,/ all work).
// ============================================================================

#ifndef ICPC_GEOMETRY_HANDBOOK_H
#define ICPC_GEOMETRY_HANDBOOK_H

#include <bits/stdc++.h>
using namespace std;

// ============================================================================
// CHAPTER 0 — SETUP & CONSTANTS
// شرح: التعريفات الأساسية اللي كل حاجة في المكتبة بتتبني عليها
// ============================================================================

// ---- Types & constants ----
typedef long double ld;
typedef complex<ld> pt;

const ld EPS = 1e-9L;
const ld PI  = acosl(-1.0L);

// Helper functions to access real and imaginary parts
// شرح: اختصار عشان نقدر نكتب p.X و p.Y بدل p.real() و p.imag()
// ---- Accessors ----
#define X real()
#define Y imag()

// ============================================================================
// CHAPTER 1 — BASIC VECTOR OPERATIONS
// شرح: عمليات المتجهات (Vectors) الأساسية اللي بتتبني عليها كل حاجة تانية
// ============================================================================

// شرح: بيرجع مربع طول المتجه (من غير sqrt، أسرع للمقارنات)
// Squared length of a vector
// ---- Length ----
ld sq(pt p) {
    return p.X * p.X + p.Y * p.Y;
}

// شرح: بيرجع طول المتجه الحقيقي (فيه sqrt)
// Returns the length (magnitude) of vector v.
ld norm(pt v) {
    return abs(v);
}

// شرح: حاصل الضرب القياسي (Dot Product) بين متجهين a و b
// Dot product: a·b = |a||b|cos(θ)
// > 0: angle < 90°, = 0: perpendicular, < 0: angle > 90°
// ---- Dot & cross ----
ld dotProduct(pt a, pt b) {
    return a.X * b.X + a.Y * b.Y;
}

// شرح: بيرجع مربع طول المتجه، بديل أسرع لـ norm() لما بتقارن مسافات بس
// Returns the squared length of vector v.
// Faster than norm() when comparing distances.
ld norm2(pt v) {
    return dotProduct(v, v);
}

// شرح: حاصل الضرب الاتجاهي (Cross Product) بين متجهين a و b
// Cross product: a×b = |a||b|sin(θ)
// > 0: left turn (CCW), < 0: right turn (CW), = 0: collinear
ld cross(pt a, pt b) {
    return a.X * b.Y - a.Y * b.X;
}

// شرح: بيرجع المتجه الوحدة (طوله 1) في نفس اتجاه v
// Returns the unit vector in the direction of v.
// Requires v != (0, 0).
// ---- Normalize & sign ----
pt normalize(pt v) {
    return v / abs(v);
}

// شرح: دالة الإشارة مع هامش خطأ EPS عشان نتجنب أخطاء الفاصلة العشرية
// Sign function with EPS tolerance
int sgn(ld val) {
    if (val > EPS) return 1;
    if (val < -EPS) return -1;
    return 0;
}

// شرح: بيتأكد إذا كان المتجهين متعامدين (زاوية 90 درجة) ولا لأ
// Check if two vectors are perpendicular
// ---- Perpendicular & rotate ----
bool arePerpendicular(pt a, pt b) {
    return fabsl(dotProduct(a, b)) < EPS;
}

// شرح: بيلف المتجه 90 درجة عكس عقارب الساعة
// Rotate vector 90° counter-clockwise: (x,y) → (-y,x)
pt rotate90CCW(pt v) {
    return {-v.Y, v.X};
}

// Rotate vector 90° clockwise: (x,y) → (y, -x)
pt rotate90CW(pt v) {
    return {v.Y, -v.X};
}

// Check if two vectors are parallel (or anti-parallel)
bool areParallel(pt a, pt b) {
    return fabsl(cross(a, b)) < EPS;
}

// شرح: إسقاط (Projection) المتجه a على اتجاه المتجه b
// Returns the projection of vector a onto vector b.
// ---- Projection & rejection ----
pt vectorProjection(pt a, pt b) {
    return b * (dotProduct(a, b) / norm2(b));
}

// شرح: الجزء من المتجه a اللي عمودي على b (المتبقي بعد الإسقاط)
// Returns the component of a perpendicular to b.
pt rejection(pt a, pt b) {
    return a - vectorProjection(a, b);
}


// ============================================================================
// CHAPTER 2 — TRANSFORMATIONS
// شرح: عمليات تحويل النقط زي الإزاحة والتكبير والدوران
// ============================================================================

// شرح: بيزيح النقطة p بمقدار المتجه v
// Translate point p by vector v
// ---- Translate / scale / rotate ----
pt translate(pt v, pt p) {
    return p + v;
}

// شرح: بيكبر/يصغر النقطة p حوالين مركز c بمعامل factor
// Scale point p relative to center c by factor
pt scale(pt c, ld factor, pt p) {
    return c + (p - c) * factor;
}

// شرح: بيدور النقطة p حوالين مركز c بزاوية a بالراديان (عكس عقارب الساعة)
// Rotate point p around center c by angle a (radians, CCW)
pt rot(pt p, pt c, ld a) {
    pt v = p - c;
    pt rotate = {cosl(a), sinl(a)};
    return c + rotate * v;
}

// شرح: تحويل خطي بيحول القطعة (p→q) للقطعة (fp→fq) وبيطبق نفس التحويل على r
// Linear transformation mapping segment (p→q) to (fp→fq)
// Applies same translation, rotation, and scaling to point r
// ---- Linear transform ----
pt linearTransfo(pt p, pt q, pt r, pt fp, pt fq) {
    return fp + (r - p) * (fq - fp) / (q - p);
}

// ============================================================================
// CHAPTER 3 — ANGLE OPERATIONS
// شرح: كل حاجة متعلقة بالزوايا بين النقط والمتجهات
// ============================================================================

// شرح: اتجاه (Orientation) ثلاث نقط: هل الدوران يمين ولا شمال ولا على استقامة واحدة
// Orientation of three points: cross(b-a, c-a)
// > 0: left turn (CCW), < 0: right turn (CW), = 0: collinear
// ---- Orientation ----
ld orient(pt a, pt b, pt c) {
    return cross(b - a, c - a);
}

// شرح: أصغر زاوية بين متجهين، النتيجة دايمًا بين 0 و π
// Smallest angle between two vectors [0, π]
// ---- Angles ----
ld angle(pt v, pt w) {
    ld cos_theta = dotProduct(v, w) / abs(v) / abs(w);
    return acosl(max((ld)-1.0L, min((ld)1.0L, cos_theta)));
}

// شرح: الزاوية القطبية للنقطة p، متطبَّعة عشان تبقى بين 0 و 2π
// Polar angle of point p normalized to [0, 2π)
ld polarAngle(pt p) {
    ld ang = arg(p);
    if (ang < 0) ang += 2 * PI;
    return ang;
}

// شرح: الزاوية الموجهة من المتجه AB للمتجه AC عكس عقارب الساعة، بين 0 و 2π
// Oriented angle from vector AB to AC, CCW in [0, 2π)
ld orientedAngle(pt a, pt b, pt c) {
    ld theta = angle(b - a, c - a);
    if (orient(a, b, c) > 0) return theta;
    return 2 * PI - theta;
}

// شرح: الزاوية بإشارتها (موجبة أو سالبة) من AB لـ AC، بين -π و π
// Signed angle from vector AB to AC in [-π, π]
ld angleTravelled(pt a, pt b, pt c) {
    ld theta = angle(b - a, c - a);
    if (orient(a, b, c) > 0) return theta;
    return -theta;
}

// شرح: بيتأكد إذا كانت النقطة p واقعة جوه زاوية ∠BAC (شامل الحدود)
// Check if point p lies inside angle ∠BAC (including boundary)
// ---- Point in angle ----
bool inAngle(pt a, pt b, pt c, pt p) {
    ld abp = orient(a, b, p);
    ld acp = orient(a, c, p);
    ld abc = orient(a, b, c);
    
    if (abc < 0) swap(abp, acp);
    return (abp >= 0 && acp <= 0) ^ (abc < 0);
}


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


// ============================================================================
// CHAPTER 9 — MINKOWSKI SUM
// شرح: جمع منكوفسكي (Minkowski Sum) بين مضلعين محدبين
// ============================================================================

// شرح: بيرتب المضلع بحيث يبدأ من أدنى نقطة فيه (مطلوب قبل عمل Minkowski Sum)
// Reorder polygon starting from lowest point (for Minkowski sum)
// ---- Minkowski sum ----
void reorderPolygon(vector<pt>& P) {
    size_t pos = 0;
    for (size_t i = 1; i < P.size(); i++) {
        if (P[i].Y < P[pos].Y || (P[i].Y == P[pos].Y && P[i].X < P[pos].X))
            pos = i;
    }
    rotate(P.begin(), P.begin() + pos, P.end());
}

// شرح: جمع منكوفسكي بين مضلعين محدبين (ترتيبهم CCW) في O(n+m)
// Minkowski sum of two convex polygons (CCW order)
vector<pt> minkowskiSum(vector<pt> P, vector<pt> Q) {
    reorderPolygon(P);
    reorderPolygon(Q);
    
    P.push_back(P[0]);
    P.push_back(P[1]);
    Q.push_back(Q[0]);
    Q.push_back(Q[1]);
    
    vector<pt> result;
    size_t i = 0, j = 0;
    
    while (i < P.size() - 2 || j < Q.size() - 2) {
        result.push_back(P[i] + Q[j]);
        ld cr = cross(P[i + 1] - P[i], Q[j + 1] - Q[j]);
        if (cr >= 0 && i < P.size() - 2) ++i;
        if (cr <= 0 && j < Q.size() - 2) ++j;
    }
    return result;
}

// ============================================================================
// CHAPTER 10 — POLYGON DISTANCE OPERATIONS
// شرح: حساب المسافة بين نقطة ومضلع، أو بين مضلعين
// ============================================================================

// شرح: أقل مسافة من نقطة p لمضلع (محدب أو غير محدب)، ممكن ترجع أقرب نقطة كمان
// Minimum distance between a point and a polygon (convex or simple)
// Returns distance, and optionally the closest point on the polygon
// ---- Point to polygon ----
ld distancePointToPolygon(const vector<pt>& poly, pt p, pt* closest = nullptr) {
    int n = poly.size();
    if (n == 0) return numeric_limits<ld>::max();
    
    ld minDist = numeric_limits<ld>::max();
    pt closestPt;
    
    for (int i = 0; i < n; i++) {
        pt a = poly[i];
        pt b = poly[(i + 1) % n];
        
        // Distance from point to segment
        ld dist = distancePointToSegment(a, b, p);
        
        if (dist < minDist) {
            minDist = dist;
            
            // Find closest point on segment if requested
            if (closest) {
                pt proj = Line(a, b).projection(p);
                if (onSegment(a, b, proj)) {
                    closestPt = proj;
                } else {
                    closestPt = (abs(p - a) < abs(p - b)) ? a : b;
                }
            }
        }
    }
    
    if (closest) *closest = closestPt;
    return minDist;
}

// شرح: أقل مسافة بين مضلعين محدبين باستخدام Rotating Calipers في O(n+m)
// Minimum distance between two convex polygons
// Uses rotating calipers approach for O(n+m) complexity
// ---- Convex polygon distance ----
ld distanceConvexPolygons(vector<pt>& P, vector<pt>& Q) {
    int n = P.size(), m = Q.size();
    
    // Check for intersection first
    // If polygons intersect, distance is 0
    for (const auto& p : P) {
        if (pointInPolygon(Q, p, false)) return 0;
    }
    for (const auto& q : Q) {
        if (pointInPolygon(P, q, false)) return 0;
    }
    
    // Check if any edge intersects
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            pt temp;
            if (properIntersection(P[i], P[(i+1)%n], Q[j], Q[(j+1)%m], temp))
                return 0;
        }
    }
    
    // Find minimum distance using rotating calipers
    ld minDist = numeric_limits<ld>::max();
    
    // Find starting points
    int p = 0, q = 0;
    for (int i = 1; i < n; i++) {
        if (P[i].Y < P[p].Y || (P[i].Y == P[p].Y && P[i].X < P[p].X))
            p = i;
    }
    for (int j = 1; j < m; j++) {
        if (Q[j].Y > Q[q].Y || (Q[j].Y == Q[q].Y && Q[j].X > Q[q].X))
            q = j;
    }
    
    // Rotating calipers
    for (int step = 0; step < n + m; step++) {
        // Check distance between vertices
        minDist = min(minDist, abs(P[p] - Q[q]));
        
        // Check distance from vertex to edge
        pt nextP = P[(p + 1) % n];
        pt nextQ = Q[(q + 1) % m];
        
        // Move the polygon with the smaller angle
        ld angle = cross(nextP - P[p], nextQ - Q[q]);
        if (angle >= 0) {
            p = (p + 1) % n;
        } else {
            q = (q + 1) % m;
        }
    }
    
    return minDist;
}

// شرح: أقل مسافة بين مضلعين غير محدبين (بأي شكل)، أبطأ (O(n*m)) لكنها عامة
// Minimum distance between two simple polygons (not necessarily convex)
// O(n*m) complexity - works for any simple polygons
// ---- Simple polygon distance ----
ld distanceSimplePolygons(const vector<pt>& P, const vector<pt>& Q) {
    int n = P.size(), m = Q.size();
    
    // Check for intersection first
    for (int i = 0; i < n; i++) {
        if (pointInPolygon(Q, P[i], false)) return 0;
    }
    for (int i = 0; i < m; i++) {
        if (pointInPolygon(P, Q[i], false)) return 0;
    }
    
    // Check edge intersections
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            pt temp;
            if (properIntersection(P[i], P[(i+1)%n], Q[j], Q[(j+1)%m], temp))
                return 0;
        }
    }
    
    // Check all vertex-edge distances
    ld minDist = numeric_limits<ld>::max();
    
    // Vertices of P to edges of Q
    for (int i = 0; i < n; i++) {
        minDist = min(minDist, distancePointToPolygon(Q, P[i]));
    }
    
    // Vertices of Q to edges of P
    for (int i = 0; i < m; i++) {
        minDist = min(minDist, distancePointToPolygon(P, Q[i]));
    }
    
    return minDist;
}

// شرح: دالة ذكية بتختار تلقائيًا أسرع خوارزمية حسب كون المضلعين محدبين ولا لأ
// Wrapper function that automatically chooses the best algorithm
// If both polygons are convex, uses O(n+m) algorithm
// Otherwise uses O(n*m) algorithm
// ---- Auto dispatcher ----
ld distanceBetweenPolygons(vector<pt>& P, vector<pt>& Q) {
    // Check if both polygons are convex
    bool convexP = true, convexQ = true;
    
    // Check convexity of P
    int n = P.size();
    if (n >= 3) {
        int sign = 0;
        for (int i = 0; i < n; i++) {
            ld o = orient(P[i], P[(i+1)%n], P[(i+2)%n]);
            if (fabsl(o) > EPS) {
                int currentSign = (o > 0) ? 1 : -1;
                if (sign == 0) sign = currentSign;
                else if (sign != currentSign) {
                    convexP = false;
                    break;
                }
            }
        }
    }
    
    // Check convexity of Q
    int m = Q.size();
    if (m >= 3) {
        int sign = 0;
        for (int i = 0; i < m; i++) {
            ld o = orient(Q[i], Q[(i+1)%m], Q[(i+2)%m]);
            if (fabsl(o) > EPS) {
                int currentSign = (o > 0) ? 1 : -1;
                if (sign == 0) sign = currentSign;
                else if (sign != currentSign) {
                    convexQ = false;
                    break;
                }
            }
        }
    }
    
    if (convexP && convexQ && n >= 3 && m >= 3) {
        return distanceConvexPolygons(P, Q);
    } else {
        return distanceSimplePolygons(P, Q);
    }
}


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


// ============================================================================
// CHAPTER 12 — LATTICE POINTS (Pick's Theorem)
// شرح: نظرية Pick لحساب النقط الشبكية جوه وعلى حدود مضلع
// ============================================================================

// شرح: عدد النقط الشبكية على القطعة AB (شاملة A ومش شاملة B)
// Number of lattice points on segment AB (includes A, excludes B)
// ---- Boundary lattice points ----
long long latticeOnSegment(pt a, pt b) {
    return gcd((long long)llroundl(abs(a.X - b.X)),
               (long long)llroundl(abs(a.Y - b.Y)));
}

// شرح: عدد النقط الشبكية على حدود مضلع شبكي (Lattice Polygon)
// Boundary lattice points of a lattice polygon
long long boundaryLatticePoints(const vector<pt>& poly) {
    long long cnt = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++)
        cnt += latticeOnSegment(poly[i], poly[(i + 1) % n]);
    return cnt;
}

// شرح: عدد النقط الشبكية جوه المضلع باستخدام نظرية Pick: A = I + B/2 - 1
// Interior lattice points using Pick's Theorem: A = I + B/2 - 1
// ---- Interior (Pick's theorem) ----
long long interiorLatticePoints(const vector<pt>& poly) {
    long long B = boundaryLatticePoints(poly);
    ld A = polygonArea(poly);
    return llroundl(A - (ld)B / 2.0L + 1.0L);
}

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
// CHAPTER 14 — CLOSEST PAIR OF POINTS
// شرح: أقرب زوج نقط في مجموعة نقط باستخدام Divide & Conquer في O(n log n)
// ============================================================================

// شرح: مربع أقل مسافة بين أي زوج نقط - O(n log n)
// Closest pair of points - O(n log n)
// ---- Closest pair ----
ld closestPairSquared(vector<pt> p) {
    sort(p.begin(), p.end(), [](pt a, pt b) {
        if (a.X != b.X) return a.X < b.X;
        return a.Y < b.Y;
    });
    
    set<pair<ld, ld>> window;  // {y, x}
    ld ans = numeric_limits<ld>::max();
    ld best = numeric_limits<ld>::max();
    int j = 0;
    
    for (int i = 0; i < (int)p.size(); i++) {
        while (j < i && p[i].X - p[j].X > best) {
            window.erase({p[j].Y, p[j].X});
            j++;
        }
        
        auto it1 = window.lower_bound({p[i].Y - best, -numeric_limits<ld>::max()});
        auto it2 = window.upper_bound({p[i].Y + best, numeric_limits<ld>::max()});
        
        for (auto it = it1; it != it2; ++it) {
            pt cur(it->second, it->first);
            ans = min(ans, sq(p[i] - cur));
        }
        
        if (ans != numeric_limits<ld>::max())
            best = sqrtl((ld)ans);
        
        window.insert({p[i].Y, p[i].X});
    }
    
    return ans;
}

// شرح: أقل مسافة حقيقية بين أي زوج نقط (فيها sqrt)
ld closestPairDistance(vector<pt> p) {
    return sqrtl((ld)closestPairSquared(p));
}

// ============================================================================
// CHAPTER 15 — RECTANGLES
// شرح: عمليات على المستطيلات المحاذية للمحاور (Axis-Aligned Rectangles)
// ============================================================================

// شرح: هيكل يمثل مستطيل محاذي للمحاور بإحداثيات ريل (long double)
// Axis-aligned rectangle operations
// ---- Axis-aligned rectangle ----
struct Rectangle {
    ld x1, y1, x2, y2; // corners (axis-aligned)

    Rectangle() : x1(0), y1(0), x2(0), y2(0) {}
    Rectangle(ld a, ld b, ld c, ld d) : x1(a), y1(b), x2(c), y2(d) {}

    ld width()  const { return fabsl(x2 - x1); }
    ld height() const { return fabsl(y2 - y1); }
    // المساحة
    ld area() const { return width() * height(); }
    // المحيط
    ld perimeter() const { return 2.0L * (width() + height()); }
    // القطر
    ld diagonal() const { return sqrtl(width() * width() + height() * height()); }
    bool contains(pt p, bool strict = false) const {
        ld xl = min(x1, x2), xr = max(x1, x2);
        ld yb = min(y1, y2), yt = max(y1, y2);
        if (strict)
            return p.X > xl + EPS && p.X < xr - EPS && p.Y > yb + EPS && p.Y < yt - EPS;
        return p.X >= xl - EPS && p.X <= xr + EPS && p.Y >= yb - EPS && p.Y <= yt + EPS;
    }
};

// شرح: مساحة تقاطع مستطيلين محاذيين للمحاور
// Intersection area of two axis-aligned rectangles
ld rectangleIntersectionArea(Rectangle a, Rectangle b) {
    ld xcoord = max((ld)0, min(a.x2, b.x2) - max(a.x1, b.x1));
    ld ycoord = max((ld)0, min(a.y2, b.y2) - max(a.y1, b.y1));
    return xcoord * ycoord;
}

// شرح: نفس فكرة المستطيل لكن بإحداثيات صحيحة (int) - مفيدة لخوارزميات الفرز
// Union area of axis-aligned rectangles (integer coordinates)
// Requires segment tree implementation for full functionality
// ---- Integer rectangles / union ----
struct RectInt {
    int x1, y1, x2, y2;
};

// شرح: مساحة اتحاد مجموعة مستطيلات - ملحوظة: لسه ناقصها تطبيق شجرة القطاعات
// (segment tree) عشان تشتغل فعليًا؛ حاليًا مجرد هيكل الأحداث (events) بس
// NOTE: incomplete placeholder — needs a segment tree with range-add and
// total-covered-length query to actually compute the union area.
long long unionRectangleArea(vector<RectInt> rects) {
    struct Event {
        int x, type, y1, y2;
        Event(int _x, int _type, int _y1, int _y2) : x(_x), type(_type), y1(_y1), y2(_y2) {}
    };
    
    const int SHIFT = 1000000;
    
    vector<Event> events;
    for (auto& r : rects) {
        events.push_back(Event(r.x1, 1, r.y1 + SHIFT, r.y2 + SHIFT));
        events.push_back(Event(r.x2, 0, r.y1 + SHIFT, r.y2 + SHIFT));
    }
    
    sort(events.begin(), events.end(), [](Event a, Event b) {
        if (a.x != b.x) return a.x < b.x;
        return a.type > b.type;  // Add before remove
    });
    
    // Note: Full implementation requires segment tree with range add and total length query
    // This is a placeholder showing the structure
    return 0;  // Placeholder
}


// ============================================================================
// CHAPTER 16 — BATCH LINE INTERSECTIONS
// شرح: إيجاد نقط تقاطع مجموعة خطوط مع بعض
// ============================================================================

// شرح: بيتأكد إذا كان فيه أي زوج خطين بيتقاطعوا في نقطة واحدة محددة
// Check if any pair of lines intersect (unique intersection)
// ---- Any intersection ----
bool anyLineIntersection(const vector<Line>& lines, pt& out) {
    int n = lines.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (intersection(lines[i], lines[j], out))
                return true;
        }
    }
    return false;
}

// شرح: كل نقط التقاطع بين كل زوج خطوط في المجموعة
// All intersection points between every pair of lines
// ---- All intersections ----
vector<pt> allLineIntersections(const vector<Line>& lines) {
    vector<pt> ans;
    int n = lines.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            pt p;
            if (intersection(lines[i], lines[j], p))
                ans.push_back(p);
        }
    }
    return ans;
}

// ============================================================================
// CHAPTER 17 — CONVEX POLYGON UTILITIES (ROTATING CALIPERS)
// شرح: تقنية الفرجار الدوار (Rotating Calipers) على المضلعات المحدبة
// ============================================================================

// شرح: كل الأزواج المتقابلة (Antipodal Pairs) في مضلع محدب، مفيدة لأقطار وأبعاد
// Anti-podal pairs of a convex polygon
// ---- Antipodal pairs ----
vector<pair<int, int>> allAntiPodalPairs(int n, vector<pt>& p) {
    vector<pair<int, int>> result;
    vector<bool> vis(n, false);
    
    auto nx = [&](int i) { return (i + 1) % n; };
    auto pv = [&](int i) { return (i - 1 + n) % n; };
    
    for (int p1 = 0, p2 = 0; p1 < n; ++p1) {
        pt base = p[nx(p1)] - p[p1];
        
        while (p2 == p1 || p2 == nx(p1) ||
               sgn(cross(base, p[nx(p2)] - p[p2])) == sgn(cross(base, p[p2] - p[pv(p2)]))) {
            p2 = nx(p2);
        }
        
        if (vis[p1]) continue;
        vis[p1] = true;
        
        result.push_back({p1, p2});
        result.push_back({nx(p1), p2});
        
        if (sgn(cross(base, p[nx(p2)] - p[p2])) == 0) {
            result.push_back({p1, nx(p2)});
            result.push_back({nx(p1), nx(p2)});
            vis[p2] = true;
        }
    }
    
    return result;
}

// شرح: أكبر مسافة بين نقطتين من مضلعين محدبين مختلفين (Rotating Calipers)
// Maximum distance between two convex polygons (rotating calipers)
// ---- Maximum distance ----
ld maximumDistancePolygons(vector<pt>& u, vector<pt>& v) {
    int n = (int)u.size(), m = (int)v.size();
    ld ans = 0;
    
    if (n < 3 || m < 3) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                ans = max(ans, (ld)sq(u[i] - v[j]));
        return sqrtl((ld)ans);
    }
    
    if (u[0].X > v[0].X) {
        swap(n, m);
        swap(u, v);
    }
    
    int i = 0, j = 0, step = n + m + 10;
    while (j + 1 < m && v[j].X < v[j + 1].X) j++;
    
    while (step--) {
        if (cross(u[(i + 1) % n] - u[i], v[(j + 1) % m] - v[j]) >= 0)
            j = (j + 1) % m;
        else
            i = (i + 1) % n;
        ans = max(ans, (ld)sq(u[i] - v[j]));
    }
    
    return sqrtl((ld)ans);
}


// ============================================================================
// CHAPTER 18 — INSCRIBED SHAPES
// شرح: أكبر شكل (دائرة/مضلع/مثلث/مستطيل) ممكن يتحط جوه شكل تاني
// (ملحوظة: Circle struct متعرفة قبل كده في الفصل 11، مش هنكررها هنا)
// ============================================================================

// شرح: أكبر دائرة ممكن تتحط جوه مضلع محدب (مركز تشيبيشيف)، عن طريق Binary
// Search على نصف القطر + تقاطع أنصاف مستويات
// LARGEST CIRCLE INSIDE A CONVEX POLYGON (Chebyshev Center)
// For a convex polygon, find largest inscribed circle
// Uses binary search on radius + half-plane intersection
// ---- Largest circle in convex polygon ----
Circle largestCircleInConvexPolygon(vector<pt>& polygon, ld precision = 1e-7) {
    int n = polygon.size();
    Circle result;
    result.radius = 0;
    result.center = polygon[0];
    
    // Build edges as lines
    vector<Line> edges;
    for (int i = 0; i < n; i++) {
        edges.push_back(Line(polygon[i], polygon[(i+1)%n]));
    }
    
    // Binary search on radius
    ld lo = 0, hi = 1e9;
    
    // Find upper bound
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            hi = min(hi, abs(polygon[i] - polygon[j]) / 2.0L);
        }
    }
    
    while (hi - lo > precision) {
        ld mid = (lo + hi) / 2.0L;
        
        // Shift all edges inward by 'mid'
        vector<Halfplane> halfplanes;
        for (int i = 0; i < n; i++) {
            pt p1 = polygon[i];
            pt p2 = polygon[(i+1)%n];
            pt dir = normalize(p2 - p1);
            pt inward = rotate90CCW(dir); // assuming CCW polygon
            
            // Shift edge inward
            pt newP1 = p1 + inward * mid;
            pt newP2 = p2 + inward * mid;
            halfplanes.push_back(Halfplane(newP1, newP2));
        }
        
        vector<pt> intersection = halfPlaneIntersection(halfplanes);
        
        if (!intersection.empty()) {
            // If intersection exists, circle of radius 'mid' fits
            lo = mid;
            // Find center: center of mass of intersection polygon
            pt center(0,0);
            for (auto& p : intersection) {
                center += p;
            }
            center /= intersection.size();
            result.center = center;
            result.radius = mid;
        } else {
            hi = mid;
        }
    }
    
    return result;
}

// شرح: أكبر دائرة جوه مضلع عادي (مش شرط محدب)، بأسلوب عينات نقط تقريبي
// LARGEST CIRCLE IN ANY POLYGON (Simple Polygon)
// For any simple polygon, find largest inscribed circle
// Uses medial axis / Voronoi approach (simplified using point sampling)
// ---- Largest circle in any polygon ----
Circle largestCircleInPolygon(vector<pt>& polygon, int samples = 1000) {
    int n = polygon.size();
    Circle result;
    result.radius = 0;
    
    // Find bounding box
    ld minX = polygon[0].X, maxX = polygon[0].X;
    ld minY = polygon[0].Y, maxY = polygon[0].Y;
    for (auto& p : polygon) {
        minX = min(minX, p.X);
        maxX = max(maxX, p.X);
        minY = min(minY, p.Y);
        maxY = max(maxY, p.Y);
    }
    
    // Sample points inside polygon
    ld width = maxX - minX;
    ld height = maxY - minY;
    ld area = polygonArea(polygon);
    
    int gridSize = sqrt(samples * area / (width * height)) + 1;
    gridSize = max(10, gridSize);
    
    for (int i = 0; i <= gridSize; i++) {
        for (int j = 0; j <= gridSize; j++) {
            pt p(minX + width * i / gridSize, minY + height * j / gridSize);
            
            if (!pointInPolygon(polygon, p, false)) continue;
            
            // Find distance to nearest edge
            ld minDist = distancePointToPolygon(polygon, p);
            
            if (minDist > result.radius) {
                result.radius = minDist;
                result.center = p;
            }
        }
    }
    
    // Refine around best point
    if (result.radius > 0) {
        for (int iter = 0; iter < 3; iter++) {
            ld step = max(width, height) / (gridSize * 2);
            for (int dx = -2; dx <= 2; dx++) {
                for (int dy = -2; dy <= 2; dy++) {
                    pt p(result.center.X + dx * step, result.center.Y + dy * step);
                    
                    if (!pointInPolygon(polygon, p, false)) continue;
                    
                    ld minDist = distancePointToPolygon(polygon, p);
                    
                    if (minDist > result.radius) {
                        result.radius = minDist;
                        result.center = p;
                    }
                }
            }
            gridSize *= 2;
        }
    }
    
    return result;
}

// شرح: أكبر مضلع منتظم بعدد أضلاع n جوه دائرة (بيرجع رؤوس المضلع)
// LARGEST POLYGON (n sides) INSIDE A CIRCLE
// Largest n-gon inscribed in a circle of given radius
// Returns vertices of the n-gon
// ---- Largest n-gon in circle ----
vector<pt> largestPolygonInCircle(pt center, ld radius, int n) {
    vector<pt> result;
    
    if (n < 3) return result;
    
    // Regular n-gon inscribed in circle
    // Vertices at angle 0, 2π/n, 4π/n, ...
    for (int i = 0; i < n; i++) {
        ld angle = 2 * PI * i / n;
        pt vertex = center + pt(radius * cosl(angle), radius * sinl(angle));
        result.push_back(vertex);
    }
    
    return result;
}

// شرح: نفس الفكرة لكن الدائرة معرَّفة بثلاث نقط عليها بدل مركز ونصف قطر
// Largest polygon with n sides inside a circle defined by 3 points
vector<pt> largestPolygonInCircle(pt a, pt b, pt c, int n) {
    // Find circumcircle
    Circle cir;
    if (!circumcirclePolygon({a, b, c}, cir)) {
        return vector<pt>();
    }
    
    return largestPolygonInCircle(cir.center, cir.radius, n);
}

// شرح: نفس الفكرة لكن الدائرة ممررة كـ Circle كامل بدل مركز ونصف قطر منفصلين
// Largest polygon with n sides inside a circle defined by center and radius
vector<pt> largestPolygonInCircle(Circle cir, int n) {
    return largestPolygonInCircle(cir.center, cir.radius, n);
}

// شرح: أكبر مضلع بعدد أضلاع n جوه مضلع محدب (تقريبي عن طريق تحسين تكراري)
// LARGEST POLYGON (n sides) INSIDE A CONVEX POLYGON
// Find largest n-gon inside a convex polygon using rotating calipers
// Approximation using angle bisectors and binary search
// ---- Largest n-gon in convex polygon ----
vector<pt> largestNGonInConvexPolygon(vector<pt>& polygon, int n, ld precision = 1e-6) {
    int m = polygon.size();
    vector<pt> result;
    
    if (n < 3 || m < 3) return result;
    
    // For triangle inside polygon, use Welzl's algorithm
    if (n == 3) {
        // Find largest triangle in convex polygon
        ld maxArea = 0;
        vector<pt> bestTriangle;
        
        for (int i = 0; i < m; i++) {
            int k = (i + 2) % m;
            for (int j = i + 1; j < m; j++) {
                while (k != i && polygonArea({polygon[i], polygon[j], polygon[(k+1)%m]}) > 
                       polygonArea({polygon[i], polygon[j], polygon[k]})) {
                    k = (k + 1) % m;
                }
                ld area = polygonArea({polygon[i], polygon[j], polygon[k]});
                if (area > maxArea) {
                    maxArea = area;
                    bestTriangle = {polygon[i], polygon[j], polygon[k]};
                }
            }
        }
        return bestTriangle;
    }
    
    // For n > 3, use iterative optimization
    // Start with regular n-gon approximation inside polygon
    Circle inner = largestCircleInConvexPolygon(polygon);
    
    // Find largest n-gon in circle, then project to polygon boundary
    vector<pt> initial = largestPolygonInCircle(inner.center, inner.radius, n);
    
    // Move vertices outward to polygon boundary
    for (auto& p : initial) {
        // Find closest point on polygon boundary
        ld minDist = numeric_limits<ld>::max();
        pt bestPoint = p;
        
        for (int i = 0; i < m; i++) {
            pt a = polygon[i];
            pt b = polygon[(i+1)%m];
            
            // Project p onto edge
            Line edge(a, b);
            pt proj = edge.projection(p);
            
            if (onSegment(a, b, proj)) {
                ld dist = abs(p - proj);
                if (dist < minDist) {
                    minDist = dist;
                    bestPoint = proj;
                }
            }
        }
        p = bestPoint;
    }
    
    // Optimize vertices using gradient descent
    for (int iter = 0; iter < 50; iter++) {
        for (int i = 0; i < n; i++) {
            // Move vertex to maximize area
            int prev = (i - 1 + n) % n;
            int next = (i + 1) % n;
            
            pt dir = (result[next] - result[prev]);
            dir = normalize(rotate90CCW(dir));
            
            ld step = 0.001L * inner.radius;
            pt newP = result[i] + dir * step;
            
            if (pointInPolygon(polygon, newP, false)) {
                // Check if area increases
                vector<pt> testPoly = result;
                testPoly[i] = newP;
                if (polygonArea(testPoly) > polygonArea(result)) {
                    result[i] = newP;
                }
            }
        }
    }
    
    return result;
}

// شرح: أكبر مثلث جوه مضلع محدب، بأسلوب Rotating Calipers في O(n^2)
// LARGEST TRIANGLE INSIDE A CONVEX POLYGON (Optimized)
// Uses rotating calipers for O(n^2) complexity
// ---- Largest triangle ----
vector<pt> largestTriangleInConvexPolygon(vector<pt>& polygon) {
    int n = polygon.size();
    vector<pt> result;
    ld maxArea = 0;
    
    for (int i = 0; i < n; i++) {
        int k = (i + 2) % n;
        for (int j = i + 1; j < n; j++) {
            while (k != i && polygonArea({polygon[i], polygon[j], polygon[(k+1)%n]}) > 
                   polygonArea({polygon[i], polygon[j], polygon[k]})) {
                k = (k + 1) % n;
            }
            ld area = polygonArea({polygon[i], polygon[j], polygon[k]});
            if (area > maxArea) {
                maxArea = area;
                result = {polygon[i], polygon[j], polygon[k]};
            }
        }
    }
    
    return result;
}

// شرح: أكبر مستطيل محاذي للمحاور جوه مضلع محدب (تقريب حاليًا عن طريق الدائرة الداخلية)
// LARGEST RECTANGLE INSIDE A CONVEX POLYGON
// Find largest axis-aligned rectangle in convex polygon
// ---- Largest rectangle ----
vector<pt> largestRectangleInConvexPolygon(vector<pt>& polygon) {
    int n = polygon.size();
    vector<pt> result;
    ld maxArea = 0;
    
    // For each pair of edges, find rectangle
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            // Use edge i and j as opposite sides of rectangle
            pt dir1 = normalize(polygon[(i+1)%n] - polygon[i]);
            pt dir2 = rotate90CCW(dir1);
            
            // Project all points onto dir1 and dir2
            ld min1 = numeric_limits<ld>::max(), max1 = -numeric_limits<ld>::max();
            ld min2 = numeric_limits<ld>::max(), max2 = -numeric_limits<ld>::max();
            
            for (auto& p : polygon) {
                ld proj1 = dotProduct(p, dir1);
                ld proj2 = dotProduct(p, dir2);
                min1 = min(min1, proj1);
                max1 = max(max1, proj1);
                min2 = min(min2, proj2);
                max2 = max(max2, proj2);
            }
            
            // Find rectangle using rotating calipers
            // Simplified: use extents
            vector<pt> rect = {
                pt(min1, min2),
                pt(max1, min2),
                pt(max1, max2),
                pt(min1, max2)
            };
            
            // Transform back to original coordinates
            // Actually, we need to find intersection of polygon with this rectangle
            // This is a simplified version
        }
    }
    
    // For now, return a simple approximation
    Circle inner = largestCircleInConvexPolygon(polygon);
    ld side = inner.radius / sqrtl(2.0L);
    result = {
        inner.center + pt(-side, -side),
        inner.center + pt(side, -side),
        inner.center + pt(side, side),
        inner.center + pt(-side, side)
    };
    
    return result;
}

// شرح: أكبر مثلث متساوي الأضلاع جوه دائرة (بيتحول لأكبر مضلع 3 أضلاع فيها)
// LARGEST EQUILATERAL TRIANGLE IN CIRCLE
vector<pt> largestEquilateralTriangleInCircle(pt center, ld radius) {
    return largestPolygonInCircle(center, radius, 3);
}

vector<pt> largestEquilateralTriangleInCircle(Circle cir) {
    return largestPolygonInCircle(cir, 3);
}

// شرح: أكبر مثلث جوه مضلع عادي (مش شرط محدب) بأسلوب عينات نقط
// LARGEST TRIANGLE IN ANY POLYGON (not necessarily convex)
// Using point sampling for non-convex polygons
vector<pt> largestTriangleInPolygon(vector<pt>& polygon, int samples = 1000) {
    int n = polygon.size();
    vector<pt> result;
    ld maxArea = 0;
    
    // Find bounding box
    ld minX = polygon[0].X, maxX = polygon[0].X;
    ld minY = polygon[0].Y, maxY = polygon[0].Y;
    for (auto& p : polygon) {
        minX = min(minX, p.X);
        maxX = max(maxX, p.X);
        minY = min(minY, p.Y);
        maxY = max(maxY, p.Y);
    }
    
    // Sample points inside polygon
    vector<pt> insidePoints;
    int gridSize = sqrt(samples) + 1;
    
    for (int i = 0; i <= gridSize; i++) {
        for (int j = 0; j <= gridSize; j++) {
            pt p(minX + (maxX - minX) * i / gridSize,
                 minY + (maxY - minY) * j / gridSize);
            if (pointInPolygon(polygon, p, false)) {
                insidePoints.push_back(p);
            }
        }
    }
    
    // Find largest triangle among sampled points
    for (int i = 0; i < (int)insidePoints.size(); i++) {
        for (int j = i+1; j < (int)insidePoints.size(); j++) {
            for (int k = j+1; k < (int)insidePoints.size(); k++) {
                ld area = triangleArea(insidePoints[i], insidePoints[j], insidePoints[k]);
                if (area > maxArea) {
                    maxArea = area;
                    result = {insidePoints[i], insidePoints[j], insidePoints[k]};
                }
            }
        }
    }
    
    return result;
}


// ============================================================================
// CHAPTER 19 — QUICK FORMULA REFERENCE (no vectors needed)
// شرح: صيغ هندسية جاهزة تشتغل على أرقام عادية (مش محتاجة إحداثيات pt)
// مفيدة لما المسألة بتديك أطوال/زوايا مباشرة بدل نقط
// ============================================================================

// ---- Basic trigonometry | حساب المثلثات الأساسي ----

// شرح: جيب الزاوية = المقابل / الوتر
ld sine(ld opposite, ld hypotenuse) {
    return opposite / hypotenuse;
}

// شرح: جيب تمام الزاوية = المجاور / الوتر
ld cosine(ld adjacent, ld hypotenuse) {
    return adjacent / hypotenuse;
}

// شرح: ظل الزاوية = المقابل / المجاور
ld tangent(ld opposite, ld adjacent) {
    return opposite / adjacent;
}

// ---- Law of Sines | قانون الجيب ----

// شرح: نسبة الضلع / جيب الزاوية المقابلة له (ثابتة في أي مثلث)
ld lawOfSines(ld side, ld angle) {
    return side / sinl(angle);
}

// شرح: إيجاد ضلع مجهول باستخدام قانون الجيب
ld sideFromSine(ld knownSide, ld knownAngle, ld targetAngle) {
    return knownSide * sinl(targetAngle) / sinl(knownAngle);
}

// شرح: إيجاد زاوية مجهولة باستخدام قانون الجيب
ld angleFromSine(ld knownSide, ld knownAngle, ld targetSide) {
    return asinl(targetSide * sinl(knownAngle) / knownSide);
}

// ---- Law of Cosines | قانون جيب التمام ----

// شرح: جيب تمام الزاوية C من أطوال الأضلاع الثلاثة
ld lawOfCosines(ld a, ld b, ld c) {
    return (a * a + b * b - c * c) / (2.0L * a * b);
}

// شرح: إيجاد الضلع الثالث من ضلعين والزاوية بينهم
ld sideFromCosines(ld a, ld b, ld angleC) {
    return sqrtl(a * a + b * b - 2.0L * a * b * cosl(angleC));
}

// شرح: إيجاد زاوية من أطوال الأضلاع الثلاثة
ld angleFromCosines(ld a, ld b, ld c) {
    return acosl((a * a + b * b - c * c) / (2.0L * a * b));
}

// ---- Pythagorean theorem | نظرية فيثاغورس ----

// شرح: طول الوتر من الضلعين القائمين
ld hypotenuse(ld a, ld b) {
    return sqrtl(a * a + b * b);
}

// شرح: طول ضلع قائم من الوتر والضلع التاني
ld pythagoreanLeg(ld hyp, ld leg) {
    return sqrtl(hyp * hyp - leg * leg);
}

// ---- Distance & midpoint | المسافة ومنتصف القطعة ----

// شرح: المسافة بين نقطتين في بعدين (2D)
ld distance2D(ld x1, ld y1, ld x2, ld y2) {
    return hypotl(x2 - x1, y2 - y1);
}

// شرح: المسافة بين نقطتين في ثلاثة أبعاد (3D)
ld distance3D(ld x1, ld y1, ld z1, ld x2, ld y2, ld z2) {
    return sqrtl((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

// شرح: منتصف القطعة بين نقطتين في بعدين
pair<ld, ld> midpoint2D(ld x1, ld y1, ld x2, ld y2) {
    return {(x1 + x2) / 2.0L, (y1 + y2) / 2.0L};
}

// شرح: منتصف القطعة بين نقطتين في ثلاثة أبعاد
tuple<ld, ld, ld> midpoint3D(ld x1, ld y1, ld z1, ld x2, ld y2, ld z2) {
    return {(x1 + x2) / 2.0L, (y1 + y2) / 2.0L, (z1 + z2) / 2.0L};
}

// ---- Heron's formula | صيغة هيرون ----

// شرح: مساحة المثلث من أطوال أضلاعه الثلاثة (صيغة هيرون)
ld heronArea(ld a, ld b, ld c) {
    ld s = (a + b + c) / 2.0L;
    return sqrtl(s * (s - a) * (s - b) * (s - c));
}

// ---- Circle formulas | صيغ الدائرة ----

// شرح: قطر الدائرة من نصف القطر
ld circleDiameter(ld r) {
    return 2.0L * r;
}

// شرح: نصف قطر الدائرة من القطر
ld circleRadius(ld d) {
    return d / 2.0L;
}

// شرح: محيط الدائرة
ld circleCircumference(ld r) {
    return 2.0L * PI * r;
}

// شرح: مساحة الدائرة
ld circleArea(ld r) {
    return PI * r * r;
}

// ---- Sector formulas | صيغ القطاع الدائري ----

// شرح: طول القوس بالراديان
ld arcLength(ld r, ld theta) {
    return r * theta;
}

// شرح: طول القوس بالدرجات
ld arcLengthDegrees(ld r, ld thetaDeg) {
    return thetaDeg / 360.0L * 2.0L * PI * r;
}

// شرح: مساحة القطاع الدائري بالراديان
ld sectorArea(ld r, ld theta) {
    return 0.5L * r * r * theta;
}

// شرح: مساحة القطاع الدائري بالدرجات
ld sectorAreaDegrees(ld r, ld thetaDeg) {
    return thetaDeg / 360.0L * PI * r * r;
}

// مساحة القطاع = ½ × طول القوس × نصف القطر  (نفس ½ θ ر²)
// Sector area from arc length: (1/2) * L * r
ld sectorAreaFromArc(ld r, ld arcLen) {
    return 0.5L * r * arcLen;
}

// محيط القطاع الدائري = ٢ نق + طول القوس
// Sector perimeter = 2r + arc length
ld sectorPerimeter(ld r, ld theta) {
    return 2.0L * r + arcLength(r, theta);
}
ld sectorPerimeterDegrees(ld r, ld thetaDeg) {
    return 2.0L * r + arcLengthDegrees(r, thetaDeg);
}

// ---- Segment formulas | صيغ القطعة الدائرية (Circular Segment) ----

// شرح: مساحة القطعة الدائرية (المنطقة بين الوتر والقوس)
ld segmentArea(ld r, ld theta) {
    return 0.5L * r * r * (theta - sinl(theta));
}

// مساحة القطعة الدائرية بالدرجات
ld segmentAreaDegrees(ld r, ld thetaDeg) {
    return segmentArea(r, thetaDeg * PI / 180.0L);
}

// شرح: طول الوتر (Chord) لزاوية معينة في الدائرة
ld chordLength(ld r, ld theta) {
    return 2.0L * r * sinl(theta / 2.0L);
}

// محيط القطعة الدائرية = طول القوس + طول الوتر
// Segment perimeter = arc + chord
ld segmentPerimeter(ld r, ld theta) {
    return arcLength(r, theta) + chordLength(r, theta);
}

// ---- Triangle formulas | صيغ المثلث ----

// شرح: محيط المثلث
ld trianglePerimeter(ld a, ld b, ld c) {
    return a + b + c;
}

// شرح: مساحة المثلث = نصف القاعدة × الارتفاع
ld triangleAreaBH(ld base, ld height) {
    return 0.5L * base * height;
}

// شرح: مساحة المثلث من ضلعين والزاوية بينهم (SAS)
ld triangleAreaSAS(ld a, ld b, ld angleC) {
    return 0.5L * a * b * sinl(angleC);
}

// شرح: مساحة المثلث من الأضلاع الثلاثة (صيغة هيرون)
ld triangleAreaHeron(ld a, ld b, ld c) {
    ld s = (a + b + c) / 2.0L;
    return sqrtl(s * (s - a) * (s - b) * (s - c));
}

// شرح: مساحة المثلث من ضلعين وزاوية بالدرجات (مش راديان)
ld triangleAreaDegrees(ld a, ld b, ld angleDegrees) {
    return 0.5L * a * b * sinl(angleDegrees * PI / 180.0L);
}

// شرح: نصف قطر الدائرة المحصورة داخل المثلث (Inradius)
ld triangleInradius(ld a, ld b, ld c) {
    ld s = (a + b + c) / 2.0L;
    return triangleAreaHeron(a, b, c) / s;
}

// شرح: نصف قطر الدائرة المحيطة بالمثلث (Circumradius)
ld triangleCircumradius(ld a, ld b, ld c) {
    return (a * b * c) / (4.0L * triangleAreaHeron(a, b, c));
}

// طول المتوسط من الرأس إلى منتصف الضلع المقابل a
// Median length to side a (from the opposite vertex)
ld triangleMedian(ld a, ld b, ld c) {
    // m_a = 0.5 * sqrt(2b² + 2c² - a²)
    return 0.5L * sqrtl(2.0L * b * b + 2.0L * c * c - a * a);
}

// ---- Equilateral triangle | المثلث متساوي الأضلاع ----

// شرح: محيط المثلث متساوي الأضلاع
ld equilateralPerimeter(ld side) {
    return 3.0L * side;
}

// شرح: مساحة المثلث متساوي الأضلاع
ld equilateralArea(ld side) {
    return sqrtl(3.0L) / 4.0L * side * side;
}

// شرح: ارتفاع المثلث متساوي الأضلاع
ld equilateralHeight(ld side) {
    return sqrtl(3.0L) / 2.0L * side;
}

// شرح: نصف قطر الدائرة الداخلية للمثلث متساوي الأضلاع
ld equilateralInradius(ld side) {
    return side * sqrtl(3.0L) / 6.0L;
}

// شرح: نصف قطر الدائرة المحيطة بالمثلث متساوي الأضلاع
ld equilateralCircumradius(ld side) {
    return side / sqrtl(3.0L);
}

// ---- Rectangle | المستطيل ----

// شرح: محيط المستطيل
ld rectanglePerimeter(ld length, ld width) {
    return 2.0L * (length + width);
}

// شرح: مساحة المستطيل
ld rectangleArea(ld length, ld width) {
    return length * width;
}

// شرح: طول قطر المستطيل
ld rectangleDiagonal(ld length, ld width) {
    return sqrtl(length * length + width * width);
}

// ---- Square | المربع ----

// شرح: محيط المربع
ld squarePerimeter(ld side) {
    return 4.0L * side;
}

// شرح: مساحة المربع
ld squareArea(ld side) {
    return side * side;
}

// شرح: قطر المربع
ld squareDiagonal(ld side) {
    return side * sqrtl(2.0L);
}

// شرح: مساحة المربع من طول قطره
ld squareAreaFromDiagonal(ld diagonal) {
    return diagonal * diagonal / 2.0L;
}

// ---- Rhombus | المعين ----

// شرح: محيط المعين
ld rhombusPerimeter(ld side) {
    return 4.0L * side;
}

// شرح: مساحة المعين من قطريه
ld rhombusAreaDiagonals(ld d1, ld d2) {
    return d1 * d2 / 2.0L;
}

// شرح: مساحة المعين من القاعدة والارتفاع
ld rhombusArea(ld base, ld height) {
    return base * height;
}

// ---- Parallelogram | متوازي الأضلاع ----

// شرح: محيط متوازي الأضلاع
ld parallelogramPerimeter(ld a, ld b) {
    return 2.0L * (a + b);
}

// شرح: مساحة متوازي الأضلاع
ld parallelogramArea(ld base, ld height) {
    return base * height;
}

// مساحة متوازي الأضلاع = ضلع × ضلع × جا الزاوية المحصورة
// Area = a * b * sin(included angle)
ld parallelogramAreaSAS(ld a, ld b, ld angle) {
    return a * b * sinl(angle);
}

// ---- Trapezoid | شبه المنحرف ----

// شرح: محيط شبه المنحرف (مجموع الأضلاع الأربعة)
ld trapezoidPerimeter(ld a, ld b, ld c, ld d) {
    return a + b + c + d;
}

// شرح: مساحة شبه المنحرف من القاعدتين والارتفاع
ld trapezoidArea(ld a, ld b, ld height) {
    return (a + b) * height / 2.0L;
}

// شرح: الخط الأوسط (Median) لشبه المنحرف
ld trapezoidMedian(ld a, ld b) {
    return (a + b) / 2.0L;
}

// شرح: مساحة شبه المنحرف من الخط الأوسط والارتفاع
ld trapezoidAreaFromMedian(ld median, ld height) {
    return median * height;
}

// ---- Kite | الطائرة الورقية (شكل الطائرة) ----

// شرح: محيط شكل الطائرة الورقية
ld kitePerimeter(ld a, ld b) {
    return 2.0L * (a + b);
}

// شرح: مساحة شكل الطائرة الورقية من قطريها
ld kiteArea(ld d1, ld d2) {
    return d1 * d2 / 2.0L;
}

// ---- Regular polygon | المضلع المنتظم ----

// شرح: محيط المضلع المنتظم (n ضلع، طول كل ضلع side)
ld regularPolygonPerimeter(int n, ld side) {
    return n * side;
}

// شرح: مساحة المضلع المنتظم من عدد الأضلاع وطول الضلع
ld regularPolygonArea(int n, ld side) {
    return (n * side * side) / (4.0L * tanl(PI / n));
}

// شرح: مساحة المضلع المنتظم من نصف قطر الدائرة المحيطة (Circumradius)
ld regularPolygonAreaFromCircumradius(int n, ld R) {
    return n * R * R * sinl(2.0L * PI / n) / 2.0L;
}

// شرح: مساحة المضلع المنتظم من نصف قطر الدائرة الداخلية (Inradius)
ld regularPolygonAreaFromInradius(int n, ld r) {
    return n * r * r * tanl(PI / n);
}

// شرح: مساحة المضلع المنتظم من المحيط والسهم (Apothem)
ld regularPolygonAreaWithApothem(int n, ld side, ld apothem) {
    return regularPolygonPerimeter(n, side) * apothem / 2.0L;
}

// شرح: نصف قطر الدائرة المحيطة بالمضلع المنتظم
ld regularPolygonCircumradius(int n, ld side) {
    return side / (2.0L * sinl(PI / n));
}

// شرح: نصف قطر الدائرة الداخلية للمضلع المنتظم (السهم Apothem)
ld regularPolygonInradius(int n, ld side) {
    return side / (2.0L * tanl(PI / n));
}

// ---- Ellipse | القطع الناقص ----

// شرح: مساحة القطع الناقص من نصفي محوريه
ld ellipseArea(ld a, ld b) {
    return PI * a * b;
}

// ---- 3D shapes | الأشكال المجسمة (ثلاثية الأبعاد) ----

// شرح: حجم المكعب
ld cubeVolume(ld side) {
    return side * side * side;
}

// مساحة وجه المكعب
// Area of one face of a cube
ld cubeFaceArea(ld side) {
    return side * side;
}

// شرح: المساحة السطحية الكلية للمكعب
ld cubeSurfaceArea(ld side) {
    return 6.0L * side * side;
}

// شرح: المساحة الجانبية للمكعب (بدون القاعدتين)
ld cubeLateralArea(ld side) {
    return 4.0L * side * side;
}

// شرح: قطر المكعب (من ركن لركن مقابل له)
ld cubeDiagonal(ld side) {
    return side * sqrtl(3.0L);
}

// شرح: حجم متوازي المستطيلات (الصندوق)
ld cuboidVolume(ld l, ld w, ld h) {
    return l * w * h;
}

// شرح: المساحة السطحية الكلية لمتوازي المستطيلات
ld cuboidSurfaceArea(ld l, ld w, ld h) {
    return 2.0L * (l * w + l * h + w * h);
}

// شرح: المساحة الجانبية لمتوازي المستطيلات
ld cuboidLateralArea(ld l, ld w, ld h) {
    return 2.0L * h * (l + w);
}

// شرح: قطر متوازي المستطيلات
ld cuboidDiagonal(ld l, ld w, ld h) {
    return sqrtl(l * l + w * w + h * h);
}

// شرح: حجم الأسطوانة
ld cylinderVolume(ld r, ld h) {
    return PI * r * r * h;
}

// مساحة قاعدة الأسطوانة
ld cylinderBaseArea(ld r) {
    return PI * r * r;
}

// شرح: المساحة الجانبية للأسطوانة
ld cylinderLateralArea(ld r, ld h) {
    return 2.0L * PI * r * h;
}

// شرح: المساحة السطحية الكلية للأسطوانة (شامل القاعدتين)
ld cylinderSurfaceArea(ld r, ld h) {
    return 2.0L * PI * r * (r + h);
}

// شرح: حجم المخروط
ld coneVolume(ld r, ld h) {
    return PI * r * r * h / 3.0L;
}

// شرح: الميل الجانبي (Slant Height) للمخروط
ld coneSlantHeight(ld r, ld h) {
    return sqrtl(r * r + h * h);
}

// شرح: المساحة الجانبية للمخروط
ld coneLateralArea(ld r, ld slantHeight) {
    return PI * r * slantHeight;
}

// شرح: المساحة السطحية الكلية للمخروط
ld coneSurfaceArea(ld r, ld slantHeight) {
    return PI * r * (r + slantHeight);
}

// المساحة الجانبية للمخروط من نق والارتفاع
ld coneLateralAreaRH(ld r, ld h) {
    return coneLateralArea(r, coneSlantHeight(r, h));
}

// المساحة الكلية للمخروط من نق والارتفاع
ld coneSurfaceAreaRH(ld r, ld h) {
    return coneSurfaceArea(r, coneSlantHeight(r, h));
}

// شرح: حجم الكرة
ld sphereVolume(ld r) {
    return (4.0L / 3.0L) * PI * r * r * r;
}

// شرح: المساحة السطحية للكرة
ld sphereSurfaceArea(ld r) {
    return 4.0L * PI * r * r;
}

// شرح: حجم نصف الكرة
ld hemisphereVolume(ld r) {
    return (2.0L / 3.0L) * PI * r * r * r;
}

// شرح: مساحة الجزء المنحني (القبة) لنصف الكرة
ld hemisphereCurvedArea(ld r) {
    return 2.0L * PI * r * r;
}

// شرح: المساحة السطحية الكلية لنصف الكرة (القبة + القاعدة)
ld hemisphereTotalArea(ld r) {
    return 3.0L * PI * r * r;
}

// شرح: حجم المنشور من مساحة القاعدة والارتفاع
ld prismVolume(ld baseArea, ld height) {
    return baseArea * height;
}

// شرح: المساحة الجانبية للمنشور
ld prismLateralArea(ld basePerimeter, ld height) {
    return basePerimeter * height;
}

// شرح: المساحة السطحية الكلية للمنشور
ld prismSurfaceArea(ld baseArea, ld basePerimeter, ld height) {
    return 2.0L * baseArea + basePerimeter * height;
}

// شرح: حجم الهرم من مساحة القاعدة والارتفاع
ld pyramidVolume(ld baseArea, ld height) {
    return baseArea * height / 3.0L;
}

// شرح: المساحة الجانبية للهرم
ld pyramidLateralArea(ld basePerimeter, ld slantHeight) {
    return 0.5L * basePerimeter * slantHeight;
}

// شرح: المساحة السطحية الكلية للهرم
ld pyramidSurfaceArea(ld baseArea, ld basePerimeter, ld slantHeight) {
    return pyramidLateralArea(basePerimeter, slantHeight) + baseArea;
}

// شرح: ارتفاع الهرم المنتظم من طول الحرف الجانبي
ld regularPyramidHeight(int n, ld edge) {
    ld R = regularPolygonCircumradius(n, edge);
    return sqrtl(edge * edge - R * R);
}

// شرح: حجم الهرم المنتظم من طول الحرف الجانبي
ld regularPyramidVolume(int n, ld edge) {
    return regularPolygonArea(n, edge) * regularPyramidHeight(n, edge) / 3.0L;
}

// ---- Regular star | النجمة المنتظمة ----

// شرح: مساحة نجمة منتظمة بعدد رؤوس n ونصف قطر خارجي r
ld regularStarArea(int n, ld r) {
    ld theta = PI / n;
    return n * r * r * sinl(2.0L * theta) * sinl(theta)
           / (2.0L * (sinl(3.0L * theta) + sinl(theta)));
}

// ---- Quadratic equation | المعادلة التربيعية ax² + bx + c = 0 ----

// المميز: Δ = b² - 4ac
// Discriminant
ld quadraticDiscriminant(ld a, ld b, ld c) {
    return b * b - 4.0L * a * c;
}

// حل المعادلة التربيعية ax² + bx + c = 0
// يرجع عدد الجذور الحقيقية (0 / 1 / 2) ويكتبها في out
// Solve ax² + bx + c = 0; returns number of real roots (0/1/2)
int solveQuadratic(ld a, ld b, ld c, pair<ld, ld>& out) {
    // Linear case: bx + c = 0
    if (fabsl(a) < EPS) {
        if (fabsl(b) < EPS) return 0; // no equation or infinite solutions
        out = {-c / b, -c / b};
        return 1;
    }
    ld disc = quadraticDiscriminant(a, b, c);
    if (disc < -EPS) return 0; // no real roots
    if (fabsl(disc) <= EPS) {
        ld x = -b / (2.0L * a);
        out = {x, x};
        return 1;
    }
    ld s = sqrtl(disc);
    // Numerically stable form
    ld q = -0.5L * (b + (b >= 0 ? s : -s));
    ld x1 = q / a;
    ld x2 = c / q;
    if (x1 > x2) swap(x1, x2);
    out = {x1, x2};
    return 2;
}

// مجموع الجذور = -b/a  |  حاصل ضرب الجذور = c/a  (فييت)
// Sum of roots = -b/a
ld quadraticSumOfRoots(ld a, ld b, ld c) {
    return -b / a;
}
// Product of roots = c/a
ld quadraticProductOfRoots(ld a, ld b, ld c) {
    return c / a;
}

// ---- Algebraic square formulas | المتطابقات الجبرية (مربع المجموع والفرق) ----

// مربع المجموع: (a + b)² = a² + 2ab + b²
// Square of a sum
ld squareOfSum(ld a, ld b) {
    return a * a + 2.0L * a * b + b * b;
}

// مربع الفرق: (a - b)² = a² - 2ab + b²
// Square of a difference
ld squareOfDifference(ld a, ld b) {
    return a * a - 2.0L * a * b + b * b;
}

// فرق المربعين: a² - b² = (a - b)(a + b)
// Difference of squares
ld differenceOfSquares(ld a, ld b) {
    return (a - b) * (a + b);
}

// مربع المجموع لثلاثة حدود: (a + b + c)² = a² + b² + c² + 2ab + 2ac + 2bc
// Square of sum of three terms
ld squareOfSum3(ld a, ld b, ld c) {
    return a * a + b * b + c * c + 2.0L * (a * b + a * c + b * c);
}

// ---- Algebraic cube formulas | مكعب المجموع والفرق ----

// مكعب المجموع: (a + b)³ = a³ + 3a²b + 3ab² + b³
// Cube of a sum
ld cubeOfSum(ld a, ld b) {
    return a * a * a + 3.0L * a * a * b + 3.0L * a * b * b + b * b * b;
}

// مكعب الفرق: (a - b)³ = a³ - 3a²b + 3ab² - b³
// Cube of a difference
ld cubeOfDifference(ld a, ld b) {
    return a * a * a - 3.0L * a * a * b + 3.0L * a * b * b - b * b * b;
}

// مجموع المكعبات: a³ + b³ = (a + b)(a² - ab + b²)
// Sum of cubes
ld sumOfCubes(ld a, ld b) {
    return (a + b) * (a * a - a * b + b * b);
}

// فرق المكعبات: a³ - b³ = (a - b)(a² + ab + b²)
// Difference of cubes
ld differenceOfCubes(ld a, ld b) {
    return (a - b) * (a * a + a * b + b * b);
}

// ---- Utility | تحويلات مساعدة ----

// شرح: تحويل من درجات لراديان
ld toRadians(ld degrees) {
    return degrees * PI / 180.0L;
}

// شرح: تحويل من راديان لدرجات
ld toDegrees(ld radians) {
    return radians * 180.0L / PI;
}


// ============================================================================
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


#endif // ICPC_GEOMETRY_HANDBOOK_H