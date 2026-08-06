// ============================================================================
// Geometry Notebook — CHAPTER 19 — QUICK FORMULA REFERENCE (no vectors needed)
// ============================================================================

// CHAPTER 19 — QUICK FORMULA REFERENCE (no vectors needed)
// شرح: صيغ هندسية جاهزة تشتغل على أرقام عادية (مش محتاجة إحداثيات pt)
// مفيدة لما المسألة بتديك أطوال/زوايا مباشرة بدل نقط
// ============================================================================

// ---- Basic trigonometry | حساب المثلثات الأساسي ----

// شرح: جيب الزاوية = المقابل / الوتر
ld sine(ld opposite, ld hypotenuse) {
    return opposite / hypotenuse;
}

// شرح: جيب تمام الزاوية = المجاور / الوتر
ld cosine(ld adjacent, ld hypotenuse) {
    return adjacent / hypotenuse;
}

// شرح: ظل الزاوية = المقابل / المجاور
ld tangent(ld opposite, ld adjacent) {
    return opposite / adjacent;
}

// ---- Law of Sines | قانون الجيب ----

// شرح: نسبة الضلع / جيب الزاوية المقابلة له (ثابتة في أي مثلث)
ld lawOfSines(ld side, ld angle) {
    return side / sinl(angle);
}

// شرح: إيجاد ضلع مجهول باستخدام قانون الجيب
ld sideFromSine(ld knownSide, ld knownAngle, ld targetAngle) {
    return knownSide * sinl(targetAngle) / sinl(knownAngle);
}

// شرح: إيجاد زاوية مجهولة باستخدام قانون الجيب
ld angleFromSine(ld knownSide, ld knownAngle, ld targetSide) {
    return asinl(targetSide * sinl(knownAngle) / knownSide);
}

// ---- Law of Cosines | قانون جيب التمام ----

// شرح: جيب تمام الزاوية C من أطوال الأضلاع الثلاثة
ld lawOfCosines(ld a, ld b, ld c) {
    return (a * a + b * b - c * c) / (2.0L * a * b);
}

// شرح: إيجاد الضلع الثالث من ضلعين والزاوية بينهم
ld sideFromCosines(ld a, ld b, ld angleC) {
    return sqrtl(a * a + b * b - 2.0L * a * b * cosl(angleC));
}

// شرح: إيجاد زاوية من أطوال الأضلاع الثلاثة
ld angleFromCosines(ld a, ld b, ld c) {
    return acosl((a * a + b * b - c * c) / (2.0L * a * b));
}

// ---- Pythagorean theorem | نظرية فيثاغورس ----

// شرح: طول الوتر من الضلعين القائمين
ld hypotenuse(ld a, ld b) {
    return sqrtl(a * a + b * b);
}

// شرح: طول ضلع قائم من الوتر والضلع التاني
ld pythagoreanLeg(ld hyp, ld leg) {
    return sqrtl(hyp * hyp - leg * leg);
}

// ---- Distance & midpoint | المسافة ومنتصف القطعة ----

// شرح: المسافة بين نقطتين في بعدين (2D)
ld distance2D(ld x1, ld y1, ld x2, ld y2) {
    return hypotl(x2 - x1, y2 - y1);
}

