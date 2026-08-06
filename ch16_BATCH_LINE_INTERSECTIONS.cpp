// ============================================================================
// Geometry Notebook — CHAPTER 16 — BATCH LINE INTERSECTIONS
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
