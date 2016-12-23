#include "..\include\AffineTransformation2.h"

namespace ls
{
    template <class T>
    constexpr AffineTransformation2<T>::AffineTransformation2() noexcept(std::is_nothrow_constructible<T, T>::value) :
        a(1), b(0), c(0),
        d(0), e(1), f(0)
    {
    }

    template <class T>
    constexpr AffineTransformation2<T>::AffineTransformation2(const T& _a, const T& _b, const T& _c, const T& _d, const T& _e, const T& _f) noexcept(std::is_nothrow_copy_constructible<T>::value) :
    a(_a), b(_b), c(_c),
        d(_d), e(_e), f(_f)
    {
    }

    template <class T>
    AffineTransformation2<T> AffineTransformation2<T>::fromTriangleMapping(const Triangle2<T>& from, const Triangle2<T>& to)
    {
        AffineTransformation2<T> result;
        const T& x1 = from.vertices[0].x;
        const T& x2 = from.vertices[1].x;
        const T& x3 = from.vertices[2].x;

        const T& y1 = from.vertices[0].y;
        const T& y2 = from.vertices[1].y;
        const T& y3 = from.vertices[2].y;

        const T& z1 = to.vertices[0].x;
        const T& z2 = to.vertices[1].x;
        const T& z3 = to.vertices[2].x;

        const T& w1 = to.vertices[0].y;
        const T& w2 = to.vertices[1].y;
        const T& w3 = to.vertices[2].y;

        result.a = -(y1*(z3 - z2) - y2*z3 + y3*z2 + (y2 - y3)*z1) / (x1*(y3 - y2) - x2*y3 + x3*y2 + (x2 - x3)*y1);
        result.b = (x1 * (z3 - z2) - x2 * z3 + x3 * z2 + (x2 - x3) * z1) / (x1 * (y3 - y2) - x2 * y3 + x3 * y2 + (x2 - x3) * y1);
        result.c = (x1 * (y3 * z2 - y2 * z3) + y1 * (x2 * z3 - x3 * z2) + (x3 * y2 - x2 * y3) * z1) / (x1 * (y3 - y2) - x2 * y3 + x3 * y2 + (x2 - x3) * y1);
        result.d = (w1 * (y3 - y2) - w2 * y3 + w3 * y2 + (w2 - w3) * y1) / (x1 * (y3 - y2) - x2 * y3 + x3 * y2 + (x2 - x3) * y1);
        result.e = -(w1 * (x3 - x2) - w2 * x3 + w3 * x2 + (w2 - w3) * x1) / (x1 * (y3 - y2) - x2 * y3 + x3 * y2 + (x2 - x3) * y1);
        result.f = (w1 * (x3 * y2 - x2 * y3) + x1 * (w2 * y3 - w3 * y2) + (w3 * x2 - w2 * x3) * y1) / (x1 * (y3 - y2) - x2 * y3 + x3 * y2 + (x2 - x3) * y1);

        return result;
    }
    template <class T>
    AffineTransformation2<T> AffineTransformation2<T>::identity() noexcept(std::is_nothrow_default_constructible<AffineTransformation2<T>>::value)
    {
        return{};
    }

    template <class T>
    AffineTransformation2<T>& AffineTransformation2<T>::rotateClockwise(const Angle2<T>& angle)
    {
        return rotateAntiClockwise(-angle);
    }
    template <class T>
    AffineTransformation2<T>& AffineTransformation2<T>::rotateAntiClockwise(const Angle2<T>& angle)
    {
        const T sine = angle.sin();
        const T cosine = angle.cos();
        const T aa = cosine;
        const T bb = -sine;
        const T dd = sine;
        const T ee = cosine;
        combine(AffineTransformation2<T>(aa, bb, T(0), dd, ee, T(0)));

        return *this;
    }

    template <class T>
    AffineTransformation2<T>& AffineTransformation2<T>::translate(const Vec2<T>& displacement)
    {
        const T cc = displacement.x;
        const T ff = displacement.y;
        combine(AffineTransformation2<T>(T(1), T(0), cc, T(0), T(1), ff));

        return *this;
    }

    template <class T>
    AffineTransformation2<T>& AffineTransformation2<T>::scale(const Vec2<T>& factor)
    {
        const T aa = factor.x;
        const T ee = factor.y;
        combine(AffineTransformation2<T>(aa, T(0), T(0), T(0), ee, T(0)));

        return *this;
    }

