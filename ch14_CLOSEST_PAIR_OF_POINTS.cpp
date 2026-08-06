// ============================================================================
// Geometry Notebook — CHAPTER 14 — CLOSEST PAIR OF POINTS
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
