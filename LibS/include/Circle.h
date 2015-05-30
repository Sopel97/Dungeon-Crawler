#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

template <class T>
class Circle : public Shape2<T>
{
public:
    Vec2<T> origin;
    T radius;

    Circle() = default;
    Circle(const Vec2<T>& p1, T r);

    Circle(const Circle<T>&) = default;
    template<class X>
    Circle(const Circle<X>&);
    Circle(Circle<T>&&) = default;

    virtual ~Circle(){}

    Circle& operator =(Circle<T> &&) = default;
    Circle& operator =(const Circle<T>&) = default;
    template<class X>
    Circle& operator =(const Circle<X>& c);

    Circle<T> operator +(const Vec2<T>& v) const;
    Circle<T> operator -(const Vec2<T>& v) const;

    Circle<T>& operator +=(const Vec2<T>& v);
    Circle<T>& operator -=(const Vec2<T>& v);

    virtual void translate(const Vec2<T>& v);
    virtual void scale(const Vec2<T>& s);

    virtual void transform(const std::function<void(Vec2<T>&)>& transformationFunction);
    virtual void transform(const Transformation2<T>& transformation);
    Circle<T> transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const;
    Circle<T> transformed(const Transformation2<T>& transformation) const;

    virtual T distanceTo(const Vec2<T>& v1) const;
    virtual Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    Polyline<T> asPolyline() const;

    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    virtual Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
    virtual Vec2<T> center() const;
    virtual T signedArea() const;

#ifndef GEOMETRY_LIGHT
    virtual bool intersects(const Shape2<T>* other) const {return other->intersects(*this);}
    virtual bool contains(const Shape2<T>* other) const {return other->isContained(*this);}
    virtual bool isContained(const Shape2<T>* other) const {return other->contains(*this);}
#endif // GEOMETRY_LIGHT
    SHAPE2_DOUBLE_DISPATCHING_METHODS

    virtual std::unique_ptr<Shape2<T>> clone() const;
};

typedef Circle<double> CircleD;
typedef Circle<float> CircleF;
typedef Circle<int> CircleI;

#include "../src/Circle.cpp"

#endif // CIRCLE_H_INCLUDED
