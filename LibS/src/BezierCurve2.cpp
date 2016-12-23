#include "..\include\BezierCurve2.h"

namespace ls
{
    template <class T>
    BezierCurve2<T>::BezierCurve2(const std::vector<Vec2<T>>& points) :
        m_controlPoints(points)
    {

    }
    template <class T>
    BezierCurve2<T>::BezierCurve2(std::vector<Vec2<T>>&& points) noexcept(std::is_nothrow_move_constructible<std::vector<Vec2<T>>>::value) :
        m_controlPoints(std::move(points))
    {

    }
    template <class T>
    BezierCurve2<T>::BezierCurve2(std::initializer_list<Vec2<T>> points) :
        m_controlPoints(points)
    {

    }
    template <class T>
    template<class InputIt>
    BezierCurve2<T>::BezierCurve2(InputIt begin, InputIt end) :
        m_controlPoints(begin, end)
    {

    }

    template <class T>
    template <class... Args>
    void BezierCurve2<T>::emplaceControlPoint(Args&&... coords)
    {
        m_controlPoints.emplace_back(std::forward<Args>(coords)...);
    }
    template <class T>
    void BezierCurve2<T>::addControlPoint(const Vec2<T>& controlPoint) &
    {
        m_controlPoints.emplace_back(controlPoint);
    }
    template <class T>
    template<class InputIt>
    void BezierCurve2<T>::addControlPoints(InputIt begin, InputIt end) &
    {
        m_controlPoints.insert(m_controlPoints.end(), begin, end);
    }
    template <class T>
    void BezierCurve2<T>::addControlPoints(std::initializer_list<Vec2<T>> controlPoint) &
    {
        m_controlPoints.insert(m_controlPoints.end(), controlPoint.begin(), controlPoint.end());
    }

    template <class T>
    const Vec2<T>& BezierCurve2<T>::controlPoint(int i) const
    {
        return m_controlPoints[i];
    }
    template <class T>
    Vec2<T>& BezierCurve2<T>::controlPoint(int i)
    {
        return m_controlPoints[i];
    }
    template <class T>
    const std::vector<Vec2<T>>& BezierCurve2<T>::controlPoints() const &
    {
        return m_controlPoints;
    }
    template <class T>
    std::vector<Vec2<T>> BezierCurve2<T>::controlPoints() &&
    {
        return std::move(m_controlPoints);
    }
    template <class T>
    int BezierCurve2<T>::degree() const
    {
        return m_controlPoints.size();
    }

    template <class T>
    Vec2<T> BezierCurve2<T>::evaluate(const T& t) const
    {
        Vec2<T> point(T(0), T(0));
        const int numberOfControlPoints = m_controlPoints.size();
        for(int i = 0; i < numberOfControlPoints; ++i)
        {
            point += m_controlPoints[i] * static_cast<T>(BinomialCoefficients64::value(numberOfControlPoints - 1, i)) * static_cast<T>(std::pow(T(1) - t, numberOfControlPoints - 1 - i)) * static_cast<T>(std::pow(t, i));
        }
        return point;
    }

    template <class T>
    std::vector<Vec2<T>> BezierCurve2<T>::evaluateAll(const int numberOfSamples) const
    {
        return evaluateRange(T(0), T(1), numberOfSamples);
    }

    template <class T>
    std::vector<Vec2<T>> BezierCurve2<T>::evaluateAll(const T& step) const
    {
        return evaluateRange(T(0), T(1), step);
    }

    template <class T>
    std::vector<Vec2<T>> BezierCurve2<T>::evaluateRange(const T& rangeMin, const T& rangeMax, const int numberOfSamples) const
    {
        std::vector<Vec2<T>> result;
        result.reserve(numberOfSamples);

        const T step = (rangeMax - rangeMin) / static_cast<T>(numberOfSamples - 1);

        T t = rangeMin;
        int i = 0;
        while(i < numberOfSamples)
        {
            result.emplace_back(evaluate(t));

            t += step;
            ++i;
        }

        return result;
    }

    template <class T>
    std::vector<Vec2<T>> BezierCurve2<T>::evaluateRange(const T& rangeMin, const T& rangeMax, const T& step) const
    {
        std::vector<Vec2<T>> result;
        result.reserve(static_cast<int>((rangeMax - rangeMin) / step) + 1);

        T t = rangeMin;
        while(t <= rangeMax)
        {
            result.emplace_back(evaluate(t));
            t += step;
        }

        return result;
    }
}