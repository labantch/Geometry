// ============================================================================
// Geometry Notebook — CHAPTER 12 — LATTICE POINTS (Pick's Theorem)
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
