// ============================================================================
// Geometry Notebook — CHAPTER 2 — TRANSFORMATIONS
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
