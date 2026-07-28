// GeometrySimple.hpp - Complete Geometry Formula Library
// All functions are reusable - just call them directly!

#ifndef GEOMETRY_SIMPLE_HPP
#define GEOMETRY_SIMPLE_HPP

#include <bits/stdc++.h>
using namespace std;

using ld = long double;

const ld PI = acosl(-1.0L);

//==================================================
// BASIC TRIGONOMETRY
//==================================================

ld sine(ld opposite, ld hypotenuse) {
    return opposite / hypotenuse;
}

ld cosine(ld adjacent, ld hypotenuse) {
    return adjacent / hypotenuse;
}

ld tangent(ld opposite, ld adjacent) {
    return opposite / adjacent;
}

//==================================================
// LAW OF SINES
//==================================================

ld lawOfSines(ld side, ld angle) {
    return side / sinl(angle);
}

ld sideFromSine(ld knownSide, ld knownAngle, ld targetAngle) {
    return knownSide * sinl(targetAngle) / sinl(knownAngle);
}

ld angleFromSine(ld knownSide, ld knownAngle, ld targetSide) {
    return asinl(targetSide * sinl(knownAngle) / knownSide);
}

//==================================================
// LAW OF COSINES
//==================================================

ld lawOfCosines(ld a, ld b, ld c) {
    return (a * a + b * b - c * c) / (2.0L * a * b);
}

ld sideFromCosines(ld a, ld b, ld angleC) {
    return sqrtl(a * a + b * b - 2.0L * a * b * cosl(angleC));
}

ld angleFromCosines(ld a, ld b, ld c) {
    return acosl((a * a + b * b - c * c) / (2.0L * a * b));
}

//==================================================
// PYTHAGOREAN THEOREM
//==================================================

ld hypotenuse(ld a, ld b) {
    return sqrtl(a * a + b * b);
}

ld pythagoreanLeg(ld hyp, ld leg) {
    return sqrtl(hyp * hyp - leg * leg);
}

//==================================================
// DISTANCE & MIDPOINT
//==================================================

ld distance2D(ld x1, ld y1, ld x2, ld y2) {
    return hypotl(x2 - x1, y2 - y1);
}

