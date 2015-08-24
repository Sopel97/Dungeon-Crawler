template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Rectangle<T>& b)
{
    if(a.min.x >= b.max.x) return false;
    if(a.min.y >= b.max.y) return false;
    if(a.max.x <= b.min.x) return false;
    if(a.max.y <= b.min.y) return false;
    return true;
}
template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Circle<T>& b)
{
    double closestX = clamp(b.origin.x, a.min.x, a.max.x);
    double closestY = clamp(b.origin.y, a.min.y, a.max.y);
    double distanceX = b.origin.x - closestX;
    double distanceY = b.origin.y - closestY;
    double distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    return distanceSquared <= (b.radius * b.radius);
}
template <class T>
bool Intersections::intersection(const Circle<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Vec2<T>& a, const Circle<T>& b)
{
    double dx = b.origin.x - a.x;
    dx *= dx;
    double dy = b.origin.y - a.y;
    dy *= dy;
    return dx + dy <= b.radius * b.radius;
}
template <class T>
bool Intersections::intersection(const Circle<T>& a, const Vec2<T>& b)
{
    return intersection(b, a);
}

template <class T>
bool Intersections::intersection(const Vec2<T>& a, const LineSegment<T>& b)
{
    Vec2<T> t = (b.end - b.begin) / (a - b.begin);
    if(std::abs(t.x - t.y) < EPSILON)
    {
        if(t.x >= T(0) && t.x < T(1)) return true;
    }
    return false;
}
template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const Vec2<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Vec2<T>& a, const Ray<T>& b)
{
    Vec2<T> t = (b.direction - b.origin) / (a - b.origin);
    if(std::abs(t.x - t.y) < EPSILON)
    {
        if(t.x >= T(0)) return true;
    }
    return false;
}
template <class T>
bool Intersections::intersection(const Ray<T>& a, const Vec2<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Vec2<T>& a, const Polyline<T>& b)
{
    size_t polySize = b.size();
    for(size_t i = 0; i < polySize - 1; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[i + 1];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), a)) return true;
    }
    return false;
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const Vec2<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Circle<T>& a, const Circle<T>& b)
{
    double x = fabs(a.origin.x - b.origin.x);
    double y = fabs(a.origin.y - b.origin.y);
    double radiusSum = a.radius + b.radius;
    return x * x + y * y <= radiusSum * radiusSum;
}


template <class T>
bool Intersections::intersection(const Cylinder<T>& a, const Cylinder<T>& b)
{
    if(a.origin.z > b.origin.z + b.height) return false;
    if(a.origin.z + a.height < b.origin.z) return false;
    double x = fabs(a.origin.x - b.origin.x);
    double y = fabs(a.origin.y - b.origin.y);
    double radiusSum = a.radius + b.radius;
    return x * x + y * y <= radiusSum * radiusSum;
}

template <class T>
bool Intersections::intersection(const Cuboid<T>& a, const Cuboid<T>& b)
{
    if(a.min.x >= b.max.x) return false;
    if(a.min.y >= b.max.y) return false;
    if(a.min.z >= b.max.z) return false;
    if(a.max.x <= b.min.x) return false;
    if(a.max.y <= b.min.y) return false;
    if(a.max.z <= b.min.z) return false;
    return true;
}


