#include "..\include\BezierPath2.h"

namespace ls
{
    template <class T>
    BezierPath2<T>::BezierPath2(const std::vector<Vec2<T>>& controlPoints) :
        m_controlPoints(controlPoints)
    {

    }
    template <class T>
    BezierPath2<T>::BezierPath2(std::vector<Vec2<T>>&& controlPoints) noexcept(std::is_nothrow_move_constructible<std::vector<Vec2<T>>>::value) :
        m_controlPoints(std::move(controlPoints))
    {

    }
    template <class T>
    BezierPath2<T>::BezierPath2(std::initializer_list<Vec2<T>> controlPoints) :
        m_controlPoints(controlPoints)
    {

    }
    template <class T>
    template<class InputIt>
    BezierPath2<T>::BezierPath2(InputIt begin, InputIt end) :
        m_controlPoints(begin, end)
    {

    }

    template <class T>
    template <class... Args>
    void BezierPath2<T>::emplaceControlPoint(Args&&... coords)
    {
        m_controlPoints.emplace_back(std::forward<Args>(coords)...);
    }
    template <class T>
    void BezierPath2<T>::addControlPoint(const Vec2<T>& controlPoint) &
    {
        m_controlPoints.emplace_back(controlPoint);
    }
    template <class T>
    template <class InputIt>
    void BezierPath2<T>::addControlPoints(InputIt begin, InputIt end) &
    {
        m_controlPoints.insert(m_controlPoints.end(), begin, end);
    }
    template <class T>
    void BezierPath2<T>::addControlPoints(std::initializer_list<Vec2<T>> controlPoints) &
    {
        m_controlPoints.insert(m_controlPoints.end(), controlPoints.begin(), controlPoints.end());
    }
    template <class T>
    const Vec2<T>& BezierPath2<T>::controlPoint(int i) const
    {
        return m_controlPoints[i];
    }
    template <class T>
    Vec2<T>& BezierPath2<T>::controlPoint(int i)
    {
        return m_controlPoints[i];
    }
    template <class T>
    const std::vector<Vec2<T>>& BezierPath2<T>::controlPoints() const &
    {
        return m_controlPoints;
    }
    template <class T>
    std::vector<Vec2<T>> BezierPath2<T>::controlPoints() &&
    {
        return std::move(m_controlPoints);
    }
    template <class T>
    int BezierPath2<T>::degree() const
    {
        return m_controlPoints.size();
    }

    template <class T>
    Vec2<T> BezierPath2<T>::evaluate(const T& t) const
    {
        Vec2<T> point(T(0), T(0));
        const int numberOfControlPoints = m_controlPoints.size();
        const int numberOfSubCurves = (numberOfControlPoints - 1) / 3;

        const T tt = t*static_cast<T>(numberOfSubCurves);
        const int tti = Util::fastFloor(tt);
        int subCurveAtT = tti;

        if(subCurveAtT < 0) subCurveAtT = 0;
        if(subCurveAtT >= numberOfSubCurves) subCurveAtT = numberOfSubCurves - 1;

        const int firstControlPoint = subCurveAtT * 3;
        const Vec2<T>& controlPoint0 = m_controlPoints[firstControlPoint];
        const Vec2<T>& controlPoint1 = m_controlPoints[firstControlPoint + 1];
        const Vec2<T>& controlPoint2 = m_controlPoints[firstControlPoint + 2];
        const Vec2<T>& controlPoint3 = m_controlPoints[firstControlPoint + 3];

        const T localT = tt - static_cast<T>(tti);
        point += controlPoint0 * static_cast<T>(std::pow(T(1) - localT, 3));
        point += controlPoint1 * static_cast<T>(std::pow(T(1) - localT, 2)) * 3;
        point += controlPoint2 * static_cast<T>(std::pow(T(1) - localT, 2)) * 3;
        point += controlPoint3 * static_cast<T>(std::pow(T(1) - localT, 3));

        return point;
    }

    template <class T>
    std::vector<Vec2<T>> BezierPath2<T>::evaluateAll(const int numberOfSamples) const
    {
        return evaluateRange(T(0), T(1), numberOfSamples);
    }

    template <class T>
    std::vector<Vec2<T>> BezierPath2<T>::evaluateAll(const T& step) const
    {
        return evaluateRange(T(0), T(1), step);
    }


    template <class T>
    std::vector<Vec2<T>> BezierPath2<T>::evaluateRange(const T& rangeMin, const T& rangeMax, const int numberOfSamples) const
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
    std::vector<Vec2<T>> BezierPath2<T>::evaluateRange(const T& rangeMin, const T& rangeMax, const T& step) const
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
