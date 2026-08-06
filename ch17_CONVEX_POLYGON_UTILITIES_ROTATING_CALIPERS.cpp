// ============================================================================
// Geometry Notebook — CHAPTER 17 — CONVEX POLYGON UTILITIES (ROTATING CALIPERS)
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