template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const LineSegment<T>& b, Vec2<T>& intersectionPoint)
{
    double s1_x, s1_y, s2_x, s2_y;
    s1_x = a.end.x - a.begin.x;     s1_y = a.end.y - a.begin.y;
    s2_x = b.end.x - b.begin.x;     s2_y = b.end.y - b.begin.y;

    float s, t;
    s = (-s1_y * (a.begin.x - b.begin.x) + s1_x * (a.begin.y - b.begin.y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = (s2_x * (a.begin.y - b.begin.y) - s2_y * (a.begin.x - b.begin.x)) / (-s2_x * s1_y + s1_x * s2_y);

    if(s >= T(0) && s <= T(1) && t >= T(0) && t <= T(1))
    {
        // Collision detected
        intersectionPoint.x = a.begin.x + (t * s1_x);
        intersectionPoint.y = a.begin.y + (t * s1_y);
        return true;
    }

    return false; // No collision
    /*
    double denominator1 = a.end.x - a.begin.x;
    double denominator2 = b.end.x - a.begin.x;
    double aa = (a.end.y - a.begin.y) / denominator1;
    double bb = (b.end.y - b.begin.y) / denominator2;
    double cc = (a.end.x * a.begin.y - a.begin.x * a.end.y) / denominator1;
    double dd = (b.end.x * b.begin.y - b.begin.x * b.end.y) / denominator2;
    intersectionPoint.x = (dd - cc) / (aa - bb);
    intersectionPoint.y = aa * intersectionPoint.x + cc;
    return (intersectionPoint.x < max(a.begin.x, a.end.x) && intersectionPoint.x > min(a.begin.x, a.end.x));
    */
}

template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const LineSegment<T>& b)
{
    Vec2<T> intersectionPoint;
    return intersection(a, b, intersectionPoint);
}

template <class T>
bool Intersections::intersection(const Triangle<T>& a, const Vec2<T>& b)
{
    auto sign = [](const Vec2<T>& v1, const Vec2<T>& v2, const Vec2<T>& v3) -> double
    {
        return (v1.x - v3.x) * (v2.y - v3.y) - (v2.x - v3.x) * (v1.y - v3.y);
    };

    if(sign(b, a.vertices[0], a.vertices[1]) < T(0)) return false;
    if(sign(b, a.vertices[1], a.vertices[2]) < T(0)) return false;
    if(sign(b, a.vertices[2], a.vertices[0]) < T(0)) return false;

    return true;
}
template <class T>
bool Intersections::intersection(const Vec2<T>& a, const Triangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Triangle<T>& a, const Triangle<T>& b) //do poprawy
{
    int i = 0;
    while(i < 3)
    {
        if(intersection(a.vertices[i], b)) return true;
        ++i;
    }
    while(i < 3)
    {
        if(intersection(b.vertices[i], a)) return true;
        ++i;
    }
    for(i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            if(intersection(LineSegment<T>(a.vertices[i], a.vertices[(i + 1) % 3]), LineSegment<T>(b.vertices[j], b.vertices[(j + 1) % 3]))) return true;
        }
    }
    return false;
}

template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const LineSegment<T>& b)
{
    // Find min and max X for the segment

    double minX = b.begin.x;
    double maxX = b.end.x;

    if(b.begin.x > b.end.x)
    {
        minX = b.end.x;
        maxX = b.begin.x;
    }

    // Find the intersection of the segment's and rectangle's x-projections

    if(maxX > a.max.x)
    {
        maxX = a.max.x;
    }

    if(minX < a.min.x)
    {
        minX = a.min.x;
    }

    if(minX > maxX) // If their projections do not intersect return false
    {
        return false;
    }

    // Find corresponding min and max Y for min and max X we found before

    double minY = b.begin.y;
    double maxY = b.end.y;

    double dx = b.end.x - b.begin.x;

    if(std::abs(dx) > 0.0000001)
    {
        double aa = (b.end.y - b.begin.y) / dx;
        double bb = b.begin.y - aa * b.begin.x;
        minY = aa * minX + bb;
        maxY = aa * maxX + bb;
    }

    if(minY > maxY)
    {
        double tmp = maxY;
        maxY = minY;
        minY = tmp;
    }

    // Find the intersection of the segment's and rectangle's y-projections

    if(maxY > a.max.y)
    {
        maxY = a.max.y;
    }

    if(minY < a.min.y)
    {
        minY = a.min.y;
    }

    if(minY > maxY) // If Y-projections do not intersect return false
    {
        return false;
    }

    return true;
}
template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}

