#ifndef LINESEGMENT_H_INCLUDED
#define LINESEGMENT_H_INCLUDED

template <class T>
class LineSegment : public Shape2<T>
{
public:
    Vec2<T> begin;
    Vec2<T> end;

    LineSegment() {}
    LineSegment(const Vec2<T>& _begin, const Vec2<T>& _end);

    LineSegment(const LineSegment<T>& other) { begin = other.begin; end = other.end; }
    LineSegment(LineSegment<T>&& other) { begin = std::move(other.begin); end = std::move(other.end); }

    virtual ~LineSegment() {}

    LineSegment<T>& operator=(const LineSegment<T>& other) { begin = other.begin; end = other.end; return *this; }
    LineSegment<T>& operator=(LineSegment<T> && other) { begin = std::move(other.begin); end = std::move(other.end); return *this; }

    LineSegment<T> operator+(const Vec2<T>& v) const;
    LineSegment<T> operator-(const Vec2<T>& v) const;

    LineSegment<T>& operator+=(const Vec2<T>& v);
    LineSegment<T>& operator-=(const Vec2<T>& v);

    template <class T2>
    explicit operator LineSegment<T2>() const;

    T length() const;

    T distanceTo(const Vec2<T>& v1) const;
    Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    Vec2<T> centerOfMass() const;
};

typedef LineSegment<double> LineSegmentD;
typedef LineSegment<float> LineSegmentF;
typedef LineSegment<int> LineSegmentI;

extern template class LineSegment<double>;
extern template class LineSegment<float>;
extern template class LineSegment<int>;

#include "../src/LineSegment.cpp"
#endif // LINESEGMENT_H_INCLUDED
