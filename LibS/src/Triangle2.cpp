#include "..\include\Triangle2.h"

#include "..\Constants.h"

namespace ls
{
    template <class T>
    constexpr Triangle2<T>::Triangle2(const Vec2<T>& v1, const Vec2<T>& v2, const Vec2<T>& v3) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value) :
        vertices{v1, v2, v3}
    {
    }

    template <class T>
    constexpr Triangle2<T> Triangle2<T>::translated(const Vec2<T>& displacement) const
    {
        return Triangle2<T>(vertices[0] + displacement, vertices[1] + displacement, vertices[2] + displacement);
    }
    template <class T>
    Triangle2<T>& Triangle2<T>::translate(const Vec2<T>& displacement) &
    {
        vertices[0] += displacement;
        vertices[1] += displacement;
        vertices[2] += displacement;
        return *this;
    }

    template <class T>
    template <class T2>
    Triangle2<T>::operator Triangle2<T2>() const
    {
        return Triangle2<T2>(static_cast<Vec2<T2>>(vertices[0]), static_cast<Vec2<T2>>(vertices[1]), static_cast<Vec2<T2>>(vertices[2]));
    }

    template <class T>
    constexpr Vec2<T> Triangle2<T>::centerOfMass() const
    {
        return (vertices[0] + vertices[1] + vertices[2]) / T(3);
    }
    template <class T>
    constexpr Triangle2<T> Triangle2<T>::equilateral(const Vec2<T>& center, const T& baseLength) noexcept(std::is_nothrow_constructible<Triangle2<T>, const Vec2<T>&, const Vec2<T>&, const Vec2<T>&>::value)
    {
        return Triangle2<T>(
            Vec2<T>(center.x - baseLength * T(0.5), center.y - (baseLength * static_cast<T>(SQRT3) * T(0.5)) * T(0.5)),
            Vec2<T>(center.x, center.y + (baseLength * static_cast<T>(SQRT3) * T(0.5)) * T(0.5)),
            Vec2<T>(center.x - baseLength * T(0.5), center.y + (baseLength * static_cast<T>(SQRT3) * T(0.5)) * T(0.5))
            );
    }
    template <class T>
    constexpr Triangle2<T> Triangle2<T>::isosceles(const Vec2<T>& center, const T& baseLength, const T& height) noexcept(std::is_nothrow_constructible<Triangle2<T>, const Vec2<T>&, const Vec2<T>&, const Vec2<T>&>::value)
    {
        return Triangle2<T>(
            Vec2<T>(center.x - baseLength * T(0.5), center.y - height * T(0.5)),
            Vec2<T>(center.x, center.y + height * T(0.5)),
            Vec2<T>(center.x + baseLength * T(0.5), center.y - height * T(0.5))
            );
    }
    template <class T>
    constexpr Triangle2<T> Triangle2<T>::right(const Vec2<T>& rightAngledVertex, const T& width, const T& height) noexcept(std::is_nothrow_constructible<Triangle2<T>, const Vec2<T>&, const Vec2<T>&, const Vec2<T>&>::value)
    {
        return Triangle2<T>(
            rightAngledVertex,
            rightAngledVertex + Vec2<T> {T(0), height},
            rightAngledVertex + Vec2<T> {width, T(0)}
        );
    }
    template <class T>
    constexpr T Triangle2<T>::signedArea() const
    {
        return ((vertices[1] - vertices[0]).cross(vertices[2] - vertices[0])) * T(0.5);
    }
    template <class T>
    constexpr T Triangle2<T>::area() const
    {
        return std::abs(signedArea());
    }

    template <class T>
    Vec2<T> Triangle2<T>::circumcenter() const
    {
        /*

        Let A = (x1, y1) - vertices
            B = (x2, y2)
            C = (x3, y3)

            O = (xo, yo) - circumcenter

        S_AB = ((x1+x2)/2, (y1+y2)/2)
        S_AC = ((x1+x3)/2, (y1+y3)/2)

        k: (A in k) and (C in k)
        l: (A in k) and (B in k)

        k': (k' perp to k) and (S_AC in k')
        l': (l' perp to l) and (S_AB in l')

        l: (x-x1)(y3-y1) - (y-y1)(x3-x1) = 0
        l: (y3-y1)x - (x3-x1)y + [(x3-x1)y1 - (y3-y1)x1] = 0

        l': (x3-x1)x - (y3-y1)y + b = 0
            (x3-x1)(x1+x3)/2 + (y3-y1)(y1+y3)/2 + b = 0 , after substituting S_AC
            b = [(x1^2 + y1^2) - (x3^2 + y3^2)]/2

        l': (x3-x1)x - (y3-y1)y + [(x1^2 + y1^2) - (x3^2 + y3^2)]/2 = 0
        By analogy:
        k': (x2-x1)x - (y2-y1)y + [(x1^2 + y1^2) - (x2^2 + y2^2)]/2 = 0


        O: {k, l

        W = (x3-x1)  (y2-y1) - (y3-y1)  (x2-x1)
            ^^^^^^^  ^^^^^^^   ^^^^^^^  ^^^^^^^
               f        e         g        d


        Wx = [(x3^2 + y3^2) - (x1^2 + y1^2)]/2 * (y2-y1) - [(x2^2 + y2^2) - (x1^2 + y1^2)]/2 * (y3-y1)
              ^^^^^^^^^^^^^   ^^^^^^^^^^^^^      ^^^^^^^    ^^^^^^^^^^^^^   ^^^^^^^^^^^^^      ^^^^^^^
                    c               a               e             b               a               g
             ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^             ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
                             i                                             h


        Wy = [(x2^2 + y2^2) - (x1^2 + y1^2)]/2 * (x3-x1) - [(x3^2 + y3^2) - (x1^2 + y1^2)]/2 * (x2-x1)
             ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   ^^^^^^^   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   ^^^^^^^
                             h                      f                      i                      d

        xo = Wx / W
        yo = Wy / W

        */

        const T& x1 = vertices[0].x;
        const T& x2 = vertices[1].x;
        const T& x3 = vertices[2].x;

        const T& y1 = vertices[0].y;
        const T& y2 = vertices[1].y;
        const T& y3 = vertices[2].y;

        const T a = x1*x1 + y1*y1;
        const T b = x2*x2 + y2*y2;
        const T c = x3*x3 + y3*y3;

        const T d = x2 - x1;
        const T e = y2 - y1;
        const T f = x3 - x1;
        const T g = y3 - y1;

        const T h = T(0.5)*(b - a);
        const T i = T(0.5)*(c - a);

        const T W = e*f - d*g;
        const T Wx = i*e - h*g;
        const T Wy = h*f - i*d;

        return Vec2<T>(Wx / W, Wy / W);
    }
    template <class T>
    Circle2<T> Triangle2<T>::circumcircle() const
    {
        Vec2<T> origin = circumcenter();
        const T radius = origin.distance(vertices[0]);

        return Circle2<T>(origin, radius);
    }


    template <class T>
    ConstTriangle2View<T> ConstTriangle2View<T>::ordered(const Vec2<T>* a, const Vec2<T>* b, const Vec2<T>* c) noexcept
    {
        ConstTriangle2View<T> object(a, b, c);

        object.sort();

        return object;
    }

    template <class T>
    ConstTriangle2View<T>::ConstTriangle2View(const Vec2<T>* a, const Vec2<T>* b, const Vec2<T>* c) noexcept :
    m_v{a, b, c}
    {

    }

    template <class T>
    const Vec2<T>& ConstTriangle2View<T>::a() const
    {
        return *(m_v[0]);
    }
    template <class T>
    const Vec2<T>& ConstTriangle2View<T>::b() const
    {
        return *(m_v[1]);
    }
    template <class T>
    const Vec2<T>& ConstTriangle2View<T>::c() const
    {
        return *(m_v[2]);
    }

    template <class T>
    const Vec2<T>* ConstTriangle2View<T>::ptrToA() const
    {
        return m_v[0];
    }
    template <class T>
    const Vec2<T>* ConstTriangle2View<T>::ptrToB() const
    {
        return m_v[1];
    }
    template <class T>
    const Vec2<T>* ConstTriangle2View<T>::ptrToC() const
    {
        return m_v[2];
    }

    template <class T>
    void ConstTriangle2View<T>::setPtrToA(const Vec2<T>* newPtrToA) &
    {
        m_v[0] = newPtrToA;
    }
    template <class T>
    void ConstTriangle2View<T>::setPtrToB(const Vec2<T>* newPtrToB) &
    {
        m_v[1] = newPtrToB;
    }
    template <class T>
    void ConstTriangle2View<T>::setPtrToC(const Vec2<T>* newPtrToC) &
    {
        m_v[2] = newPtrToC;
    }

    template <class T>
    void ConstTriangle2View<T>::sort() &
    {
        if(m_v[1] > m_v[2]) std::swap(m_v[1], m_v[2]);
        if(m_v[0] > m_v[2]) std::swap(m_v[0], m_v[2]);
        if(m_v[0] > m_v[1]) std::swap(m_v[0], m_v[1]);
    }

    template <class T>
    Triangle2<T> ConstTriangle2View<T>::toTriangle() const
    {
        return Triangle2<T>(a(), b(), c());
    }

    template <class T>
    bool operator==(const ConstTriangle2View<T>& lhs, const ConstTriangle2View<T>& rhs)
    {
        return std::tie(lhs.a(), lhs.b(), lhs.c()) == std::tie(rhs.a(), rhs.b(), rhs.c());
    }
    template <class T>
    bool operator!=(const ConstTriangle2View<T>& lhs, const ConstTriangle2View<T>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T>
    bool operator<(const ConstTriangle2View<T>& lhs, const ConstTriangle2View<T>& rhs)
    {
        return std::tie(lhs.a(), lhs.b(), lhs.c()) < std::tie(rhs.a(), rhs.b(), rhs.c());
    }
    template <class T>
    bool operator<=(const ConstTriangle2View<T>& lhs, const ConstTriangle2View<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    bool operator>(const ConstTriangle2View<T>& lhs, const ConstTriangle2View<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    bool operator>=(const ConstTriangle2View<T>& lhs, const ConstTriangle2View<T>& rhs)
    {
        return !(lhs < rhs);
    }


}