template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Vec2<T>& b)
{
    return (b.x > a.min.x && b.x < a.max.x && b.y > a.min.y && b.y < a.max.y);
}
template <class T>
bool Intersections::intersection(const Vec2<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const Vec2<T>& b)
{
    auto determineQuadrant = [](const Vec2<T>& p, const Vec2<T>& c) -> int
    {
        if(p.x < c.x)
            return p.y < c.y ? 1 : 4;
        else
            return p.y < c.y ? 2 : 3;
    };
    size_t vertexCount = a.size();
    const Vec2<T>* currentVertex = &(a.vertices[vertexCount - 1]);
    int totalQuadrantCrossDelta = 0;
    for(size_t i = 0; i < vertexCount; ++i)
    {
        const Vec2<T>* nextVertex = &(a.vertices[i]);
        //quadrants
        //1 2
        //4 3
        int currentVertexQuadrant = determineQuadrant(*currentVertex, b);
        int nextVertexQuadrant = determineQuadrant(*nextVertex, b);

        int quadrantCrossDelta = nextVertexQuadrant - currentVertexQuadrant;
        switch(quadrantCrossDelta)
        {
            case 2:
            case -2:
                if((nextVertex->x - (((nextVertex->y - b.y) * (currentVertex->x - nextVertex->x)) / (currentVertex->y - nextVertex->y))) < b.x) quadrantCrossDelta = -quadrantCrossDelta;
                break;
            case 3:
                quadrantCrossDelta = -1;
                break;
            case -3:
                quadrantCrossDelta = 1;
                break;
        }

        totalQuadrantCrossDelta += quadrantCrossDelta;
        currentVertex = nextVertex;
    }
    return totalQuadrantCrossDelta == 4 || totalQuadrantCrossDelta == -4;
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const Polygon<T>& b)
{
    Vec2<T> lastVertex = a.vertices.back();
    for(const Vec2<T>& vertex : a.vertices)
    {
        Vec2<T> perp = (vertex - lastVertex).normal();
        std::pair<T, T> aProjection = a.projectMinMax(perp);
        std::pair<T, T> bProjection = b.projectMinMax(perp);
        if(aProjection.first > bProjection.second || bProjection.first > aProjection.second) return false;
        lastVertex = vertex;
    }

    lastVertex = b.vertices.back();
    for(const Vec2<T>& vertex : b.vertices)
    {
        Vec2<T> perp = (vertex - lastVertex).normal();
        std::pair<T, T> aProjection = a.projectMinMax(perp);
        std::pair<T, T> bProjection = b.projectMinMax(perp);
        if(aProjection.first > bProjection.second || bProjection.first > aProjection.second) return false;
        lastVertex = vertex;
    }
    return true;
}
template <class T>
bool Intersections::intersection(const Vec2<T>& a, const Polygon<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const Polygon<T>& b)
{
    if(intersection(a.begin, b)) return true;
    if(intersection(a.end, b)) return true;
    size_t polySize = b.size();
    for(size_t i = 0; i < polySize; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[(i + 1) % polySize];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), a)) return true;
    }
    return false;
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const LineSegment<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const Polyline<T>& b)
{
    size_t polySize = b.size();
    for(size_t i = 0; i < polySize - 1; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[i + 1];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), a)) return true;
    }
    return false;
}
template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Triangle<T>& b)
{
    return intersection(Polygon<T>::fromRectangle(a), Polygon<T>::fromTriangle(b));
}
template <class T>
bool Intersections::intersection(const Triangle<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Polyline<T>& b)
{
    if(intersection(a.asPolyline(), b)) return true;

    if(intersection(a, b.vertices.back())) return true; //only one check is needed because the polyline is either intersecting edge of the rectangle or is enclosed by it

    return false;
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const Triangle<T>& b)
{
    if(intersection(a, b.asPolyline())) return true;

    if(intersection(a.vertices.back(), b)) return true;

    return false;
}
template <class T>
bool Intersections::intersection(const Triangle<T>& a, const Polyline<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const LineSegment<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Polygon<T>& b)
{
    return intersection(Polygon<T>::fromRectangle(a), b);
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Triangle<T>& a, const Polygon<T>& b)
{
    return intersection(Polygon<T>::fromTriangle(a), b);
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const Triangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const Polyline<T>& b)
{
    size_t polySizeA = a.size();
    for(size_t i = 0; i < polySizeA - 1; ++i)
    {
        const Vec2<T>& thisVertex = a.vertices[i];
        const Vec2<T>& nextVertex = a.vertices[i + 1];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), b)) return true;
    }
    return false;
}

