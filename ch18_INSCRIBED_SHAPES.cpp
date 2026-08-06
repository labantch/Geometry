// ============================================================================
// Geometry Notebook — CHAPTER 18 — INSCRIBED SHAPES
// ============================================================================

// CHAPTER 18 — INSCRIBED SHAPES
// شرح: أكبر شكل (دائرة/مضلع/مثلث/مستطيل) ممكن يتحط جوه شكل تاني
// (ملحوظة: Circle struct متعرفة قبل كده في الفصل 11، مش هنكررها هنا)
// ============================================================================

// شرح: أكبر دائرة ممكن تتحط جوه مضلع محدب (مركز تشيبيشيف)، عن طريق Binary
// Search على نصف القطر + تقاطع أنصاف مستويات
// LARGEST CIRCLE INSIDE A CONVEX POLYGON (Chebyshev Center)
// For a convex polygon, find largest inscribed circle
// Uses binary search on radius + half-plane intersection
// ---- Largest circle in convex polygon ----
Circle largestCircleInConvexPolygon(vector<pt>& polygon, ld precision = 1e-7) {
    int n = polygon.size();
    Circle result;
    result.radius = 0;
    result.center = polygon[0];
    
    // Build edges as lines
    vector<Line> edges;
    for (int i = 0; i < n; i++) {
        edges.push_back(Line(polygon[i], polygon[(i+1)%n]));
    }
    
    // Binary search on radius
    ld lo = 0, hi = 1e9;
    
    // Find upper bound
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            hi = min(hi, abs(polygon[i] - polygon[j]) / 2.0L);
        }
    }
    
    while (hi - lo > precision) {
        ld mid = (lo + hi) / 2.0L;
        
        // Shift all edges inward by 'mid'
        vector<Halfplane> halfplanes;
        for (int i = 0; i < n; i++) {
            pt p1 = polygon[i];
            pt p2 = polygon[(i+1)%n];
            pt dir = normalize(p2 - p1);
            pt inward = rotate90CCW(dir); // assuming CCW polygon
            
            // Shift edge inward
            pt newP1 = p1 + inward * mid;
            pt newP2 = p2 + inward * mid;
            halfplanes.push_back(Halfplane(newP1, newP2));
        }
        
        vector<pt> intersection = halfPlaneIntersection(halfplanes);
        
        if (!intersection.empty()) {
            // If intersection exists, circle of radius 'mid' fits
            lo = mid;
            // Find center: center of mass of intersection polygon
            pt center(0,0);
            for (auto& p : intersection) {
                center += p;
            }
            center /= intersection.size();
            result.center = center;
            result.radius = mid;
        } else {
            hi = mid;
        }
    }
    
    return result;
}

// شرح: أكبر دائرة جوه مضلع عادي (مش شرط محدب)، بأسلوب عينات نقط تقريبي
// LARGEST CIRCLE IN ANY POLYGON (Simple Polygon)
// For any simple polygon, find largest inscribed circle
// Uses medial axis / Voronoi approach (simplified using point sampling)
// ---- Largest circle in any polygon ----
Circle largestCircleInPolygon(vector<pt>& polygon, int samples = 1000) {
    int n = polygon.size();
    Circle result;
    result.radius = 0;
    
    // Find bounding box
    ld minX = polygon[0].X, maxX = polygon[0].X;
    ld minY = polygon[0].Y, maxY = polygon[0].Y;
    for (auto& p : polygon) {
        minX = min(minX, p.X);
        maxX = max(maxX, p.X);
        minY = min(minY, p.Y);
        maxY = max(maxY, p.Y);
    }
    
    // Sample points inside polygon
    ld width = maxX - minX;
    ld height = maxY - minY;
    ld area = polygonArea(polygon);
    
    int gridSize = sqrt(samples * area / (width * height)) + 1;
    gridSize = max(10, gridSize);
    
    for (int i = 0; i <= gridSize; i++) {
        for (int j = 0; j <= gridSize; j++) {
            pt p(minX + width * i / gridSize, minY + height * j / gridSize);
            
            if (!pointInPolygon(polygon, p, false)) continue;
            
            // Find distance to nearest edge
            ld minDist = distancePointToPolygon(polygon, p);
            
            if (minDist > result.radius) {
                result.radius = minDist;
                result.center = p;
            }
        }
    }
    
    // Refine around best point
    if (result.radius > 0) {
        for (int iter = 0; iter < 3; iter++) {
            ld step = max(width, height) / (gridSize * 2);
            for (int dx = -2; dx <= 2; dx++) {
                for (int dy = -2; dy <= 2; dy++) {
                    pt p(result.center.X + dx * step, result.center.Y + dy * step);
                    
                    if (!pointInPolygon(polygon, p, false)) continue;
                    
                    ld minDist = distancePointToPolygon(polygon, p);
                    
                    if (minDist > result.radius) {
                        result.radius = minDist;
                        result.center = p;
                    }
                }
            }
            gridSize *= 2;
        }
    }
    
    return result;
}

