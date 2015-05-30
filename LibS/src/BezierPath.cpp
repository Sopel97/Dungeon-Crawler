template <class T>
void BezierPath<T>::add(const Vec2<T>& point)
{
    controlPoints.push_back(point);
}
template <class T>
void BezierPath<T>::add(const std::vector<Vec2<T>>& points)
{
    controlPoints.insert(controlPoints.end(), points.begin(), points.end());
}
template <class T>
void BezierPath<T>::add(std::initializer_list<Vec2<T>> points)
{
    controlPoints.insert(controlPoints.end(), points.begin(), points.end());
}
template <class T>
int BezierPath<T>::degree() const
{
    return controlPoints.size();
}

template <class T>
Vec2<T> BezierPath<T>::evaluate(T t) const
{
    Vec2<T> point {0.0, 0.0};
    int numberOfControlPoints = controlPoints.size();
    int numberOfCurves = (numberOfControlPoints - 1) / 2;
    for(int i = 0; i < numberOfCurves; ++i)
    {
        int start = i * 2;
        for(int j = 0; j < 3; ++j)
        {
            point += controlPoints[i + j] * m_binomialCoefficients.coefficients(2, j) * pow(1.0 - t, 2 - j) * pow(t, j);
        }
    }
    return point;
}

template <class T>
std::vector<Vec2<T>> BezierPath<T>::evaluateAll(T step) const
{
    std::vector<Vec2<T>> result;
    result.reserve(1.0 / step + 1.0);

    int numberOfControlPoints = controlPoints.size();
    int numberOfCurves = (numberOfControlPoints - 1) / 2;
    step *= numberOfCurves;
    for(int i = 0; i < numberOfCurves; ++i)
    {
        int start = i * 2;
        T t {0.0};
        while(t <= T(1.0))
        {
            Vec2<T> point {0.0, 0.0};
            for(int j = 0; j < 3; ++j)
            {
                point += controlPoints[start + j] * m_binomialCoefficients.coefficients(2, j) * pow(1.0 - t, 2 - j) * pow(t, j);
            }
            result.push_back(point);
            t += step;
        }
    }

    return result;
}