template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const Triangle<T>& b)
{
    return intersection(a.asPolyline(), b);
}
template <class T>
bool Intersections::intersection(const Triangle<T>& a, const LineSegment<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const Polygon<T>& b)
{
    size_t polySizeA = a.size();
    for(size_t i = 0; i < polySizeA - 1; ++i)
    {
        const Vec2<T>& thisVertex = a.vertices[i];
        const Vec2<T>& nextVertex = a.vertices[i + 1];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), b)) return true;
    }

    return false;
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const Polyline<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Circle<T>& a, const LineSegment<T>& b)
{
    return intersection(a, b.nearestPointTo(a.origin));
}
template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const Circle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Circle<T>& a, const Ray<T>& b)
{
    return intersection(a, b.nearestPointTo(a.origin));
}
template <class T>
bool Intersections::intersection(const Ray<T>& a, const Circle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const LineSegment<T>& a, const Ray<T>& b)
{
    const T& rdx = b.direction.x;
    const T& rdy = b.direction.y;
    const T& rox = b.origin.x;
    const T& roy = b.origin.y;

    const T& sbx = a.begin.x;
    const T& sby = a.begin.y;

    Vec2<T> sd = (a.end - a.begin);
    const T& sdx = sd.x;
    const T& sdy = sd.y;

    /*
        v1 = ray.origin - lineSegment.begin
        v2 = lineSegment.end - lineSegment.begin
        v3 = ray.direction.normal() //perpendicular
        t1 = (v1 . v3) / (v2 . v3)
    */
    T t1 = (rdx * (sby - roy) + rdy * (rox - sbx)) / (sdx * rdy - sdy * rdx);
    T t0 = (sbx + sdx * t1 - rox) / rdx;
    return (t0 >= T(0) && t1 >= T(0) && t1 <= T(1));
}
template <class T>
bool Intersections::intersection(const Ray<T>& a, const LineSegment<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Rectangle<T>& a, const Ray<T>& b)
{
    if(intersection(b.origin, a)) return true;

    if(intersection(LineSegment<T>(a.min, Vec2<T>(a.min.x, a.max.y)), b)) return true;
    if(intersection(LineSegment<T>(a.min, Vec2<T>(a.max.x, a.min.y)), b)) return true;
    if(intersection(LineSegment<T>(a.max, Vec2<T>(a.min.x, a.max.y)), b)) return true;
    if(intersection(LineSegment<T>(a.max, Vec2<T>(a.max.x, a.min.y)), b)) return true;

    return false;
}
template <class T>
bool Intersections::intersection(const Ray<T>& a, const Rectangle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Circle<T>& a, const Polygon<T>& b)
{
    if(intersection(a.origin, b)) return true;

    size_t polySize = b.size();
    for(size_t i = 0; i < polySize; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[(i + 1) % polySize];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), a)) return true;
    }

    return false;
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const Circle<T>& b)
{
    return intersection(b, a);
}

template <class T>
bool Intersections::intersection(const Ray<T>& a, const Ray<T>& b)
{
    T d1x = a.direction.x;
    T d1y = a.direction.y;
    T o1x = a.origin.x;
    T o1y = a.origin.y;

    T d2x = b.direction.x;
    T d2y = b.direction.y;
    T o2x = b.origin.x;
    T o2y = b.origin.y;

    T t1 = (d1x * o2y - d1y * o2x + o1x * d1y - o1y * d1y) / (d1y * d2x - d1x * d2y);
    T t2 = (o2x + d2x * t1 - o1x) / d1x;

    return (t1 >= T(0) && t2 >= T(0));
}