    template <class T>
    AffineTransformation2<T>& AffineTransformation2<T>::scale(const T& factor)
    {
        const T ae = factor;
        combine(AffineTransformation2<T>(ae, T(0), T(0), T(0), ae, T(0)));

        return *this;
    }

    template <class T>
    AffineTransformation2<T>& AffineTransformation2<T>::shearInXDirection(const T& factor)
    {
        const T bb = factor;
        combine(AffineTransformation2<T>(T(1), bb, T(0), T(0), T(1), T(0)));

        return *this;
    }
    template <class T>
    AffineTransformation2<T>& AffineTransformation2<T>::shearInYDirection(const T& factor)
    {
        const T dd = factor;
        combine(AffineTransformation2<T>(T(1), T(0), T(0), dd, T(1), T(0)));

        return *this;
    }
    template <class T>
    AffineTransformation2<T>& AffineTransformation2<T>::shear(const Vec2<T>& factor)
    {
        const T bb = factor.x;
        const T dd = factor.y;
        combine(AffineTransformation2<T>(T(1), bb, T(0), dd, T(1), T(0)));

        return *this;
    }

    template <class T>
    AffineTransformation2<T>& AffineTransformation2<T>::reflectOverOrigin()
    {
        combine(AffineTransformation2<T>(T(-1), T(0), T(0), T(0), T(-1), T(0)));

        return *this;
    }
    template <class T>
    AffineTransformation2<T>& AffineTransformation2<T>::reflectOverXAxis()
    {
        combine(AffineTransformation2<T>(T(1), T(0), T(0), T(0), T(-1), T(0)));

        return *this;
    }
    template <class T>
    AffineTransformation2<T>& AffineTransformation2<T>::reflectOverYAxis()
    {
        combine(AffineTransformation2<T>(T(-1), T(0), T(0), T(0), T(1), T(0)));

        return *this;
    }

    template <class T>
    void AffineTransformation2<T>::transform(Vec2<T>& point) const
    {
        const T px = point.x;
        const T py = point.y;

        point.x = a * px + b * py + c;
        point.y = d * px + e * py + f;
    }

    template <class T>
    Vec2<T> AffineTransformation2<T>::transformed(const Vec2<T>& point) const
    {
        const T& px = point.x;
        const T& py = point.y;

        return Vec2<T>(a * px + b * py + c, d * px + e * py + f);
    }
    template <class T>
    AffineTransformation2<T> AffineTransformation2<T>::combined(const AffineTransformation2<T>& transformation) const
    {
        AffineTransformation2<T> combinedTransformation;

        combinedTransformation.a = a * transformation.a + transformation.b * d;
        combinedTransformation.b = transformation.a * b + transformation.b * e;
        combinedTransformation.c = transformation.a * c + transformation.c + transformation.b * f;
        combinedTransformation.d = transformation.a * d + d * transformation.e;
        combinedTransformation.e = b * transformation.d + e * transformation.e;
        combinedTransformation.f = c * transformation.d + transformation.e * f + transformation.f;

        return combinedTransformation;
    }
    template <class T>
    AffineTransformation2<T>& AffineTransformation2<T>::combine(const AffineTransformation2<T>& transformation)
    {
        const T aa = a;
        const T bb = b;
        const T dd = d;
        const T ee = e;

        a = aa * transformation.a + transformation.b * dd;
        b = transformation.a * bb + transformation.b * ee;
        c = transformation.a * c + transformation.c + transformation.b * f;
        d = transformation.a * dd + dd * transformation.e;
        e = bb * transformation.d + ee * transformation.e;
        f = c * transformation.d + transformation.e * f + transformation.f;

        return *this;
    }

    template <class T>
    constexpr bool operator==(const AffineTransformation2<T>& lhs, const AffineTransformation2<T>& rhs)
    {
        return std::tie(lhs.a, lhs.b, lhs.c, lhs.d, lhs.e, lhs.f) == std::tie(rhs.a, rhs.b, rhs.c, rhs.d, rhs.e, rhs.f);
    }
    template <class T>
    constexpr bool operator!=(const AffineTransformation2<T>& lhs, const AffineTransformation2<T>& rhs)
    {
        return !(lhs == rhs);
    }
}