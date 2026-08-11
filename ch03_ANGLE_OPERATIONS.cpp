// ============================================================================
// Geometry Notebook — CHAPTER 3 — ANGLE OPERATIONS
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

// الزاوية الموقّعة بين متجهين (موجب = عكس عقارب الساعة)
// Signed angle from v to w in radians (CCW positive)
ld signedAngleBetween(pt v, pt w) {
    return atan2l(cross(v, w), dotProduct(v, w));
}

// هل النقطة في النصف العلوي؟ (للـ polar sort)
// Upper half-plane test used in polar sort
bool half(pt p) {
    return p.Y > 0 || (fabsl(p.Y) < EPS && p.X < 0);
}


// ============================================================================