template <class T>
bool Intersections::intersection(const Circle<T>& a, const Polyline<T>& b)
{
    size_t polySize = b.size();
    for(size_t i = 0; i < polySize - 1; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[i + 1];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), a)) return true;
    }

    return false;
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const Circle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Circle<T>& a, const Triangle<T>& b)
{
    if(intersection(a.origin, b)) return true;

    if(intersection(LineSegment<T>(b.vertices[0], b.vertices[1]), b)) return true;
    if(intersection(LineSegment<T>(b.vertices[1], b.vertices[2]), b)) return true;
    if(intersection(LineSegment<T>(b.vertices[2], b.vertices[0]), b)) return true;

    return false;
}
template <class T>
bool Intersections::intersection(const Triangle<T>& a, const Circle<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Ray<T>& a, const Triangle<T>& b)
{
    if(intersection(a.origin, b)) return true;

    if(intersection(LineSegment<T>(b.vertices[0], b.vertices[1]), b)) return true;
    if(intersection(LineSegment<T>(b.vertices[1], b.vertices[2]), b)) return true;
    if(intersection(LineSegment<T>(b.vertices[2], b.vertices[0]), b)) return true;

    return false;
}
template <class T>
bool Intersections::intersection(const Triangle<T>& a, const Ray<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Ray<T>& a, const Polyline<T>& b)
{
    size_t polySize = b.size();
    for(size_t i = 0; i < polySize - 1; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[i + 1];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), a)) return true;
    }

    return false;
}
template <class T>
bool Intersections::intersection(const Polyline<T>& a, const Ray<T>& b)
{
    return intersection(b, a);
}
template <class T>
bool Intersections::intersection(const Ray<T>& a, const Polygon<T>& b)
{
    if(intersection(a.origin, b)) return true;

    size_t polySize = b.size();
    for(size_t i = 0; i < polySize; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[(i + 1) % polySize];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), a)) return true;
    }

    return false;
}
template <class T>
bool Intersections::intersection(const Polygon<T>& a, const Ray<T>& b)
{
    return intersection(b, a);
}
//meshes

template <class T, class V>
bool Intersections::intersection(const Mesh2<T>& a, const Mesh2<V>& b)
{
    size_t sizeA = a.size();
    size_t sizeB = b.size();
    for(size_t i = 0; i < sizeA; ++i)
    {
        for(size_t j = 0; j < sizeB; ++j)
        {
            if(intersection(a.elements[i], a.elements[j])) return true;
        }
    }
    return false;
}
template <class T, class S>
bool Intersections::intersection(const Mesh2<T>& a, const S& b)
{
    size_t size = a.size();
    for(size_t i = 0; i < size; ++i)
    {
        if(intersection(a.elements[i], b)) return true;
    }
    return false;
}
template <class T, class S>
bool Intersections::intersection(const S& a, const Mesh2<T>& b)
{
    return intersection(b, a);
}


//meshes

template <class T, class V>
bool Intersections::contains(const Mesh2<T>& a, const Mesh2<V>& b)
{
    size_t sizeA = a.size();
    size_t sizeB = b.size();
    std::vector<int> contained(false, sizeB);
    for(size_t i = 0; i < sizeA; ++i)
    {
        for(size_t j = 0; j < sizeB; ++j)
        {
            if(contains(a.elements[i], b.elements[j])) contained[j] = true;
        }
    }
    return std::find(contained.begin(), contained.end(), false) == contained.end();
}
template <class T, class S>
bool Intersections::contains(const Mesh2<T>& a, const S& b)
{
    size_t size = a.size();
    for(size_t i = 0; i < size; ++i)
    {
        if(contains(a.elements[i], b)) return true;
    }
    return false;
}
template <class T, class S>
bool Intersections::contains(const S& a, const Mesh2<T>& b)
{
    size_t size = b.size();
    for(size_t i = 0; i < size; ++i)
    {
        if(!contains(a, b.elements[i])) return false;
    }
    return true;
}

//Polymorphic
template <class T>
bool Intersections::intersection(const Shape2<T>& a, const Shape2<T>& b)
{
    return a.intersects(b);
}
template <class T, class S>
bool Intersections::intersection(const Shape2<T>& a, const S& b)
{
    return a.intersects(b);
}
template <class T, class S>
bool Intersections::intersection(const S& a, const Shape2<T>& b)
{
    return b.intersects(a);
}

//temporary
template <class T>
bool Intersections::intersection(const Vec2<T>& a, const Vec2<T>& b)
{
    return Circle<T>(a, T(5)).intersects(Circle<T>(b, T(10)));
}

/*
    containing operations
*/


