#ifndef BEZIERPATH_H
#define BEZIERPATH_H

template <class T>
class BezierPath
{
public:
    std::vector<Vec2<T>> controlPoints;

    BezierPath() = default;

    void add(const Vec2<T>& point);
    void add(const std::vector<Vec2<T>>& points);
    void add(std::initializer_list<Vec2<T>> points);

    int degree() const;

    Vec2<T> evaluate(T t) const;

    std::vector<Vec2<T>> evaluateAll(T step) const;
private:
    static const typename BezierCurve<T>::BinomialCoefficientsLookup m_binomialCoefficients;
};

template <class T>
const typename BezierCurve<T>::BinomialCoefficientsLookup BezierPath<T>::m_binomialCoefficients = typename BezierCurve<T>::BinomialCoefficientsLookup(3);

typedef BezierPath<double> BezierPathD;
typedef BezierPath<float> BezierPathF;
typedef BezierPath<int> BezierPathI;

#include "../src/BezierPath.cpp"

#endif // BEZIERPATH_H
