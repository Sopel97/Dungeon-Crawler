#include "..\include\LineSegment2.h"

namespace ls
{
    template <class T>
    constexpr LineSegment2<T>::LineSegment2(const Vec2<T>& _begin, const Vec2<T>& _end) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value) :
        begin(_begin),
        end(_end)
    {
    }

    template <class T>
    constexpr LineSegment2<T> LineSegment2<T>::translated(const Vec2<T>& displacement) const
    {
        return LineSegment2<T>(begin + displacement, end + displacement);
    }
    template <class T>
    LineSegment2<T>& LineSegment2<T>::translate(const Vec2<T>& displacement) &
    {
        begin += displacement;
        end += displacement;

        return *this;
    }

    template <class T>
    template <class T2>
    LineSegment2<T>::operator LineSegment2<T2>() const
    {
        return LineSegment2<T2>(static_cast<Vec2<T2>>(begin), static_cast<Vec2<T2>>(end));
    }

    template <class T>
    constexpr T LineSegment2<T>::length() const
    {
        return begin.distance(end);
    }

    template <class T>
    constexpr Vec2<T> LineSegment2<T>::centerOfMass() const
    {
        return (begin + end) * T(0.5);
    }

    template <class T>
    constexpr bool operator== (const LineSegment2<T>& lhs, const LineSegment2<T>& rhs)
    {
        return std::tie(lhs.begin, lhs.end) == std::tie(rhs.begin, rhs.end);
    }
    template <class T>
    constexpr bool operator!= (const LineSegment2<T>& lhs, const LineSegment2<T>& rhs)
    {
        return !(lhs == rhs);
    }



    template <class T>
    ConstLineSegment2View<T> ConstLineSegment2View<T>::ordered(const Vec2<T>* a, const Vec2<T>* b) noexcept
    {
        ConstLineSegment2View<T> object(a, b);

        object.sort();

        return object;
    }

    template <class T>
    ConstLineSegment2View<T>::ConstLineSegment2View(const Vec2<T>* a, const Vec2<T>* b) noexcept :
    m_v{a, b}
    {

    }

    template <class T>
    const Vec2<T>& ConstLineSegment2View<T>::begin() const
    {
        return *(m_v[0]);
    }
    template <class T>
    const Vec2<T>& ConstLineSegment2View<T>::end() const
    {
        return *(m_v[1]);
    }

    template <class T>
    const Vec2<T>* ConstLineSegment2View<T>::ptrBegin() const
    {
        return m_v[0];
    }
    template <class T>
    const Vec2<T>* ConstLineSegment2View<T>::ptrEnd() const
    {
        return m_v[1];
    }

    template <class T>
    void ConstLineSegment2View<T>::setPtrBegin(const Vec2<T>* newA) &
    {
        m_v[0] = newA;
    }
    template <class T>
    void ConstLineSegment2View<T>::setPtrEnd(const Vec2<T>* newB) &
    {
        m_v[1] = newB;
    }

    template <class T>
    void ConstLineSegment2View<T>::sort() &
    {
        if(m_v[0] > m_v[1]) std::swap(m_v[0], m_v[1]);
    }

    template <class T>
    LineSegment2<T> ConstLineSegment2View<T>::toLineSegment() const
    {
        return LineSegment2<T>(begin(), end());
    }

    template <class T>
    bool operator==(const ConstLineSegment2View<T>& lhs, const ConstLineSegment2View<T>& rhs)
    {
        return std::tie(lhs.m_v[0], lhs.m_v[1]) == std::tie(rhs.m_v[0], rhs.m_v[1]);
    }
    template <class T>
    bool operator!=(const ConstLineSegment2View<T>& lhs, const ConstLineSegment2View<T>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T>
    bool operator<(const ConstLineSegment2View<T>& lhs, const ConstLineSegment2View<T>& rhs)
    {
        return std::tie(lhs.m_v[0], lhs.m_v[1]) < std::tie(rhs.m_v[0], rhs.m_v[1]);
    }
    template <class T>
    bool operator>(const ConstLineSegment2View<T>& lhs, const ConstLineSegment2View<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    bool operator<=(const ConstLineSegment2View<T>& lhs, const ConstLineSegment2View<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    bool operator>=(const ConstLineSegment2View<T>& lhs, const ConstLineSegment2View<T>& rhs)
    {
        return !(lhs < rhs);
    }

}

