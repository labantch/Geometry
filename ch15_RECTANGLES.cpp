// ============================================================================
// Geometry Notebook — CHAPTER 15 — RECTANGLES
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
