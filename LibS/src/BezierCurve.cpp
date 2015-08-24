template <class T>
BezierCurve<T>::BinomialCoefficientsLookup::BinomialCoefficientsLookup(int size) : coefficients(size, size)
{
    coefficients(0, 0) = 1;
    for(int y = 1; y < size; ++y)
    {
        for(int x = 0; x <= y; ++x)
        {
            int left = x > 0 ? coefficients(y - 1, x - 1) : 0;
            int right = x < y ? coefficients(y - 1, x) : 0;
            coefficients(y, x) = left + right;
        }
    }
}
template <class T>
void BezierCurve<T>::add(const Vec2<T>& point)
{
    controlPoints.push_back(point);
}
template <class T>
void BezierCurve<T>::add(const std::vector<Vec2<T>>& points)
{
    controlPoints.insert(controlPoints.end(), points.begin(), points.end());
}
template <class T>
void BezierCurve<T>::add(std::initializer_list<Vec2<T>> points)
{
    controlPoints.insert(controlPoints.end(), points.begin(), points.end());
}
template <class T>
int BezierCurve<T>::degree() const
{
    return controlPoints.size();
}

template <class T>
Vec2<T> BezierCurve<T>::evaluate(T t, const BinomialCoefficientsLookup& binomialCoefficients) const
{
    Vec2<T> point {T(0), T(0)};
    int numberOfControlPoints = controlPoints.size();
    for(int i = 0; i < numberOfControlPoints; ++i)
    {
        point += controlPoints[i] * binomialCoefficients.coefficients(numberOfControlPoints - 1, i) * pow(T(1) - t, numberOfControlPoints - 1 - i) * pow(t, i);
    }
    return point;
}

template <class T>
std::vector<Vec2<T>> BezierCurve<T>::evaluateAll(T step, const BinomialCoefficientsLookup& binomialCoefficients) const
{
    std::vector<Vec2<T>> result;
    result.reserve(1.0 / step + 1);

    int numberOfControlPoints = controlPoints.size();
    T t = 0;
    while(t <= T(1))
    {
        Vec2<T> point {T(0), T(0)};
        for(int i = 0; i < numberOfControlPoints; ++i)
        {
            point += controlPoints[i] * binomialCoefficients.coefficients(numberOfControlPoints - 1, i) * pow(T(1) - t, numberOfControlPoints - 1 - i) * pow(t, i);
        }
        result.push_back(point);
        t += step;
    }

    return result;
}
template <class T>
std::vector<Vec2<T>> BezierCurve<T>::evaluateAll(T step) const
{
    BinomialCoefficientsLookup binomialCoefficients(controlPoints.size());
    return evaluateAll(step, binomialCoefficients);
}
