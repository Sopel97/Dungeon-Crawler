#ifndef CONVEXHULL_H_INCLUDED
#define CONVEXHULL_H_INCLUDED

#include "Vec2.h"
#include "Polygon.h"

template <class T>
class ConvexHull
{
public:
    ConvexHull(const std::vector<Vec2<T>>& p);
    ConvexHull(std::vector<Vec2<T>>&& p);

    void calculate();
    void sortPoints();

    const Polygon<T>& convexHull();
    const std::vector<Vec2<T>>& points();
private:
    std::vector<Vec2<T>> m_points;
    Polygon<T> m_convexHull;
    bool m_sorted;

    T cross(const Vec2<T>& o, const Vec2<T>& a, const Vec2<T>& b);
};

typedef ConvexHull<double> ConvexHullD;
typedef ConvexHull<float> ConvexHullF;
typedef ConvexHull<int> ConvexHullI;

#include "../src/ConvexHull.cpp"

#endif // CONVEXHULL_H_INCLUDED
