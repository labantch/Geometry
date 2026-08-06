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

// ============================================================================