//Rectangle
template <class T>
bool Intersections::contains(const Rectangle<T>& a, const Circle<T>& b)
{
    return ((a.min.x < b.origin.x - b.radius) && (a.max.x > b.origin.x + b.radius) && (a.min.y < b.origin.y - b.radius) && (a.max.y > b.origin.y + b.radius));
}
template <class T>
bool Intersections::contains(const Rectangle<T>& a, const LineSegment<T>& b)
{
    return (intersection(a, b.begin) && intersection(a, b.end));
}
template <class T>
bool Intersections::contains(const Rectangle<T>& a, const Polygon<T>& b)
{
    for(const auto& v : b.vertices)
    {
        if(!intersection(a, v)) return false;
    }
    return true;
}
template <class T>
bool Intersections::contains(const Rectangle<T>& a, const Polyline<T>& b)
{
    for(const auto& v : b.vertices)
    {
        if(!intersection(a, v)) return false;
    }
    return true;
}
template <class T>
bool Intersections::contains(const Rectangle<T>& a, const Rectangle<T>& b)
{
    return ((a.min.x < b.min.x) && (a.max.x > b.max.x) && (a.min.y < b.min.y) && (a.max.y > b.max.y));
}
template <class T>
bool Intersections::contains(const Rectangle<T>& a, const Triangle<T>& b)
{
    return (intersection(a, b.vertices[0]) && intersection(a, b.vertices[1]) && intersection(a, b.vertices[2]));
}
template <class T>
bool Intersections::contains(const Rectangle<T>& a, const Vec2<T>& b)
{
    return intersection(a, b);
}



//Triangle
template <class T>
bool Intersections::contains(const Triangle<T>& a, const Circle<T>& b)
{
    if(!contains(a, b.origin)) return false;
    if(intersection(LineSegment<T>(a.vertices[0], a.vertices[1]), b)) return false;
    if(intersection(LineSegment<T>(a.vertices[1], a.vertices[2]), b)) return false;
    if(intersection(LineSegment<T>(a.vertices[2], a.vertices[0]), b)) return false;
    return true;
}
template <class T>
bool Intersections::contains(const Triangle<T>& a, const LineSegment<T>& b)
{
    return (intersection(a, b.begin) && intersection(a, b.end));
}
template <class T>
bool Intersections::contains(const Triangle<T>& a, const Polygon<T>& b)
{
    for(const auto& v : b.vertices)
    {
        if(!intersection(a, v)) return false;
    }
    return true;
}
template <class T>
bool Intersections::contains(const Triangle<T>& a, const Polyline<T>& b)
{
    for(const auto& v : b.vertices)
    {
        if(!intersection(a, v)) return false;
    }
    return true;
}
template <class T>
bool Intersections::contains(const Triangle<T>& a, const Rectangle<T>& b)
{
    if(!intersection(a, b.min)) return false;
    if(!intersection(a, b.max)) return false;
    if(!intersection(a, Vec2<T>(b.min.x + b.max.y))) return false;
    if(!intersection(a, Vec2<T>(b.min.y + b.max.x))) return false;
    return true;
}
template <class T>
bool Intersections::contains(const Triangle<T>& a, const Triangle<T>& b)
{
    return (intersection(a, b.vertices[0]) && intersection(a, b.vertices[1]) && intersection(a, b.vertices[2]));
}
template <class T>
bool Intersections::contains(const Triangle<T>& a, const Vec2<T>& b)
{
    return intersection(a, b);
}

