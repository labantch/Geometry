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


// حدث خط المسح لاتحاد مستطيلات
struct UnionEvent {
    int x, y1, y2, type; // +1 start, -1 end
    bool operator<(const UnionEvent& o) const { return x < o.x; }
};

// شجرة قطاعات لطول الغطاء على محور y
struct SegTree {
    struct Node { int cover; ld length; };
    vector<Node> st;
    vector<int> ys;
    int N;

    SegTree(const vector<int>& _ys) : ys(_ys) {
        N = (int)ys.size() - 1;
        st.assign(4 * max(1, N), {0, 0});
    }

    void update(int p, int l, int r, int ql, int qr, int val) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            st[p].cover += val;
        } else {
            int m = (l + r) / 2;
            update(p << 1, l, m, ql, qr, val);
            update(p << 1 | 1, m, r, ql, qr, val);
        }
        if (st[p].cover > 0)
            st[p].length = (ld)(ys[r] - ys[l]);
        else if (l + 1 == r)
            st[p].length = 0;
        else
            st[p].length = st[p << 1].length + st[p << 1 | 1].length;
    }

    void update(int y1, int y2, int v) {
        int l = lower_bound(ys.begin(), ys.end(), y1) - ys.begin();
        int r = lower_bound(ys.begin(), ys.end(), y2) - ys.begin();
        update(1, 0, N, l, r, v);
    }

    ld coveredLength() const { return st[1].length; }
};

// مساحة اتحاد مستطيلات محاذاة للمحاور {x1,y1,x2,y2}
ld areaUnionRectangles(const vector<array<int, 4>>& rectangles) {
    vector<UnionEvent> events;
    vector<int> ys;
    for (auto& r : rectangles) {
        int x1 = r[0], y1 = r[1], x2 = r[2], y2 = r[3];
        events.push_back({x1, y1, y2, +1});
        events.push_back({x2, y1, y2, -1});
        ys.push_back(y1); ys.push_back(y2);
    }
    if (events.empty()) return 0;
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    SegTree st(ys);
    sort(events.begin(), events.end());
    ld area = 0;
    int prev_x = events[0].x;
    for (auto& e : events) {
        area += st.coveredLength() * (e.x - prev_x);
        st.update(e.y1, e.y2, e.type);
        prev_x = e.x;
    }
    return area;
}


// ============================================================================
