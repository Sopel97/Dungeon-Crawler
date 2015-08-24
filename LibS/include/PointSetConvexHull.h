#ifndef POINTSETCONVEXHULL_H_INCLUDED
#define POINTSETCONVEXHULL_H_INCLUDED

#include "Vec2.h"
#include "Polygon.h"

template <class T>
class PointSetConvexHull
{
public:
    PointSetConvexHull(const std::vector<Vec2<T>>& p);
    PointSetConvexHull(std::vector<Vec2<T>>&& p);

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

typedef PointSetConvexHull<double> PointSetConvexHullD;
typedef PointSetConvexHull<float> PointSetConvexHullF;
typedef PointSetConvexHull<int> PointSetConvexHullI;

extern template class PointSetConvexHull<double>;
extern template class PointSetConvexHull<float>;
extern template class PointSetConvexHull<int>;

#include "../src/PointSetConvexHull.cpp"

#endif // POINTSETCONVEXHULL_H_INCLUDED
