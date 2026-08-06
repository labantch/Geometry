// ============================================================================
// Geometry Notebook — CHAPTER 1 — BASIC VECTOR OPERATIONS
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