ld distance3D(ld x1, ld y1, ld z1, ld x2, ld y2, ld z2) {
    return sqrtl((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

pair<ld, ld> midpoint2D(ld x1, ld y1, ld x2, ld y2) {
    return {(x1 + x2) / 2.0L, (y1 + y2) / 2.0L};
}

tuple<ld, ld, ld> midpoint3D(ld x1, ld y1, ld z1, ld x2, ld y2, ld z2) {
    return {(x1 + x2) / 2.0L, (y1 + y2) / 2.0L, (z1 + z2) / 2.0L};
}

//==================================================
// HEROIN'S FORMULA
//==================================================

ld heronArea(ld a, ld b, ld c) {
    ld s = (a + b + c) / 2.0L;
    return sqrtl(s * (s - a) * (s - b) * (s - c));
}

//==================================================
// CIRCLE FORMULAS
//==================================================

ld circleDiameter(ld r) {
    return 2.0L * r;
}

ld circleRadius(ld d) {
    return d / 2.0L;
}

ld circleCircumference(ld r) {
    return 2.0L * PI * r;
}

ld circleArea(ld r) {
    return PI * r * r;
}

//==================================================
// SECTOR FORMULAS
//==================================================

ld arcLength(ld r, ld theta) {
    return r * theta;
}

ld arcLengthDegrees(ld r, ld thetaDeg) {
    return thetaDeg / 360.0L * 2.0L * PI * r;
}

ld sectorArea(ld r, ld theta) {
    return 0.5L * r * r * theta;
}

ld sectorAreaDegrees(ld r, ld thetaDeg) {
    return thetaDeg / 360.0L * PI * r * r;
}

//==================================================
// SEGMENT FORMULAS
//==================================================

ld segmentArea(ld r, ld theta) {
    return 0.5L * r * r * (theta - sinl(theta));
}

ld chordLength(ld r, ld theta) {
    return 2.0L * r * sinl(theta / 2.0L);
}

//==================================================
// TRIANGLE FORMULAS
//==================================================

ld trianglePerimeter(ld a, ld b, ld c) {
    return a + b + c;
}

ld triangleArea(ld base, ld height) {
    return 0.5L * base * height;
}

ld triangleAreaSAS(ld a, ld b, ld angleC) {
    return 0.5L * a * b * sinl(angleC);
}

ld triangleAreaHeron(ld a, ld b, ld c) {
    ld s = (a + b + c) / 2.0L;
    return sqrtl(s * (s - a) * (s - b) * (s - c));
}

ld triangleAreaDegrees(ld a, ld b, ld angleDegrees) {
    return 0.5L * a * b * sinl(angleDegrees * PI / 180.0L);
}

ld triangleInradius(ld a, ld b, ld c) {
    ld s = (a + b + c) / 2.0L;
    return triangleAreaHeron(a, b, c) / s;
}

ld triangleCircumradius(ld a, ld b, ld c) {
    return (a * b * c) / (4.0L * triangleAreaHeron(a, b, c));
}

//==================================================
// EQUILATERAL TRIANGLE
//==================================================

ld equilateralPerimeter(ld side) {
    return 3.0L * side;
}

ld equilateralArea(ld side) {
    return sqrtl(3.0L) / 4.0L * side * side;
}

ld equilateralHeight(ld side) {
    return sqrtl(3.0L) / 2.0L * side;
}

ld equilateralInradius(ld side) {
    return side * sqrtl(3.0L) / 6.0L;
}

ld equilateralCircumradius(ld side) {
    return side / sqrtl(3.0L);
}

//==================================================
// RECTANGLE
//==================================================

ld rectanglePerimeter(ld length, ld width) {
    return 2.0L * (length + width);
}

ld rectangleArea(ld length, ld width) {
    return length * width;
}

ld rectangleDiagonal(ld length, ld width) {
    return sqrtl(length * length + width * width);
}

//==================================================
// SQUARE
//==================================================

ld squarePerimeter(ld side) {
    return 4.0L * side;
}

ld squareArea(ld side) {
    return side * side;
}

ld squareDiagonal(ld side) {
    return side * sqrtl(2.0L);
}

ld squareAreaFromDiagonal(ld diagonal) {
    return diagonal * diagonal / 2.0L;
}

//==================================================
// RHOMBUS
//==================================================

ld rhombusPerimeter(ld side) {
    return 4.0L * side;
}

ld rhombusAreaDiagonals(ld d1, ld d2) {
    return d1 * d2 / 2.0L;
}

ld rhombusArea(ld base, ld height) {
    return base * height;
}

//==================================================
// PARALLELOGRAM
//==================================================

ld parallelogramPerimeter(ld a, ld b) {
    return 2.0L * (a + b);
}

ld parallelogramArea(ld base, ld height) {
    return base * height;
}

//==================================================
// TRAPEZOID
//==================================================

ld trapezoidPerimeter(ld a, ld b, ld c, ld d) {
    return a + b + c + d;
}

ld trapezoidArea(ld a, ld b, ld height) {
    return (a + b) * height / 2.0L;
}

ld trapezoidMedian(ld a, ld b) {
    return (a + b) / 2.0L;
}

ld trapezoidAreaFromMedian(ld median, ld height) {
    return median * height;
}

//==================================================
// KITE
//==================================================

ld kitePerimeter(ld a, ld b) {
    return 2.0L * (a + b);
}

ld kiteArea(ld d1, ld d2) {
    return d1 * d2 / 2.0L;
}

//==================================================
// REGULAR POLYGON
//==================================================

ld regularPolygonPerimeter(int n, ld side) {
    return n * side;
}

ld regularPolygonArea(int n, ld side) {
    return (n * side * side) / (4.0L * tanl(PI / n));
}

ld regularPolygonAreaFromCircumradius(int n, ld R) {
    return n * R * R * sinl(2.0L * PI / n) / 2.0L;
}

ld regularPolygonAreaFromInradius(int n, ld r) {
    return n * r * r * tanl(PI / n);
}

ld regularPolygonAreaWithApothem(int n, ld side, ld apothem) {
    return regularPolygonPerimeter(n, side) * apothem / 2.0L;
}

ld regularPolygonCircumradius(int n, ld side) {
    return side / (2.0L * sinl(PI / n));
}

ld regularPolygonInradius(int n, ld side) {
    return side / (2.0L * tanl(PI / n));
}

//==================================================
// ELLIPSE
//==================================================

ld ellipseArea(ld a, ld b) {
    return PI * a * b;
}

//==================================================
// 3D SHAPES
//==================================================

ld cubeVolume(ld side) {
    return side * side * side;
}

ld cubeSurfaceArea(ld side) {
    return 6.0L * side * side;
}

ld cubeLateralArea(ld side) {
    return 4.0L * side * side;
}

ld cubeDiagonal(ld side) {
    return side * sqrtl(3.0L);
}

ld cuboidVolume(ld l, ld w, ld h) {
    return l * w * h;
}

ld cuboidSurfaceArea(ld l, ld w, ld h) {
    return 2.0L * (l * w + l * h + w * h);
}

ld cuboidLateralArea(ld l, ld w, ld h) {
    return 2.0L * h * (l + w);
}

ld cuboidDiagonal(ld l, ld w, ld h) {
    return sqrtl(l * l + w * w + h * h);
}

ld cylinderVolume(ld r, ld h) {
    return PI * r * r * h;
}

ld cylinderLateralArea(ld r, ld h) {
    return 2.0L * PI * r * h;
}

ld cylinderSurfaceArea(ld r, ld h) {
    return 2.0L * PI * r * (r + h);
}

ld coneVolume(ld r, ld h) {
    return PI * r * r * h / 3.0L;
}

ld coneSlantHeight(ld r, ld h) {
    return sqrtl(r * r + h * h);
}

ld coneLateralArea(ld r, ld slantHeight) {
    return PI * r * slantHeight;
}

ld coneSurfaceArea(ld r, ld slantHeight) {
    return PI * r * (r + slantHeight);
}

ld sphereVolume(ld r) {
    return (4.0L / 3.0L) * PI * r * r * r;
}

ld sphereSurfaceArea(ld r) {
    return 4.0L * PI * r * r;
}

ld hemisphereVolume(ld r) {
    return (2.0L / 3.0L) * PI * r * r * r;
}

ld hemisphereCurvedArea(ld r) {
    return 2.0L * PI * r * r;
}

ld hemisphereTotalArea(ld r) {
    return 3.0L * PI * r * r;
}

ld prismVolume(ld baseArea, ld height) {
    return baseArea * height;
}

ld prismLateralArea(ld basePerimeter, ld height) {
    return basePerimeter * height;
}

ld prismSurfaceArea(ld baseArea, ld basePerimeter, ld height) {
    return 2.0L * baseArea + basePerimeter * height;
}

ld pyramidVolume(ld baseArea, ld height) {
    return baseArea * height / 3.0L;
}

ld pyramidLateralArea(ld basePerimeter, ld slantHeight) {
    return 0.5L * basePerimeter * slantHeight;
}

ld pyramidSurfaceArea(ld baseArea, ld basePerimeter, ld slantHeight) {
    return pyramidLateralArea(basePerimeter, slantHeight) + baseArea;
}

ld regularPyramidHeight(int n, ld edge) {
    ld R = regularPolygonCircumradius(n, edge);
    return sqrtl(edge * edge - R * R);
}

ld regularPyramidVolume(int n, ld edge) {
    return regularPolygonArea(n, edge) * regularPyramidHeight(n, edge) / 3.0L;
}

//==================================================
// REGULAR STAR
//==================================================

ld regularStarArea(int n, ld r) {
    ld theta = PI / n;
    return n * r * r * sinl(2.0L * theta) * sinl(theta)
           / (2.0L * (sinl(3.0L * theta) + sinl(theta)));
}

//==================================================
// UTILITY FUNCTIONS
//==================================================

ld toRadians(ld degrees) {
    return degrees * PI / 180.0L;
}

ld toDegrees(ld radians) {
    return radians * 180.0L / PI;
}

#endif // GEOMETRY_SIMPLE_HPP