#ifndef BEZIERCURVW_H
#define BEZIERCURVW_H

template <class T>
class BezierCurve
{
public:
    class BinomialCoefficientsLookup
    {
    public:
        Array2<T> coefficients;
        BinomialCoefficientsLookup(int size);
    };

    std::vector<Vec2<T>> controlPoints;

    BezierCurve() = default;

    void add(const Vec2<T>& point);
    void add(const std::vector<Vec2<T>>& points);
    void add(std::initializer_list<Vec2<T>> points);

    int degree() const;

    Vec2<T> evaluate(T t, const BinomialCoefficientsLookup& binomialCoefficients) const;

    std::vector<Vec2<T>> evaluateAll(T step, const BinomialCoefficientsLookup& binomialCoefficients) const;
    std::vector<Vec2<T>> evaluateAll(T step) const;
};

typedef BezierCurve<double> BezierCurveD;
typedef BezierCurve<float> BezierCurveF;
typedef BezierCurve<int> BezierCurveI;

#include "../src/BezierCurve.cpp"

#endif // BEZIERCURVW_H
