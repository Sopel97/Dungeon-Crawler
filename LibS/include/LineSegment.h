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

    virtual T distanceTo(const Vec2<T>& v1) const;
    virtual Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    virtual void translate(const Vec2<T>& v);
    virtual void scale(const Vec2<T>& s);

    virtual void transform(const std::function<void(Vec2<T>&)>& transformationFunction);
    virtual void transform(const Transformation2<T>& transformation);
    LineSegment<T> transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const;
    LineSegment<T> transformed(const Transformation2<T>& transformation) const;

    Polyline<T> asPolyline() const;

    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    virtual Vec2<T> center() const;

#ifndef GEOMETRY_LIGHT
    virtual bool intersects(const Shape2<T>* other) const {return other->intersects(*this);}
    virtual bool contains(const Shape2<T>* other) const {return other->isContained(*this);}
    virtual bool isContained(const Shape2<T>* other) const {return other->contains(*this);}
#endif // GEOMETRY_LIGHT
    SHAPE2_DOUBLE_DISPATCHING_METHODS

    bool intersects(const LineSegment<T>& lineSegment, Vec2<T>& intersectionPoint) const;

    virtual std::unique_ptr<Shape2<T>> clone() const;
};

typedef LineSegment<double> LineSegmentD;
typedef LineSegment<float> LineSegmentF;
typedef LineSegment<int> LineSegmentI;

#include "../src/LineSegment.cpp"
#endif // LINESEGMENT_H_INCLUDED