template <class T>
bool Intersections::contains(const Circle<T>& a, const Circle<T>& b)
{
    return a.origin.distanceTo(b.origin) < a.radius - b.radius;
}
template <class T>
bool Intersections::contains(const Circle<T>& a, const LineSegment<T>& b)
{
    return contains(a, b.begin) && contains(a, b.end);
}
template <class T>
bool Intersections::contains(const Circle<T>& a, const Polygon<T>& b)
{
    size_t polySize = b.size();
    for(size_t i = 0; i < polySize; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[(i + 1) % polySize];

        if(!contains(a, LineSegment<T>(thisVertex, nextVertex))) return false;
    }
    return true;
}
template <class T>
bool Intersections::contains(const Circle<T>& a, const Polyline<T>& b)
{
    size_t polySize = b.size();
    for(size_t i = 0; i < polySize - 1; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[i + 1];

        if(!contains(a, LineSegment<T>(thisVertex, nextVertex))) return false;
    }
    return true;
}
template <class T>
bool Intersections::contains(const Circle<T>& a, const Rectangle<T>& b)
{
    return contains(a, b.min) && contains(a, b.max);
}
template <class T>
bool Intersections::contains(const Circle<T>& a, const Triangle<T>& b)
{
    return contains(a, b.vertices[0]) && contains(a, b.vertices[1]) && contains(a, b.vertices[2]);
}
template <class T>
bool Intersections::contains(const Circle<T>& a, const Vec2<T>& b)
{
    return intersection(a, b);
}


template <class T>
bool Intersections::contains(const Polygon<T>& a, const Circle<T>& b)
{
    if(!contains(a, b.origin)) return false;

    size_t polySize = a.size();
    for(size_t i = 0; i < polySize; ++i)
    {
        const Vec2<T>& thisVertex = a.vertices[i];
        const Vec2<T>& nextVertex = a.vertices[(i + 1) % polySize];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), b)) return false;
    }
    return true;
}
template <class T>
bool Intersections::contains(const Polygon<T>& a, const LineSegment<T>& b)
{
    if(!contains(a, b.begin) && !contains(a, b.end)) return false;

    size_t polySize = a.size();
    for(size_t i = 0; i < polySize; ++i)
    {
        const Vec2<T>& thisVertex = a.vertices[i];
        const Vec2<T>& nextVertex = a.vertices[(i + 1) % polySize];

        if(intersection(LineSegment<T>(thisVertex, nextVertex), b)) return false;
    }
    return true;
}
template <class T>
bool Intersections::contains(const Polygon<T>& a, const Polygon<T>& b)
{
    size_t polySize = b.size();
    for(size_t i = 0; i < polySize; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[(i + 1) % polySize];

        if(!contains(a, LineSegment<T>(thisVertex, nextVertex))) return false;
    }
    return true;
}
template <class T>
bool Intersections::contains(const Polygon<T>& a, const Polyline<T>& b)
{
    size_t polySize = b.size();
    for(size_t i = 0; i < polySize - 1; ++i)
    {
        const Vec2<T>& thisVertex = b.vertices[i];
        const Vec2<T>& nextVertex = b.vertices[i + 1];

        if(!contains(a, LineSegment<T>(thisVertex, nextVertex))) return false;
    }
    return true;
}
template <class T>
bool Intersections::contains(const Polygon<T>& a, const Rectangle<T>& b)
{
    if(!contains(a, LineSegment<T>(b.min, Vec2<T>(b.min.x, b.max.y)))) return false;
    if(!contains(a, LineSegment<T>(b.max, Vec2<T>(b.min.x, b.max.y)))) return false;
    if(!contains(a, LineSegment<T>(b.min, Vec2<T>(b.max.x, b.min.y)))) return false;
    if(!contains(a, LineSegment<T>(b.max, Vec2<T>(b.max.x, b.min.y)))) return false;
    return true;
}
template <class T>
bool Intersections::contains(const Polygon<T>& a, const Triangle<T>& b)
{
    if(!contains(a, LineSegment<T>(b.vertices[0], b.vertices[1]))) return false;
    if(!contains(a, LineSegment<T>(b.vertices[1], b.vertices[2]))) return false;
    if(!contains(a, LineSegment<T>(b.vertices[2], b.vertices[0]))) return false;
    return true;
}
template <class T>
bool Intersections::contains(const Polygon<T>& a, const Vec2<T>& b)
{
    return intersection(a, b);
}

//Polymorphic
template <class T>
bool Intersections::contains(const Shape2<T>& a, const Shape2<T>& b)
{
    return a.contains(b);
}
template <class T, class S>
bool Intersections::contains(const Shape2<T>& a, const S& b)
{
    return a.contains(b);
}
template <class T, class S>
bool Intersections::contains(const S& a, const Shape2<T>& b)
{
    return b.isContained(a);
}

