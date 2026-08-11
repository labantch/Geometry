// ============================================================================
// Geometry Notebook — CHAPTER 0 — SETUP & CONSTANTS
// ============================================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================================

// CHAPTER 0 — SETUP & CONSTANTS
// شرح: التعريفات الأساسية اللي كل حاجة في المكتبة بتتبني عليها
// ============================================================================

// ---- Types & constants ----
typedef long double ld;
typedef complex<ld> pt;

const ld EPS = 1e-9L;
const ld PI  = acosl(-1.0L);

// Helper functions to access real and imaginary parts
// شرح: اختصار عشان نقدر نكتب p.X و p.Y بدل p.real() و p.imag()
// ---- Accessors ----
#define X real()
#define Y imag()


struct pt {
    ld x, y;

    pt() : x(0), y(0) {}
    pt(ld x_, ld y_) : x(x_), y(y_) {}

    // ---- arithmetic ----
    pt operator+(const pt& o) const { return {x + o.x, y + o.y}; }
    pt operator-(const pt& o) const { return {x - o.x, y - o.y}; }
    pt operator*(ld k) const { return {x * k, y * k}; }
    pt operator/(ld k) const { return {x / k, y / k}; }
    pt& operator+=(const pt& o) { x += o.x; y += o.y; return *this; }
    pt& operator-=(const pt& o) { x -= o.x; y -= o.y; return *this; }
    pt& operator*=(ld k) { x *= k; y *= k; return *this; }
    pt& operator/=(ld k) { x /= k; y /= k; return *this; }
    pt operator-() const { return {-x, -y}; }

    // ---- comparison (for set / sort) ----
    bool operator==(const pt& o) const {
        return fabsl(x - o.x) < EPS && fabsl(y - o.y) < EPS;
    }
    bool operator!=(const pt& o) const { return !(*this == o); }
    bool operator<(const pt& o) const {
        if (fabsl(x - o.x) > EPS) return x < o.x;
        return y < o.y - EPS;
    }
}
// ============================================================================
