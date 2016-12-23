#pragma once

#include "..\Fwd.h"

#include "BezierCurve2.h"
#include "Vec2.h"
#include "BinomialCoefficients.h"

#include <vector>
#include <initializer_list>

namespace ls
{
    template <class T>
    class BezierPath2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;

        BezierPath2() = default;
        BezierPath2(const std::vector<Vec2<T>>& controlPoints);
        BezierPath2(std::vector<Vec2<T>>&& controlPoints) noexcept(std::is_nothrow_move_constructible<std::vector<Vec2<T>>>::value);
        BezierPath2(std::initializer_list<Vec2<T>> controlPoints);
        template<class InputIt>
        BezierPath2(InputIt begin, InputIt end);
        BezierPath2(const BezierPath2<T>& other) = default;
        BezierPath2(BezierPath2<T>&& other) = default;

        BezierPath2<T>& operator=(const BezierPath2<T>& other) & = default;
        BezierPath2<T>& operator=(BezierPath2<T>&& other) & = default;

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

    private:
        std::vector<Vec2<T>> m_controlPoints;
    };

    using BezierPath2D = BezierPath2<double>;
    using BezierPath2F = BezierPath2<float>;

    extern template class BezierPath2<double>;
    extern template class BezierPath2<float>;
}

#include "../src/BezierPath2.cpp"
