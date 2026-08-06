// ============================================================================
// Geometry Notebook — CHAPTER 9 — MINKOWSKI SUM
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