// شرح: أكبر مضلع منتظم بعدد أضلاع n جوه دائرة (بيرجع رؤوس المضلع)
// LARGEST POLYGON (n sides) INSIDE A CIRCLE
// Largest n-gon inscribed in a circle of given radius
// Returns vertices of the n-gon
// ---- Largest n-gon in circle ----
vector<pt> largestPolygonInCircle(pt center, ld radius, int n) {
    vector<pt> result;
    
    if (n < 3) return result;
    
    // Regular n-gon inscribed in circle
    // Vertices at angle 0, 2π/n, 4π/n, ...
    for (int i = 0; i < n; i++) {
        ld angle = 2 * PI * i / n;
        pt vertex = center + pt(radius * cosl(angle), radius * sinl(angle));
        result.push_back(vertex);
    }
    
    return result;
}

// شرح: نفس الفكرة لكن الدائرة معرَّفة بثلاث نقط عليها بدل مركز ونصف قطر
// Largest polygon with n sides inside a circle defined by 3 points
vector<pt> largestPolygonInCircle(pt a, pt b, pt c, int n) {
    // Find circumcircle
    Circle cir;
    if (!circumcirclePolygon({a, b, c}, cir)) {
        return vector<pt>();
    }
    
    return largestPolygonInCircle(cir.center, cir.radius, n);
}

// شرح: نفس الفكرة لكن الدائرة ممررة كـ Circle كامل بدل مركز ونصف قطر منفصلين
// Largest polygon with n sides inside a circle defined by center and radius
vector<pt> largestPolygonInCircle(Circle cir, int n) {
    return largestPolygonInCircle(cir.center, cir.radius, n);
}

// شرح: أكبر مضلع بعدد أضلاع n جوه مضلع محدب (تقريبي عن طريق تحسين تكراري)
// LARGEST POLYGON (n sides) INSIDE A CONVEX POLYGON
// Find largest n-gon inside a convex polygon using rotating calipers
// Approximation using angle bisectors and binary search
// ---- Largest n-gon in convex polygon ----
vector<pt> largestNGonInConvexPolygon(vector<pt>& polygon, int n, ld precision = 1e-6) {
    int m = polygon.size();
    vector<pt> result;
    
    if (n < 3 || m < 3) return result;
    
    // For triangle inside polygon, use Welzl's algorithm
    if (n == 3) {
        // Find largest triangle in convex polygon
        ld maxArea = 0;
        vector<pt> bestTriangle;
        
        for (int i = 0; i < m; i++) {
            int k = (i + 2) % m;
            for (int j = i + 1; j < m; j++) {
                while (k != i && polygonArea({polygon[i], polygon[j], polygon[(k+1)%m]}) > 
                       polygonArea({polygon[i], polygon[j], polygon[k]})) {
                    k = (k + 1) % m;
                }
                ld area = polygonArea({polygon[i], polygon[j], polygon[k]});
                if (area > maxArea) {
                    maxArea = area;
                    bestTriangle = {polygon[i], polygon[j], polygon[k]};
                }
            }
        }
        return bestTriangle;
    }
    
    // For n > 3, use iterative optimization
    // Start with regular n-gon approximation inside polygon
    Circle inner = largestCircleInConvexPolygon(polygon);
    
    // Find largest n-gon in circle, then project to polygon boundary
    vector<pt> initial = largestPolygonInCircle(inner.center, inner.radius, n);
    
    // Move vertices outward to polygon boundary
    for (auto& p : initial) {
        // Find closest point on polygon boundary
        ld minDist = numeric_limits<ld>::max();
        pt bestPoint = p;
        
        for (int i = 0; i < m; i++) {
            pt a = polygon[i];
            pt b = polygon[(i+1)%m];
            
            // Project p onto edge
            Line edge(a, b);
            pt proj = edge.projection(p);
            
            if (onSegment(a, b, proj)) {
                ld dist = abs(p - proj);
                if (dist < minDist) {
                    minDist = dist;
                    bestPoint = proj;
                }
            }
        }
        p = bestPoint;
    }
    
    // Optimize vertices using gradient descent
    for (int iter = 0; iter < 50; iter++) {
        for (int i = 0; i < n; i++) {
            // Move vertex to maximize area
            int prev = (i - 1 + n) % n;
            int next = (i + 1) % n;
            
            pt dir = (result[next] - result[prev]);
            dir = normalize(rotate90CCW(dir));
            
            ld step = 0.001L * inner.radius;
            pt newP = result[i] + dir * step;
            
            if (pointInPolygon(polygon, newP, false)) {
                // Check if area increases
                vector<pt> testPoly = result;
                testPoly[i] = newP;
                if (polygonArea(testPoly) > polygonArea(result)) {
                    result[i] = newP;
                }
            }
        }
    }
    
    return result;
}

