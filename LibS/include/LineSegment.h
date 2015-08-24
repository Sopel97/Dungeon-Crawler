#ifndef LINESEGMENT_H_INCLUDED
#define LINESEGMENT_H_INCLUDED

template <class T>
class LineSegment : public Shape2<T>
{
public:
    Vec2<T> begin;
    Vec2<T> end;

    LineSegment() = default;
    LineSegment(const Vec2<T>& _begin, const Vec2<T>& _end);

    LineSegment(const LineSegment<T>&) = default;
    template <class X>
    LineSegment(const LineSegment<X>& s);
    LineSegment(LineSegment<T>&&) = default;

    virtual ~LineSegment(){}

    LineSegment<T>& operator=(const LineSegment<T>&) = default;
    template <class X>
    LineSegment<T>& operator=(const LineSegment<X>& s);
    LineSegment<T>& operator=(LineSegment<T> &&) = default;

    LineSegment<T> operator+(const Vec2<T>& v) const;
    LineSegment<T> operator-(const Vec2<T>& v) const;

    LineSegment<T>& operator+=(const Vec2<T>& v);
    LineSegment<T>& operator-=(const Vec2<T>& v);

    T length() const;

    T distanceTo(const Vec2<T>& v1) const;
    Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& s);

    Polyline<T> asPolyline() const;

    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    Vec2<T> center() const;

    bool intersects(const LineSegment<T>& lineSegment, Vec2<T>& intersectionPoint) const;
};

typedef LineSegment<double> LineSegmentD;
typedef LineSegment<float> LineSegmentF;
typedef LineSegment<int> LineSegmentI;

extern template class LineSegment<double>;
extern template class LineSegment<float>;
extern template class LineSegment<int>;

#include "../src/LineSegment.cpp"
#endif // LINESEGMENT_H_INCLUDED
