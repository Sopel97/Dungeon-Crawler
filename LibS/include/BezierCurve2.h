#pragma once

#include "..\Fwd.h"

#include "Vec2.h"
#include "..\Array2.h"
#include "BinomialCoefficients.h"

#include <vector>
#include <initializer_list>

namespace ls
{
    template <class T>
    class BezierCurve2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;

        BezierCurve2() = default;
        BezierCurve2(const std::vector<Vec2<T>>& controlPoints);
        BezierCurve2(std::vector<Vec2<T>>&& controlPoints) noexcept(std::is_nothrow_move_constructible<std::vector<Vec2<T>>>::value);
        BezierCurve2(std::initializer_list<Vec2<T>> controlPoints);
        template<class InputIt>
        BezierCurve2(InputIt begin, InputIt end);
        BezierCurve2(const BezierCurve2<T>& other) = default;
        BezierCurve2(BezierCurve2<T>&& other) = default;

        BezierCurve2<T>& operator=(const BezierCurve2<T>& other) & = default;
        BezierCurve2<T>& operator=(BezierCurve2<T>&& other) & = default;
        
        template <class... Args>
        void emplaceControlPoint(Args&&... coords);
        void addControlPoint(const Vec2<T>& controlPoint) &;
        template<class InputIt>
        void addControlPoints(InputIt begin, InputIt end) &;
        void addControlPoints(std::initializer_list<Vec2<T>> controlPoints) &;

        const Vec2<T>& controlPoint(int i) const;
        Vec2<T>& controlPoint(int i);
        const std::vector<Vec2<T>>& controlPoints() const &;
        std::vector<Vec2<T>> controlPoints() &&;

        int degree() const;
        
        Vec2<T> evaluate(const T& t) const;
        std::vector<Vec2<T>> evaluateAll(const int numberOfSamples) const;
        std::vector<Vec2<T>> evaluateRange(const T& rangeMin, const T& rangeMax, const int numberOfSamples) const;
        std::vector<Vec2<T>> evaluateAll(const T& step) const;
        std::vector<Vec2<T>> evaluateRange(const T& rangeMin, const T& rangeMax, const T& step) const;

    protected:
        std::vector<Vec2<T>> m_controlPoints;
    };

    using BezierCurve2D = BezierCurve2<double>;
    using BezierCurve2F = BezierCurve2<float>;

    extern template class BezierCurve2<double>;
    extern template class BezierCurve2<float>;
}

#include "../src/BezierCurve2.cpp"