// شرح: أكبر مثلث جوه مضلع محدب، بأسلوب Rotating Calipers في O(n^2)
// LARGEST TRIANGLE INSIDE A CONVEX POLYGON (Optimized)
// Uses rotating calipers for O(n^2) complexity
// ---- Largest triangle ----
vector<pt> largestTriangleInConvexPolygon(vector<pt>& polygon) {
    int n = polygon.size();
    vector<pt> result;
    ld maxArea = 0;
    
    for (int i = 0; i < n; i++) {
        int k = (i + 2) % n;
        for (int j = i + 1; j < n; j++) {
            while (k != i && polygonArea({polygon[i], polygon[j], polygon[(k+1)%n]}) > 
                   polygonArea({polygon[i], polygon[j], polygon[k]})) {
                k = (k + 1) % n;
            }
            ld area = polygonArea({polygon[i], polygon[j], polygon[k]});
            if (area > maxArea) {
                maxArea = area;
                result = {polygon[i], polygon[j], polygon[k]};
            }
        }
    }
    
    return result;
}

// شرح: أكبر مستطيل محاذي للمحاور جوه مضلع محدب (تقريب حاليًا عن طريق الدائرة الداخلية)
// LARGEST RECTANGLE INSIDE A CONVEX POLYGON
// Find largest axis-aligned rectangle in convex polygon
// ---- Largest rectangle ----
vector<pt> largestRectangleInConvexPolygon(vector<pt>& polygon) {
    int n = polygon.size();
    vector<pt> result;
    ld maxArea = 0;
    
    // For each pair of edges, find rectangle
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            // Use edge i and j as opposite sides of rectangle
            pt dir1 = normalize(polygon[(i+1)%n] - polygon[i]);
            pt dir2 = rotate90CCW(dir1);
            
            // Project all points onto dir1 and dir2
            ld min1 = numeric_limits<ld>::max(), max1 = -numeric_limits<ld>::max();
            ld min2 = numeric_limits<ld>::max(), max2 = -numeric_limits<ld>::max();
            
            for (auto& p : polygon) {
                ld proj1 = dotProduct(p, dir1);
                ld proj2 = dotProduct(p, dir2);
                min1 = min(min1, proj1);
                max1 = max(max1, proj1);
                min2 = min(min2, proj2);
                max2 = max(max2, proj2);
            }
            
            // Find rectangle using rotating calipers
            // Simplified: use extents
            vector<pt> rect = {
                pt(min1, min2),
                pt(max1, min2),
                pt(max1, max2),
                pt(min1, max2)
            };
            
            // Transform back to original coordinates
            // Actually, we need to find intersection of polygon with this rectangle
            // This is a simplified version
        }
    }
    
    // For now, return a simple approximation
    Circle inner = largestCircleInConvexPolygon(polygon);
    ld side = inner.radius / sqrtl(2.0L);
    result = {
        inner.center + pt(-side, -side),
        inner.center + pt(side, -side),
        inner.center + pt(side, side),
        inner.center + pt(-side, side)
    };
    
    return result;
}

// شرح: أكبر مثلث متساوي الأضلاع جوه دائرة (بيتحول لأكبر مضلع 3 أضلاع فيها)
// LARGEST EQUILATERAL TRIANGLE IN CIRCLE
vector<pt> largestEquilateralTriangleInCircle(pt center, ld radius) {
    return largestPolygonInCircle(center, radius, 3);
}

vector<pt> largestEquilateralTriangleInCircle(Circle cir) {
    return largestPolygonInCircle(cir, 3);
}

// شرح: أكبر مثلث جوه مضلع عادي (مش شرط محدب) بأسلوب عينات نقط
// LARGEST TRIANGLE IN ANY POLYGON (not necessarily convex)
// Using point sampling for non-convex polygons
vector<pt> largestTriangleInPolygon(vector<pt>& polygon, int samples = 1000) {
    int n = polygon.size();
    vector<pt> result;
    ld maxArea = 0;
    
    // Find bounding box
    ld minX = polygon[0].X, maxX = polygon[0].X;
    ld minY = polygon[0].Y, maxY = polygon[0].Y;
    for (auto& p : polygon) {
        minX = min(minX, p.X);
        maxX = max(maxX, p.X);
        minY = min(minY, p.Y);
        maxY = max(maxY, p.Y);
    }
    
    // Sample points inside polygon
    vector<pt> insidePoints;
    int gridSize = sqrt(samples) + 1;
    
    for (int i = 0; i <= gridSize; i++) {
        for (int j = 0; j <= gridSize; j++) {
            pt p(minX + (maxX - minX) * i / gridSize,
                 minY + (maxY - minY) * j / gridSize);
            if (pointInPolygon(polygon, p, false)) {
                insidePoints.push_back(p);
            }
        }
    }
    
    // Find largest triangle among sampled points
    for (int i = 0; i < (int)insidePoints.size(); i++) {
        for (int j = i+1; j < (int)insidePoints.size(); j++) {
            for (int k = j+1; k < (int)insidePoints.size(); k++) {
                ld area = triangleArea(insidePoints[i], insidePoints[j], insidePoints[k]);
                if (area > maxArea) {
                    maxArea = area;
                    result = {insidePoints[i], insidePoints[j], insidePoints[k]};
                }
            }
        }
    }
    
    return result;
}


// ============================================================================