// شرح: المسافة بين نقطتين في ثلاثة أبعاد (3D)
ld distance3D(ld x1, ld y1, ld z1, ld x2, ld y2, ld z2) {
    return sqrtl((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

// شرح: منتصف القطعة بين نقطتين في بعدين
pair<ld, ld> midpoint2D(ld x1, ld y1, ld x2, ld y2) {
    return {(x1 + x2) / 2.0L, (y1 + y2) / 2.0L};
}

// شرح: منتصف القطعة بين نقطتين في ثلاثة أبعاد
tuple<ld, ld, ld> midpoint3D(ld x1, ld y1, ld z1, ld x2, ld y2, ld z2) {
    return {(x1 + x2) / 2.0L, (y1 + y2) / 2.0L, (z1 + z2) / 2.0L};
}

// ---- Heron's formula | صيغة هيرون ----

// شرح: مساحة المثلث من أطوال أضلاعه الثلاثة (صيغة هيرون)
ld heronArea(ld a, ld b, ld c) {
    ld s = (a + b + c) / 2.0L;
    return sqrtl(s * (s - a) * (s - b) * (s - c));
}

// ---- Circle formulas | صيغ الدائرة ----

// شرح: قطر الدائرة من نصف القطر
ld circleDiameter(ld r) {
    return 2.0L * r;
}

// شرح: نصف قطر الدائرة من القطر
ld circleRadius(ld d) {
    return d / 2.0L;
}

// شرح: محيط الدائرة
ld circleCircumference(ld r) {
    return 2.0L * PI * r;
}

// شرح: مساحة الدائرة
ld circleArea(ld r) {
    return PI * r * r;
}

// ---- Sector formulas | صيغ القطاع الدائري ----

// شرح: طول القوس بالراديان
ld arcLength(ld r, ld theta) {
    return r * theta;
}

// شرح: طول القوس بالدرجات
ld arcLengthDegrees(ld r, ld thetaDeg) {
    return thetaDeg / 360.0L * 2.0L * PI * r;
}

// شرح: مساحة القطاع الدائري بالراديان
ld sectorArea(ld r, ld theta) {
    return 0.5L * r * r * theta;
}

// شرح: مساحة القطاع الدائري بالدرجات
ld sectorAreaDegrees(ld r, ld thetaDeg) {
    return thetaDeg / 360.0L * PI * r * r;
}

// مساحة القطاع = ½ × طول القوس × نصف القطر  (نفس ½ θ ر²)
// Sector area from arc length: (1/2) * L * r
ld sectorAreaFromArc(ld r, ld arcLen) {
    return 0.5L * r * arcLen;
}

// محيط القطاع الدائري = ٢ نق + طول القوس
// Sector perimeter = 2r + arc length
ld sectorPerimeter(ld r, ld theta) {
    return 2.0L * r + arcLength(r, theta);
}
ld sectorPerimeterDegrees(ld r, ld thetaDeg) {
    return 2.0L * r + arcLengthDegrees(r, thetaDeg);
}

// ---- Segment formulas | صيغ القطعة الدائرية (Circular Segment) ----

// شرح: مساحة القطعة الدائرية (المنطقة بين الوتر والقوس)
ld segmentArea(ld r, ld theta) {
    return 0.5L * r * r * (theta - sinl(theta));
}

// مساحة القطعة الدائرية بالدرجات
ld segmentAreaDegrees(ld r, ld thetaDeg) {
    return segmentArea(r, thetaDeg * PI / 180.0L);
}

// شرح: طول الوتر (Chord) لزاوية معينة في الدائرة
ld chordLength(ld r, ld theta) {
    return 2.0L * r * sinl(theta / 2.0L);
}

// محيط القطعة الدائرية = طول القوس + طول الوتر
// Segment perimeter = arc + chord
ld segmentPerimeter(ld r, ld theta) {
    return arcLength(r, theta) + chordLength(r, theta);
}

// ---- Triangle formulas | صيغ المثلث ----

// شرح: محيط المثلث
ld trianglePerimeter(ld a, ld b, ld c) {
    return a + b + c;
}

// شرح: مساحة المثلث = نصف القاعدة × الارتفاع
ld triangleAreaBH(ld base, ld height) {
    return 0.5L * base * height;
}

// شرح: مساحة المثلث من ضلعين والزاوية بينهم (SAS)
ld triangleAreaSAS(ld a, ld b, ld angleC) {
    return 0.5L * a * b * sinl(angleC);
}

// شرح: مساحة المثلث من الأضلاع الثلاثة (صيغة هيرون)
ld triangleAreaHeron(ld a, ld b, ld c) {
    ld s = (a + b + c) / 2.0L;
    return sqrtl(s * (s - a) * (s - b) * (s - c));
}

// شرح: مساحة المثلث من ضلعين وزاوية بالدرجات (مش راديان)
ld triangleAreaDegrees(ld a, ld b, ld angleDegrees) {
    return 0.5L * a * b * sinl(angleDegrees * PI / 180.0L);
}

// شرح: نصف قطر الدائرة المحصورة داخل المثلث (Inradius)
ld triangleInradius(ld a, ld b, ld c) {
    ld s = (a + b + c) / 2.0L;
    return triangleAreaHeron(a, b, c) / s;
}

// شرح: نصف قطر الدائرة المحيطة بالمثلث (Circumradius)
ld triangleCircumradius(ld a, ld b, ld c) {
    return (a * b * c) / (4.0L * triangleAreaHeron(a, b, c));
}

// طول المتوسط من الرأس إلى منتصف الضلع المقابل a
// Median length to side a (from the opposite vertex)
ld triangleMedian(ld a, ld b, ld c) {
    // m_a = 0.5 * sqrt(2b² + 2c² - a²)
    return 0.5L * sqrtl(2.0L * b * b + 2.0L * c * c - a * a);
}

// ---- Equilateral triangle | المثلث متساوي الأضلاع ----

// شرح: محيط المثلث متساوي الأضلاع
ld equilateralPerimeter(ld side) {
    return 3.0L * side;
}

// شرح: مساحة المثلث متساوي الأضلاع
ld equilateralArea(ld side) {
    return sqrtl(3.0L) / 4.0L * side * side;
}

// شرح: ارتفاع المثلث متساوي الأضلاع
ld equilateralHeight(ld side) {
    return sqrtl(3.0L) / 2.0L * side;
}

// شرح: نصف قطر الدائرة الداخلية للمثلث متساوي الأضلاع
ld equilateralInradius(ld side) {
    return side * sqrtl(3.0L) / 6.0L;
}

// شرح: نصف قطر الدائرة المحيطة بالمثلث متساوي الأضلاع
ld equilateralCircumradius(ld side) {
    return side / sqrtl(3.0L);
}

// ---- Rectangle | المستطيل ----

// شرح: محيط المستطيل
ld rectanglePerimeter(ld length, ld width) {
    return 2.0L * (length + width);
}

// شرح: مساحة المستطيل
ld rectangleArea(ld length, ld width) {
    return length * width;
}

// شرح: طول قطر المستطيل
ld rectangleDiagonal(ld length, ld width) {
    return sqrtl(length * length + width * width);
}

// ---- Square | المربع ----

// شرح: محيط المربع
ld squarePerimeter(ld side) {
    return 4.0L * side;
}

// شرح: مساحة المربع
ld squareArea(ld side) {
    return side * side;
}

// شرح: قطر المربع
ld squareDiagonal(ld side) {
    return side * sqrtl(2.0L);
}

// شرح: مساحة المربع من طول قطره
ld squareAreaFromDiagonal(ld diagonal) {
    return diagonal * diagonal / 2.0L;
}

// ---- Rhombus | المعين ----

// شرح: محيط المعين
ld rhombusPerimeter(ld side) {
    return 4.0L * side;
}

// شرح: مساحة المعين من قطريه
ld rhombusAreaDiagonals(ld d1, ld d2) {
    return d1 * d2 / 2.0L;
}

// شرح: مساحة المعين من القاعدة والارتفاع
ld rhombusArea(ld base, ld height) {
    return base * height;
}

// ---- Parallelogram | متوازي الأضلاع ----

// شرح: محيط متوازي الأضلاع
ld parallelogramPerimeter(ld a, ld b) {
    return 2.0L * (a + b);
}

// شرح: مساحة متوازي الأضلاع
ld parallelogramArea(ld base, ld height) {
    return base * height;
}

// مساحة متوازي الأضلاع = ضلع × ضلع × جا الزاوية المحصورة
// Area = a * b * sin(included angle)
ld parallelogramAreaSAS(ld a, ld b, ld angle) {
    return a * b * sinl(angle);
}

// ---- Trapezoid | شبه المنحرف ----

// شرح: محيط شبه المنحرف (مجموع الأضلاع الأربعة)
ld trapezoidPerimeter(ld a, ld b, ld c, ld d) {
    return a + b + c + d;
}

// شرح: مساحة شبه المنحرف من القاعدتين والارتفاع
ld trapezoidArea(ld a, ld b, ld height) {
    return (a + b) * height / 2.0L;
}

// شرح: الخط الأوسط (Median) لشبه المنحرف
ld trapezoidMedian(ld a, ld b) {
    return (a + b) / 2.0L;
}

// شرح: مساحة شبه المنحرف من الخط الأوسط والارتفاع
ld trapezoidAreaFromMedian(ld median, ld height) {
    return median * height;
}

// ---- Kite | الطائرة الورقية (شكل الطائرة) ----

// شرح: محيط شكل الطائرة الورقية
ld kitePerimeter(ld a, ld b) {
    return 2.0L * (a + b);
}

// شرح: مساحة شكل الطائرة الورقية من قطريها
ld kiteArea(ld d1, ld d2) {
    return d1 * d2 / 2.0L;
}

// ---- Regular polygon | المضلع المنتظم ----

// شرح: محيط المضلع المنتظم (n ضلع، طول كل ضلع side)
ld regularPolygonPerimeter(int n, ld side) {
    return n * side;
}

// شرح: مساحة المضلع المنتظم من عدد الأضلاع وطول الضلع
ld regularPolygonArea(int n, ld side) {
    return (n * side * side) / (4.0L * tanl(PI / n));
}

// شرح: مساحة المضلع المنتظم من نصف قطر الدائرة المحيطة (Circumradius)
ld regularPolygonAreaFromCircumradius(int n, ld R) {
    return n * R * R * sinl(2.0L * PI / n) / 2.0L;
}

// شرح: مساحة المضلع المنتظم من نصف قطر الدائرة الداخلية (Inradius)
ld regularPolygonAreaFromInradius(int n, ld r) {
    return n * r * r * tanl(PI / n);
}

// شرح: مساحة المضلع المنتظم من المحيط والسهم (Apothem)
ld regularPolygonAreaWithApothem(int n, ld side, ld apothem) {
    return regularPolygonPerimeter(n, side) * apothem / 2.0L;
}

// شرح: نصف قطر الدائرة المحيطة بالمضلع المنتظم
ld regularPolygonCircumradius(int n, ld side) {
    return side / (2.0L * sinl(PI / n));
}

// شرح: نصف قطر الدائرة الداخلية للمضلع المنتظم (السهم Apothem)
ld regularPolygonInradius(int n, ld side) {
    return side / (2.0L * tanl(PI / n));
}

// ---- Ellipse | القطع الناقص ----

// شرح: مساحة القطع الناقص من نصفي محوريه
ld ellipseArea(ld a, ld b) {
    return PI * a * b;
}

// ---- 3D shapes | الأشكال المجسمة (ثلاثية الأبعاد) ----

// شرح: حجم المكعب
ld cubeVolume(ld side) {
    return side * side * side;
}

// مساحة وجه المكعب
// Area of one face of a cube
ld cubeFaceArea(ld side) {
    return side * side;
}

// شرح: المساحة السطحية الكلية للمكعب
ld cubeSurfaceArea(ld side) {
    return 6.0L * side * side;
}

// شرح: المساحة الجانبية للمكعب (بدون القاعدتين)
ld cubeLateralArea(ld side) {
    return 4.0L * side * side;
}

// شرح: قطر المكعب (من ركن لركن مقابل له)
ld cubeDiagonal(ld side) {
    return side * sqrtl(3.0L);
}

// شرح: حجم متوازي المستطيلات (الصندوق)
ld cuboidVolume(ld l, ld w, ld h) {
    return l * w * h;
}

// شرح: المساحة السطحية الكلية لمتوازي المستطيلات
ld cuboidSurfaceArea(ld l, ld w, ld h) {
    return 2.0L * (l * w + l * h + w * h);
}

// شرح: المساحة الجانبية لمتوازي المستطيلات
ld cuboidLateralArea(ld l, ld w, ld h) {
    return 2.0L * h * (l + w);
}

// شرح: قطر متوازي المستطيلات
ld cuboidDiagonal(ld l, ld w, ld h) {
    return sqrtl(l * l + w * w + h * h);
}

// شرح: حجم الأسطوانة
ld cylinderVolume(ld r, ld h) {
    return PI * r * r * h;
}

// مساحة قاعدة الأسطوانة
ld cylinderBaseArea(ld r) {
    return PI * r * r;
}

// شرح: المساحة الجانبية للأسطوانة
ld cylinderLateralArea(ld r, ld h) {
    return 2.0L * PI * r * h;
}

// شرح: المساحة السطحية الكلية للأسطوانة (شامل القاعدتين)
ld cylinderSurfaceArea(ld r, ld h) {
    return 2.0L * PI * r * (r + h);
}

// شرح: حجم المخروط
ld coneVolume(ld r, ld h) {
    return PI * r * r * h / 3.0L;
}

// شرح: الميل الجانبي (Slant Height) للمخروط
ld coneSlantHeight(ld r, ld h) {
    return sqrtl(r * r + h * h);
}

// شرح: المساحة الجانبية للمخروط
ld coneLateralArea(ld r, ld slantHeight) {
    return PI * r * slantHeight;
}

// شرح: المساحة السطحية الكلية للمخروط
ld coneSurfaceArea(ld r, ld slantHeight) {
    return PI * r * (r + slantHeight);
}

// المساحة الجانبية للمخروط من نق والارتفاع
ld coneLateralAreaRH(ld r, ld h) {
    return coneLateralArea(r, coneSlantHeight(r, h));
}

// المساحة الكلية للمخروط من نق والارتفاع
ld coneSurfaceAreaRH(ld r, ld h) {
    return coneSurfaceArea(r, coneSlantHeight(r, h));
}

// شرح: حجم الكرة
ld sphereVolume(ld r) {
    return (4.0L / 3.0L) * PI * r * r * r;
}

// شرح: المساحة السطحية للكرة
ld sphereSurfaceArea(ld r) {
    return 4.0L * PI * r * r;
}

// شرح: حجم نصف الكرة
ld hemisphereVolume(ld r) {
    return (2.0L / 3.0L) * PI * r * r * r;
}

// شرح: مساحة الجزء المنحني (القبة) لنصف الكرة
ld hemisphereCurvedArea(ld r) {
    return 2.0L * PI * r * r;
}

// شرح: المساحة السطحية الكلية لنصف الكرة (القبة + القاعدة)
ld hemisphereTotalArea(ld r) {
    return 3.0L * PI * r * r;
}

// شرح: حجم المنشور من مساحة القاعدة والارتفاع
ld prismVolume(ld baseArea, ld height) {
    return baseArea * height;
}

// شرح: المساحة الجانبية للمنشور
ld prismLateralArea(ld basePerimeter, ld height) {
    return basePerimeter * height;
}

// شرح: المساحة السطحية الكلية للمنشور
ld prismSurfaceArea(ld baseArea, ld basePerimeter, ld height) {
    return 2.0L * baseArea + basePerimeter * height;
}

// شرح: حجم الهرم من مساحة القاعدة والارتفاع
ld pyramidVolume(ld baseArea, ld height) {
    return baseArea * height / 3.0L;
}

// شرح: المساحة الجانبية للهرم
ld pyramidLateralArea(ld basePerimeter, ld slantHeight) {
    return 0.5L * basePerimeter * slantHeight;
}

// شرح: المساحة السطحية الكلية للهرم
ld pyramidSurfaceArea(ld baseArea, ld basePerimeter, ld slantHeight) {
    return pyramidLateralArea(basePerimeter, slantHeight) + baseArea;
}

// شرح: ارتفاع الهرم المنتظم من طول الحرف الجانبي
ld regularPyramidHeight(int n, ld edge) {
    ld R = regularPolygonCircumradius(n, edge);
    return sqrtl(edge * edge - R * R);
}

// شرح: حجم الهرم المنتظم من طول الحرف الجانبي
ld regularPyramidVolume(int n, ld edge) {
    return regularPolygonArea(n, edge) * regularPyramidHeight(n, edge) / 3.0L;
}

// ---- Regular star | النجمة المنتظمة ----

// شرح: مساحة نجمة منتظمة بعدد رؤوس n ونصف قطر خارجي r
ld regularStarArea(int n, ld r) {
    ld theta = PI / n;
    return n * r * r * sinl(2.0L * theta) * sinl(theta)
           / (2.0L * (sinl(3.0L * theta) + sinl(theta)));
}

// ---- Quadratic equation | المعادلة التربيعية ax² + bx + c = 0 ----

// المميز: Δ = b² - 4ac
// Discriminant
ld quadraticDiscriminant(ld a, ld b, ld c) {
    return b * b - 4.0L * a * c;
}

// حل المعادلة التربيعية ax² + bx + c = 0
// يرجع عدد الجذور الحقيقية (0 / 1 / 2) ويكتبها في out
// Solve ax² + bx + c = 0; returns number of real roots (0/1/2)
int solveQuadratic(ld a, ld b, ld c, pair<ld, ld>& out) {
    // Linear case: bx + c = 0
    if (fabsl(a) < EPS) {
        if (fabsl(b) < EPS) return 0; // no equation or infinite solutions
        out = {-c / b, -c / b};
        return 1;
    }
    ld disc = quadraticDiscriminant(a, b, c);
    if (disc < -EPS) return 0; // no real roots
    if (fabsl(disc) <= EPS) {
        ld x = -b / (2.0L * a);
        out = {x, x};
        return 1;
    }
    ld s = sqrtl(disc);
    // Numerically stable form
    ld q = -0.5L * (b + (b >= 0 ? s : -s));
    ld x1 = q / a;
    ld x2 = c / q;
    if (x1 > x2) swap(x1, x2);
    out = {x1, x2};
    return 2;
}

// مجموع الجذور = -b/a  |  حاصل ضرب الجذور = c/a  (فييت)
// Sum of roots = -b/a
ld quadraticSumOfRoots(ld a, ld b, ld c) {
    return -b / a;
}
// Product of roots = c/a
ld quadraticProductOfRoots(ld a, ld b, ld c) {
    return c / a;
}

// ---- Algebraic square formulas | المتطابقات الجبرية (مربع المجموع والفرق) ----

// مربع المجموع: (a + b)² = a² + 2ab + b²
// Square of a sum
ld squareOfSum(ld a, ld b) {
    return a * a + 2.0L * a * b + b * b;
}

// مربع الفرق: (a - b)² = a² - 2ab + b²
// Square of a difference
ld squareOfDifference(ld a, ld b) {
    return a * a - 2.0L * a * b + b * b;
}

// فرق المربعين: a² - b² = (a - b)(a + b)
// Difference of squares
ld differenceOfSquares(ld a, ld b) {
    return (a - b) * (a + b);
}

// مربع المجموع لثلاثة حدود: (a + b + c)² = a² + b² + c² + 2ab + 2ac + 2bc
// Square of sum of three terms
ld squareOfSum3(ld a, ld b, ld c) {
    return a * a + b * b + c * c + 2.0L * (a * b + a * c + b * c);
}

// ---- Algebraic cube formulas | مكعب المجموع والفرق ----

// مكعب المجموع: (a + b)³ = a³ + 3a²b + 3ab² + b³
// Cube of a sum
ld cubeOfSum(ld a, ld b) {
    return a * a * a + 3.0L * a * a * b + 3.0L * a * b * b + b * b * b;
}

// مكعب الفرق: (a - b)³ = a³ - 3a²b + 3ab² - b³
// Cube of a difference
ld cubeOfDifference(ld a, ld b) {
    return a * a * a - 3.0L * a * a * b + 3.0L * a * b * b - b * b * b;
}

// مجموع المكعبات: a³ + b³ = (a + b)(a² - ab + b²)
// Sum of cubes
ld sumOfCubes(ld a, ld b) {
    return (a + b) * (a * a - a * b + b * b);
}

// فرق المكعبات: a³ - b³ = (a - b)(a² + ab + b²)
// Difference of cubes
ld differenceOfCubes(ld a, ld b) {
    return (a - b) * (a * a + a * b + b * b);
}

// ---- Utility | تحويلات مساعدة ----

// شرح: تحويل من درجات لراديان
ld toRadians(ld degrees) {
    return degrees * PI / 180.0L;
}

// شرح: تحويل من راديان لدرجات
ld toDegrees(ld radians) {
    return radians * 180.0L / PI;
}


// ============================================================================
// ============================================================================
